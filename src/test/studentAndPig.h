#pragma once

#include "../Store.h"

class Pig : virtual public Storable {
public:
    STORABLE_DECLARE;
public:
    int health;
    int food;
};

class People : virtual public Storable {
public:
    STORABLE_DECLARE;
public:
    std::string name;
    int age;
};

STORE_EXPORT_HELPER(Student, score);
class Student : public People, public Pig {
public:
    STORABLE_DECLARE;
    STORE_EXPORT(Student, score);
public:
    int grade;
    float getScore() { return score; }
    void setScore(float v) { score = v; }
private:
    float score;
};

void testStudentAndPig();