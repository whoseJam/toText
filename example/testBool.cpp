#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class Pigman: public ttx::Storable {
    TTX_DECLARE(Pigman);
public:
    Pigman(int hp, bool isFriendly):
        hp{hp}, isFriendly{isFriendly} {}
    void checkIsSameAs(const Pigman& other) {
        assert(hp == other.hp);
        assert(isFriendly == other.isFriendly);
    }
    int hp;
    bool isFriendly;
};
TTX_IMPL(Pigman);
TTX_VARIABLE_PUB(Pigman, hp);
TTX_VARIABLE_PUB(Pigman, isFriendly);

Pigman* pig(int hp, bool isFriendly) {
    return new Pigman(hp, isFriendly);
}
int main() {
    vector<ttx::Storable*> testdata = {
        pig(50, false),
        pig(75, true),
        pig(45, true)
    };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    assert(result.size() == testdata.size());
    for (int i = 0; i < result.size(); i++) {
        Pigman* a = dynamic_cast<Pigman*>(result[i]);
        Pigman* b = dynamic_cast<Pigman*>(testdata[i]);
        a->checkIsSameAs(*b);
    }

    cout << "test bool passed\n";

    return 0;
}