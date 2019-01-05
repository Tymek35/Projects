#ifndef REBEL_FLEET_H
#define REBEL_FLEET_H

#include <type_traits>
#include <cassert>

template<typename U, int speedMin, int speedMax, bool attacking>
class RebelStarship {
private:
    U shield;
    U attackPower;
    U speed;
public:
    using valueType = U;

    //Constructor for ships which can attack.
    template<bool A = true, typename = typename std::enable_if<attacking == A>::type>
    RebelStarship(U s, U sp, U ap) {
        assert(sp <= speedMax);
        assert(sp >= speedMin);
        speed = sp;
        shield = s;
        attackPower = ap;
    }

    //Constructor for ships which can not attack.
    template<bool A = false, typename = typename std::enable_if<attacking == A>::type>
    RebelStarship(U s, U sp) {
        assert(sp <= speedMax);
        assert(sp >= speedMin);
        speed = sp;
        shield = s;
    }

    //Returns shield of the ship.
    U getShield() const {
        return shield;
    }

    //Returns speed of the ship.
    U getSpeed() const {
        return speed;
    }

    //Retruns attackPower of the ship if this ship is able to attack.
    template<bool A = true, typename = typename std::enable_if<attacking == A>::type>
    U getAttackPower() const {
        return attackPower;
    }

    //Decreases shield of the ship by given damage or
    //sets it to 0 if damage > shield.
    void takeDamage(U damage) {
        if(damage > shield) shield = 0;
        else shield -= damage;
    }

};

//Definitions o rebel starships.
template<typename U> using XWing = RebelStarship<U, 299796, 2997960, true>;
template<typename U> using StarCruiser = RebelStarship<U, 99999, 299795, true>;
template<typename U> using Explorer = RebelStarship<U, 299796, 2997960, false>;

#endif /* REBEL_FLEET_H */
