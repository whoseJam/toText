#pragma once

#include "../Store.h"

class Pig : virtual public Storable {
public:
    STORABLE_DECLARE(Pig);
public:
    Pig() = delete;
    int health;
    int food;
};

class People : virtual public Storable {
public:
    STORABLE_DECLARE(People);
public:
    People() = delete;
    std::string name;
    int age;
};

STORE_EXPORT_HELPER(Student, score);
class Student : public People, public Pig {
public:
    STORABLE_DECLARE(Student);
    STORE_EXPORT(Student, score);
public:
    Student() = delete;
    int grade;
    float getScore() { return score; }
    void setScore(float v) { score = v; }
private:
    float score;
};

void testStudentAndPig();