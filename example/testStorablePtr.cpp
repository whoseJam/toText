#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

class Weapon: public ttx::Storable {
    TTX_DECLARE(Weapon);
public:
    Weapon(int durability): durability{durability} {};
    int durability;
};
TTX_IMPL(Weapon);
TTX_VARIABLE_PUB(Weapon, durability);


class Player: public ttx::Storable {
    TTX_DECLARE(Player);
public:
    Player(Weapon* weapon, int hp): weapon{weapon}, hp{hp} {};
    void use(int delta) {
        weapon->durability -= delta;
    }
    Weapon* weapon;
    int hp;
};
TTX_IMPL(Player);
TTX_VARIABLE_PUB(Player, weapon);
TTX_VARIABLE_PUB(Player, hp);


int main() {
    Weapon* sharedWeapon = new Weapon(100);
    Player* player1 = new Player(sharedWeapon, 20);
    Player* player2 = new Player(sharedWeapon, 25);
    vector<ttx::Storable*> testdata = {
        player1, player2, sharedWeapon
    };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    assert(result.size() == testdata.size());
    {   Weapon* weapon = dynamic_cast<Weapon*>(result[2]);
        Player* player1 = dynamic_cast<Player*>(result[0]);
        Player* player2 = dynamic_cast<Player*>(result[1]);
        assert(weapon->durability == sharedWeapon->durability);
        player1->use(5);
        assert(weapon->durability == sharedWeapon->durability - 5);
        player2->use(3);
        assert(weapon->durability == sharedWeapon->durability - 8);
    }

    cout << "test storable ptr passed\n";

    return 0;
}