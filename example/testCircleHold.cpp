#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class A;
class B;
class C;

class A: public ttx::Storable {
    TTX_DECLARE(A);
public:
    A(int value): value{value}, hold{nullptr} {}
    int value;
    B* hold;
};
class B: public ttx::Storable {
    TTX_DECLARE(B);
public:
    B(string name): name{name}, hold{nullptr} {}
    string name;
    C* hold;
};
class C: public ttx::Storable {
    TTX_DECLARE(C);
public:
    C(double rate): rate{rate}, hold{nullptr} {}
    double rate;
    A* hold;
};

TTX_IMPL(A);
TTX_VARIABLE_PUB(A, hold);
TTX_VARIABLE_PUB(A, value);
TTX_IMPL(B);
TTX_VARIABLE_PUB(B, hold);
TTX_VARIABLE_PUB(B, name);
TTX_IMPL(C);
TTX_VARIABLE_PUB(C, hold);
TTX_VARIABLE_PUB(C, rate);

int main() {
    A* a = new A(500);
    B* b = new B("hello world");
    C* c = new C(-0.5);
    a->hold = b;
    b->hold = c;
    c->hold = a;
    vector<ttx::Storable*> testdata = { a };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    A* na = ttx::filter<A>(result);
    B* nb = ttx::filter<B>(result);
    C* nc = ttx::filter<C>(result);
    assert(na->hold == nb);
    assert(na->value == a->value);
    assert(nb->hold == nc);
    assert(nb->name == b->name);
    assert(nc->hold == na);
    assert(nc->rate == c->rate);

    cout << "test circle hold passed\n";

    return 0;
}