#include "Tests.h"
#include <iostream>
#include <cassert>

void DefaultConstructor()
{
    List<int> list;
    assert(list.size() == 0);

    auto it = list.iterator();
    assert(it.hasNext() == false);
    assert(it.hasPrev() == false);
}

void InsertTest() {

    List<int> list;
    for (auto i = 0; i < 10; ++i)
        list.insertTail(i);

    assert(list.size() == 10);

    auto it = list.iterator();
    for (int i = 0; it.hasNext(); it.next(), i++) {

        assert(it.get() == i);

    }


}

void RemoveTail() {

    List<int> list;

    for (int i = 0; i < 20; i++) {
        list.insertTail(i);

        assert(list.tail() == i);
    }

    for (int i = 20; i > 0; i--) {
        assert(list.size() == i);
        list.removeTail();
    }

}

void RemoveHead() {

    List<int> list;

    for (int i = 0; i < 30; i++) {
        list.insertHead(i);

        assert(list.head() == i);
    }

    for (int i = 30; i > 0; i--) {
        assert(list.size() == i);
        list.removeHead();
    }

}


void IteratorTest() {

    List<int> list;
    for (int i = 0; i < 25; i++) {
        list.insertTail(i);
    }


    auto it = list.iterator();
    for (int i = 0; i < 25; i++) {
        assert(it.get() == i);
        it.set(666);
        it.next();
    }

    it.reset();
    for (int i = 0; i < 25; i++) {
        assert(it.get() == 666);
    }

    it.reset();
    for (int i = 25; i >0; i--) {
        assert(list.size() == i);
        it.remove();
    }
    assert(list.size() == 0);

}

void RandomElement() {

    List<int> list;

    for (int i = 0; i < 30; i++) {
        list.insertTail(i);

    }

    auto it = list.iterator();
    for (int i = 0; i < 50; i++) {
        it.randomReset();

        std::cout << it.get() << "\t";

        if ((i + 1) % 5 == 0)
            std::cout << "\n";

    }

}


void RunTests()
{
    DefaultConstructor();
    InsertTest();
    RemoveTail();
    RemoveHead();
    IteratorTest();
    RandomElement();

}
