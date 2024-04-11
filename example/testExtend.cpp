
#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class Animal: virtual public ttx::Storable {
    TTX_DECLARE(Animal);
public:
    Animal(int hp, int speed): hp{hp}, speed{speed} {}
    void checkIsSameAs(ttx::Storable* o) {
        Animal* other = dynamic_cast<Animal*>(o);
        assert(other != nullptr);
        assert(other->hp == hp);
        assert(other->speed == speed);
    }
    int hp;
    int speed;
};
TTX_IMPL(Animal);
TTX_VARIABLE_PUB(Animal, hp);
TTX_VARIABLE_PUB(Animal, speed);

class Object: virtual public ttx::Storable {
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

class Monkey: public Animal, virtual public ttx::Storable {
    TTX_DECLARE(Monkey);
public:
    Monkey(int hp, int speed): Animal{hp, speed} { steal(); }
    void steal() {
        for (int i = 1; i <= 4; i++) {
            stolenObjects.push_back(new Object(i, rand()));
        }
    }
    void checkIsSameAs(ttx::Storable* o) {
        cout << "check in monkey\n";
        Monkey* other = dynamic_cast<Monkey*>(o);
        assert(other != nullptr);
        assert(other->stolenObjects.size() == stolenObjects.size());
        for (int i = 0; i < stolenObjects.size(); i++)
            stolenObjects[i]->checkIsSameAs(other->stolenObjects[i]);
        Animal::checkIsSameAs(o);
    }
    vector<Object*> stolenObjects;
};
TTX_IMPL_1(Monkey, Animal);
TTX_VARIABLE_PUB(Monkey, stolenObjects);


class Pigman: public Animal, virtual public ttx::Storable {
    TTX_DECLARE(Pigman);
public:
    Pigman(int hp, int speed, bool isFriendly): Animal{hp, speed}, isFriendly{isFriendly} {}
    void checkIsSameAs(ttx::Storable* o) {
        cout << "check in pigman\n";
        Pigman* other = dynamic_cast<Pigman*>(o);
        assert(other != nullptr);
        assert(other->isFriendly == isFriendly);
        Animal::checkIsSameAs(o);
    }
    bool isFriendly;
};
TTX_IMPL_1(Pigman, Animal);
TTX_VARIABLE_PUB(Pigman, isFriendly);

int main() {
    vector<ttx::Storable*> testdata = {
        new Pigman(101, 51, true),
        new Pigman(102, 52, false),
        new Monkey(49, 99),
        new Monkey(48, 98)
    };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");

    cout << "hello\n";

    assert(result.size() == testdata.size());
    cout << "world\n";
    for (int i = 0; i < result.size(); i++) {
        Pigman* pigman = dynamic_cast<Pigman*>(result[i]);
        Monkey* monkey = dynamic_cast<Monkey*>(result[i]);
        if (pigman) pigman->checkIsSameAs(testdata[i]);
        if (monkey) monkey->checkIsSameAs(testdata[i]);
        assert(pigman || monkey);
    }

    cout << "test extend passed\n";

    return 0;
}