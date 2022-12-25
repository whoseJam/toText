
#include "studentAndPig.h"

using namespace std;

STORABLE_IMPL(Pig);
STORE_PUBLIC(Pig, health);
STORE_PUBLIC(Pig, food);
STORE(Pig);

STORABLE_IMPL(People);
STORE_PUBLIC(People, name);
STORE_PUBLIC(People, age);
STORE(People);

STORABLE_IMPL_2(Student, People, Pig);
STORE_PUBLIC(Student, grade);
STORE_PRIVATE(Student, score);
STORE(Student);

void testStudentAndPig() {
    auto stream = Store::load("example/studentAndPig.txt");
    Student* stu = Store::filter<Student>(stream);
    Pig* pig = Store::filter<Pig>(stream);
    
    vector<Storable*> objects;
    objects.push_back(stu);
    objects.push_back(pig);
    Store::store("example/output.txt", stream);
}