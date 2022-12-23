
#include "Store.h"
#include <string>
using namespace std;

class Adventurer;

class Weapon : public Storable {
public:
    string name;
    int attack;
    int* prop;
    void setOwner(Adventurer* adv) { owner = adv; }
    Adventurer* getOwner() const { return owner; }
    STORABLE_DECLARE;
private:
    Adventurer* owner;
};
STORABLE_IMPL(Weapon);
STORE_PUBLIC(Weapon, name);
STORE_PUBLIC(Weapon, attack);
STORE_PUBLIC(Weapon, prop);
STORE(Weapon);


class Adventurer : public Storable {
public:
    string name;
    int health;
    
    Weapon* weapon;
    vector<Weapon*> weapons;
    STORABLE_DECLARE_AND_IMPL(Adventurer);
};
STORE_PUBLIC(Adventurer, name);
STORE_PUBLIC(Adventurer, health);
STORE_PUBLIC(Adventurer, weapon);
STORE_PRIVATE(Weapon, owner, getOwner, setOwner);
STORE_PUBLIC(Adventurer, weapons);
STORE(Adventurer);

class Pig : virtual public Storable {
public:
    int health;
    int food;
    STORABLE_DECLARE_AND_IMPL(Pig);
};
STORE_PUBLIC(Pig, health);
STORE_PUBLIC(Pig, food);
STORE(Pig);

class People : virtual public Storable {
public:
    string name;
    int age;
    STORABLE_DECLARE_AND_IMPL(People);
};
STORE_PUBLIC(People, name);
STORE_PUBLIC(People, age);
STORE(People);

class Student : public People, public Pig {
public:
    int grade;
    float getScore() { return score; }
    void setScore(float v) {score = v;}
    STORABLE_DECLARE_AND_IMPL_2(Student, People, Pig);
private:
    float score;
};
STORE_PUBLIC(Student, grade);
STORE_PRIVATE(Student, score, getScore, setScore);
STORE(Student);

class ArrayList : public Storable {
public:
    vector<int> data;
    deque<int> que;
    STORABLE_DECLARE_AND_IMPL(ArrayList);
};
STORE_PUBLIC(ArrayList, data);
STORE_PUBLIC(ArrayList, que);
STORE(ArrayList);

void test1() {
    ifstream input;
    input.open("example/weapon.txt");
    Weapon* test = Store::loadAs<Weapon>(input);
    cout << test->prop << " prop ptr\n";
    cout << test->attack << " " << test->name << " " << (*((int*)test->prop)) << ' ' << "\n";
    vector<Storable*> objects;
    objects.push_back(test);
    Store::store("example/output.txt", objects);
}

void test2() {
    ifstream input;
    input.open("example/studentAndPig.txt");
    Student* tst1 = Store::loadAs<Student>(input);
    Pig* tst2 = Store::loadAs<Pig>(input);
    vector<Storable*> objects;
    objects.push_back(tst2);
    objects.push_back(tst1);
    Store::store("example/output.txt", objects);
}

void test3() {
    ifstream input;
    input.open("example/adventurerAndWeapon.txt");
    Adventurer* tst = Store::loadAs<Adventurer>(input);
    Weapon* wp = Store::loadAs<Weapon>(input);
    Store::loadAs<Weapon>(input);
    Store::loadAs<Weapon>(input);
    Store::loadAs<Weapon>(input);
    Store::afterDecode();

    cout << "attack=" << tst->weapon->attack << "\n";
    cout << "name=" << tst->weapon->name << "\n";
    cout << "prop=" << *(tst->weapon->prop) << "\n";
    cout << "owner=" << wp->getOwner() << "\n";
    vector<Storable*> objects;
    objects.push_back(tst);
    Store::store("example/output.txt", objects);
}

void test4() {
    ifstream input;
    input.open("example/arrayList.txt");
    ArrayList* list1 = Store::loadAs<ArrayList>(input);
    vector<Storable*> objects;
    objects.push_back(list1);
    Store::store("example/output.txt", objects);
}

int main() {
    //Store::getter_trait<&Weapon::getOwner>();
    decltype(Store::getter_trait(&Weapon::getOwner)) t;
    test2();
    return 0;
}