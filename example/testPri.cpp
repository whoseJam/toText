
#include <iostream>
#include <cassert>

#include "ToText.h"

using namespace std;

class Student : public ttx::Storable {
public:
    TTX_DECLARE(Student);
public:
    Student(string name, int age, int score, int password) :
        name(name), age(age), score(score), password(password) {
    }
    void checkIsSameAs(const Student& other) {
        assert(name == other.name);
        assert(age == other.age);
        assert(score == other.score);
        assert(password == other.password);
    }
    string name;
    int age;

    TTX_EXPORT(Student, score);
    TTX_EXPORT(Student, password);
private:
    int score;
    int password;
};
TTX_IMPL(Student);
TTX_VARIABLE_PUB(Student, name);
TTX_VARIABLE_PUB(Student, age);
TTX_VARIABLE_PRI(Student, score);
TTX_VARIABLE_PRI(Student, password);


int main() {
    vector<ttx::Storable*> students;
    students.push_back(new Student("Jack", 16, 98, 123321));
    students.push_back(new Student("Mark", 18, 100, 789987));
    students.push_back(new Student("Timi", 20, 78, 456654));
    
    ttx::store(cout, students);

    ttx::store("./temp.txt", students);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    assert(result.size() == students.size());
    for (int i = 0; i < result.size(); i++) {
        Student* a = dynamic_cast<Student*>(result[i]);
        Student* b = dynamic_cast<Student*>(students[i]);
        a->checkIsSameAs(*b);
    }

    cout << "test private passed\n";

    return 0;
}