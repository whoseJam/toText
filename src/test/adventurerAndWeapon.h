#pragma once

#include "../Store.h"

class Adventurer;

STORE_EXPORT_HELPER(Weapon, owner);
class Weapon : public Storable {
public:
    STORABLE_DECLARE;
    STORE_EXPORT(Weapon, owner);
public:
    std::string name;
    int attack;
    int* prop;
    Adventurer* getOwner() { return owner; }
private:
    void setOwner(Adventurer* adv) { owner = adv; }
    Adventurer* owner;
};

class Adventurer : public Storable {
public:
    STORABLE_DECLARE;
public:
    std::string name;
    int health;
    std::vector<Weapon*> weapons;
};

void testAdventurerAndWeapon();
