
#include "../Store.h"
#include "container.h"

using namespace std;

STORE(Container);
STORABLE_IMPL(Container);
STORE_PUBLIC(Container, name);
STORE_PUBLIC(Container, type);
STORE_PRIVATE(Container, vec);
STORE_PRIVATE(Container, list);
STORE_PRIVATE(Container, deque);

void testContainer() {
    Container* c = new Container();
    c->name = "test";
    c->type = Container::Type::VeryLong;
    for (int i = 1; i <= 10; i++) {
        c->pushList(i);
        c->pushVec(1 << i);
        c->pushDeque(i * 2);
    }

    vector<Storable*> obj;
    obj.push_back(c);
    Store::store("example/output.txt", obj);
}