#pragma once

STORE_EXPORT_HELPER(Container, vec);
STORE_EXPORT_HELPER(Container, list);
STORE_EXPORT_HELPER(Container, deque);
struct Container : public Storable {
public:
    STORABLE_DECLARE;
    STORE_EXPORT(Container, vec);
    STORE_EXPORT(Container, list);
    STORE_EXPORT(Container, deque);
public:
    enum class Type {Short, Long, VeryLong};
    Type type;
    std::string name;
    void pushVec(int x) { vec.push_back(x); }
    void pushList(int x) { list.push_back(x); }
    void pushDeque(int x) { deque.push_back(x); }
private:
    std::vector<int> vec;
    std::vector<int> list;
    std::deque<int> deque;
};

void testContainer();