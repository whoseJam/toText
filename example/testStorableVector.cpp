
#include "ToText.h"

using namespace std;

class Object: public ttx::Storable {
    TTX_DECLARE(Object);
public:
    Object(int id, int count): id{id}, count{count} {}
    void checkIsSameAs(ttx::Storable* o) {
        Object* other = dynamic_cast<Object*>(o);
        assert(other != nullptr);
        assert(other->count == count);
        assert(other->id == id);
    }
    int count;
    int id;
};
TTX_IMPL(Object);
TTX_VARIABLE_PUB(Object, count);
TTX_VARIABLE_PUB(Object, id);

class Monkey: public ttx::Storable {
    TTX_DECLARE(Monkey);
public:
    Monkey(int len) { steal(len); }
    void steal(int len) {
        for (int i = 1; i <= len; i++) {
            stolenObjects.push_back(new Object(i, rand()));
        }
    }
    void checkIsSameAs(ttx::Storable* o) {
        Monkey* other = dynamic_cast<Monkey*>(o);
        assert(other != nullptr);
        assert(other->stolenObjects.size() == stolenObjects.size());
        for (int i = 0; i < stolenObjects.size(); i++)
            stolenObjects[i]->checkIsSameAs(other->stolenObjects[i]);
    }
    vector<Object*> stolenObjects;
};
TTX_IMPL(Monkey);
TTX_VARIABLE_PUB(Monkey, stolenObjects);

int main() {
    vector<ttx::Storable*> testdata = {
        new Monkey(3),
        new Monkey(5)
    };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");

    for (int i = 0; i < testdata.size(); i++) {
        Monkey* a = dynamic_cast<Monkey*>(result[i]);
        Monkey* b = dynamic_cast<Monkey*>(testdata[i]);
        a->checkIsSameAs(b);
    }

    cout << "test storable vector passed\n";

    return 0;
}