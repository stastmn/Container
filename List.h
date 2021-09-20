#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <algorithm>
#include <cassert>
#include <random>
#include <chrono>

const int  _NUM_OF_CHUNK_ELEMENTS = 8;

template<typename ValueType>
class Chunk;

template<typename ValueType>
void ConnectChunks(Chunk<ValueType>* first, Chunk<ValueType>* second)
{
    first->next = second;
    second->prev = first;
}

template<typename ValueType>
class Chunk
{
public:
    Chunk() : lastElementIndex(0), next(nullptr), prev(nullptr) {		}
    ~Chunk() = default;

    ValueType& operator[](int index)
    {
        return elements[index];
    }

    //false - not filled, true - filled
    bool isFilled()
    {
        return lastElementIndex >= _NUM_OF_CHUNK_ELEMENTS;
    }

    void pushFront(const ValueType& value)
    {
        assert(lastElementIndex >= 0 && lastElementIndex < _NUM_OF_CHUNK_ELEMENTS  && "Unable to insert. Bad size.");

        std::copy(elements, elements + lastElementIndex, elements + 1);

        elements[0] = value;

        lastElementIndex++;

    }

    void pushBack(const ValueType& value)
    {
        assert(lastElementIndex >= 0 && lastElementIndex < _NUM_OF_CHUNK_ELEMENTS  && "Unable to insert. Bad size.");

        elements[lastElementIndex] = value;
        lastElementIndex++;
    }


    void remove(int index)
    {
        assert(index < lastElementIndex && "Bad index.");

        if (lastElementIndex > 0) {
            std::copy(elements + index + 1, elements + lastElementIndex, elements + index);
            lastElementIndex--;
        }

    }
    int size() { return lastElementIndex; }

    void removeHead()
    {
        remove(0);
    }
    void removeTail()
    {
        lastElementIndex--;
    }

    ValueType& first()
    {
        return elements[0];
    }

    ValueType& last()
    {
        return  elements[lastElementIndex - 1];
    }


    Chunk<ValueType>* prev;
    Chunk<ValueType>* next;
    int lastElementIndex;
private:


    ValueType elements[_NUM_OF_CHUNK_ELEMENTS];

};




template<typename ValueType>
class List
{
public:
    List();
    ~List();
    void insertHead(const ValueType& value);
    void insertTail(const ValueType& value);
    void removeHead();
    void removeTail();

    const ValueType& head() const;
    const ValueType& tail()const;
    int size() const;



    class Iterator
    {

    public:
        explicit Iterator(List<ValueType>* parent);
        ValueType& get()const;
        void set(const ValueType& value);
        void insert(const ValueType& value);
        void remove();
        void next();
        void prev();
        bool hasNext() const;
        bool hasPrev() const;
        void reset();
        void randomReset();

        ~Iterator()= default;

    private:
        int indexPosition;
        List<ValueType>& list;
        Chunk<ValueType>* chunk;

    };

    Iterator& iterator();
    const Iterator& iterator() const;
private:
    int NumOfElements;
    Chunk<ValueType>* firstChunk;
    Chunk<ValueType>* lastChunk;

    List(const List& a) = delete;
    void operator=(const List& a) = delete;

};

template <typename ValueType>
List<ValueType>::List() : NumOfElements(0)
{
    firstChunk = new Chunk<ValueType>();
    lastChunk = firstChunk;
}

template <typename ValueType>
List<ValueType>::~List()
{
    Chunk<ValueType>* tmp;
    while (firstChunk != lastChunk)
    {
        tmp = lastChunk->prev;
        delete lastChunk;
        lastChunk = tmp;
    }
    delete firstChunk;
}

template <typename ValueType>
void List<ValueType>::insertHead(const ValueType& value)
{
    //Create new chunk if its filled
    if (firstChunk->isFilled())
    {
        Chunk<ValueType>* tmp = new Chunk<ValueType>();
        ConnectChunks(tmp, firstChunk);
        tmp->pushFront(value);
        firstChunk = tmp;
    }
    else
    {
        firstChunk->pushFront(value);
    }
    NumOfElements++;
}

template <typename ValueType>
void List<ValueType>::insertTail(const ValueType& value)
{
    //Create new chunk if its filled
    if (lastChunk->isFilled())
    {
        Chunk<ValueType>* tmp = new Chunk<ValueType>();
        ConnectChunks(lastChunk, tmp);
        tmp->pushBack(value);
        lastChunk = tmp;
    }
    else
    {
        lastChunk->pushBack(value);
    }
    NumOfElements++;
}

