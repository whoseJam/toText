#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class Beauty : virtual public ttx::Storable {
    TTX_DECLARE(Beauty);
public:
    Beauty(int beautiness): beautiness{beautiness} {}
    int beautiness;
};
TTX_IMPL(Beauty);
TTX_VARIABLE_PUB(Beauty, beautiness);

class Fish : virtual public ttx::Storable {
    TTX_DECLARE(Fish);
public:
    Fish(int taste): taste{taste} {}
    int taste;
};
TTX_IMPL(Fish);
TTX_VARIABLE_PUB(Fish, taste);

class FishBeauty: public Beauty, public Fish, virtual public ttx::Storable {
    TTX_DECLARE(FishBeauty);
public:
    FishBeauty(int beautiness, int taste, int money):
        Beauty{beautiness}, Fish{taste}, money{money} {}
    int money;
};
TTX_IMPL_2(FishBeauty, Beauty, Fish);
TTX_VARIABLE_PUB(FishBeauty, money);

int main() {
    vector<ttx::Storable*> testdata = {
        new Beauty(5),
        new Fish(10),
        new FishBeauty(15, 20, +1000)
    };
    ttx::store(cout, testdata);
    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    
    {   auto* a = dynamic_cast<Beauty*>(result[0]);
        auto* b = dynamic_cast<Beauty*>(testdata[0]);
        assert(a != nullptr);
        assert(b != nullptr);
        assert(a->beautiness == b->beautiness);
    }
    {   auto* a = dynamic_cast<Fish*>(result[1]);
        auto* b = dynamic_cast<Fish*>(testdata[1]);
        assert(a != nullptr);
        assert(b != nullptr);
        assert(a->taste == b->taste);
    }
    {   auto* a = dynamic_cast<FishBeauty*>(result[2]);
        auto* b = dynamic_cast<FishBeauty*>(testdata[2]);
        assert(a != nullptr);
        assert(b != nullptr);
        assert(a->beautiness == b->beautiness);
        assert(a->taste == b->taste);
        assert(a->money == b->money);
    }
    cout << "test multi extend passed\n";
    return 0;
}