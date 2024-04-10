
#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class Animal: virtual public ttx::Storable {
    TTX_DECLARE(Animal);
public:
    Animal(int hp, int speed): hp{hp}, speed{speed} {}
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
    vector<Object*> stolenObjects;
};
TTX_IMPL_1(Monkey, Animal);
TTX_VARIABLE_PUB(Monkey, stolenObjects);


class Pigman: public Animal, virtual public ttx::Storable {
    TTX_DECLARE(Pigman);
public:
    Pigman(int hp, int speed, bool isFriendly): Animal{hp, speed}, isFriendly{isFriendly} {}
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

    return 0;
}