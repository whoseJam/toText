
#include "adventurerAndWeapon.h"

using namespace std;

STORABLE_IMPL(Weapon);
STORE_PUBLIC(Weapon, name);
STORE_PUBLIC(Weapon, attack);
STORE_PUBLIC(Weapon, prop);
STORE_PRIVATE(Weapon, owner);
STORE(Weapon);

STORABLE_IMPL(Adventurer);
STORE_PUBLIC(Adventurer, name);
STORE_PUBLIC(Adventurer, health);
STORE_PUBLIC(Adventurer, weapons);
STORE(Adventurer);

void testAdventurerAndWeapon() {
    Adventurer* adv = Store::filter<Adventurer>(
        Store::load("example/adventurerAndWeapon.txt"));
    
    vector<Storable*> obj;
    obj.push_back(adv);
    Store::store("example/output.txt", obj);
}