template <typename ValueType>
void List<ValueType>::removeHead()
{
    firstChunk->removeHead();

    //Delete chunk if its empty
    if (firstChunk->size() == 0)
    {
        if (firstChunk != lastChunk) {
            Chunk<ValueType>* tmp = firstChunk->next;
            delete firstChunk;
            firstChunk = tmp;
            firstChunk->prev = nullptr;
        }
        NumOfElements--;
    }
    else
    {
        NumOfElements--;
    }

}

template <typename ValueType>
void List<ValueType>::removeTail()
{
    lastChunk->removeTail();

    //Delete chunk if its empty
    if (lastChunk->size() == 0)
    {
        if (firstChunk != lastChunk) {
            Chunk<ValueType>* tmp = lastChunk->prev;
            delete lastChunk;
            lastChunk = tmp;
            lastChunk->next = nullptr;
        }
        NumOfElements--;

    }
    else
    {
        NumOfElements--;
    }
}

template <typename ValueType>
const ValueType& List<ValueType>::head() const
{
    return firstChunk->first();
}

template <typename ValueType>
const ValueType& List<ValueType>::tail() const
{
    return lastChunk->last();
}

template <typename ValueType>
int List<ValueType>::size() const
{
    return NumOfElements;
}

template <typename ValueType>
List<ValueType>::Iterator::Iterator(List<ValueType>* parent) :list(*parent), indexPosition(0), chunk(list.firstChunk)
{
}

template <typename ValueType>
ValueType& List<ValueType>::Iterator::get() const
{
    return  chunk->operator[](indexPosition);
}

template <typename ValueType>
void List<ValueType>::Iterator::set(const ValueType& value)
{
    chunk->operator[](indexPosition) = value;
}


template <typename ValueType>
void List<ValueType>::Iterator::insert(const ValueType& value)
{

    if (chunk->isFilled())
    {
        Chunk<ValueType>* tmp = new Chunk<ValueType>();
        if (chunk->next) {
            ConnectChunks(tmp, chunk->next);
        }
        ConnectChunks(chunk, tmp);
        tmp->pushBack(value);

    }
    else
    {
        chunk->pushBack(value);
    }
    list.NumOfElements++;
}

template <typename ValueType>
void List<ValueType>::Iterator::remove()
{
    if (chunk->size() > 1) {
        chunk->remove(indexPosition);
        list.NumOfElements--;

    }
    else
    {
        //last element in chunk. Need to delete this chunk.

        if (chunk == list.firstChunk) {
            list.removeHead();
            chunk = list.firstChunk;
            return;
        }
        if (chunk == list.lastChunk) {
            list.removeTail();
            chunk = list.lastChunk;
            return;
        }

        auto tmp = chunk->next;
        ConnectChunks(chunk->prev, chunk->next);
        delete chunk;
        chunk = tmp;
        list.NumOfElements--;

    }



}



template <typename ValueType>
void List<ValueType>::Iterator::next()
{
    if (indexPosition == chunk->lastElementIndex - 1 && chunk->next)
    {
        chunk = chunk->next;
        indexPosition = 0;
    }
    else if (indexPosition < _NUM_OF_CHUNK_ELEMENTS && indexPosition < chunk->lastElementIndex)
        indexPosition++;

}

template <typename ValueType>
void List<ValueType>::Iterator::prev()
{

    if (indexPosition > 0) { indexPosition--; return; }
    if (indexPosition == 0 && chunk->prev)
    {
        chunk = chunk->prev;
        indexPosition = chunk->lastElementIndex - 1;
    }

}


template <typename ValueType>
bool List<ValueType>::Iterator::hasNext() const
{
    if (indexPosition < chunk->lastElementIndex - 1) return true;
    if (indexPosition == chunk->lastElementIndex - 1 && chunk->next != nullptr) return true;

    return false;
}

template <typename ValueType>
bool List<ValueType>::Iterator::hasPrev() const
{
    if (indexPosition > 0) return true;
    if (indexPosition == 0 && chunk->prev != nullptr) return true;
    return  false;

}

template<typename ValueType>
inline void List<ValueType>::Iterator::reset()
{
    this->chunk = list.firstChunk;
    this->indexPosition = 0;
}

template<typename ValueType>
void List<ValueType>::Iterator::randomReset()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine engine(seed);
    static std::uniform_real_distribution<double>distribution(0, list.NumOfElements);
    this->reset();

    unsigned int randomIndex = (int)distribution(engine) % list.NumOfElements;

    for (int i = 0; i < randomIndex; i++)
        this->next();

}

template <typename ValueType>
typename List<ValueType>::Iterator& List<ValueType>::iterator()
{
    return *(new Iterator(this));
}

template <typename ValueType>
const typename List<ValueType>::Iterator& List<ValueType>::iterator() const
{
    return *(new Iterator(this));
}








#endif //CONTAINER_LIST_H
