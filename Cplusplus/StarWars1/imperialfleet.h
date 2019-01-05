#ifndef IMPERIAL_FLEET_H
#define IMPERIAL_FLEET_H

#include "rebelfleet.h"
#include <type_traits>

template<typename U>
class ImperialStarship {
private:
    U shield;
    U attackPower;
public:
    using valueType = U;

    //Constructor of imperial starship.
    ImperialStarship(U s, U ap) {
        shield = s;
        attackPower = ap;
    }

    //Returns speed of the ship.
    U getShield() const {
        return shield;
    }

    //Retruns attackPower of the ship.
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

//Imperial starships' definitions.
template<typename U> using DeathStar = ImperialStarship<U>;
template<typename U> using ImperialDestroyer = ImperialStarship<U>;
template<typename U> using TIEFighter = ImperialStarship<U>;

template<typename I>
struct enableIfImperial {};

template<typename U>
struct enableIfImperial<ImperialStarship<U>> {
    using type = int;
};

template<typename R>
struct enableIfNotAttackingRebel {};

template<typename U, int speedMin, int speedMax>
struct enableIfNotAttackingRebel<RebelStarship<U, speedMin, speedMax, false>> {
    using type = int;
};

template<typename R>
struct enableIfAttackingRebel {};

template<typename U, int speedMin, int speedMax>
struct enableIfAttackingRebel<RebelStarship<U, speedMin, speedMax, true>> {
    using type = int;
};

//In this function imperial ship attaks rebel ship what means: decreasing
//rebel ship's shield by imperial ship's attack power and if rebel ship is
//able to attack then it does same to imperial ship. If any of those ships
//has shield equal to 0 it doesn't attack.
template<typename I,
         typename R,
         typename enableIfImperial<I>::type = 0,
         typename enableIfNotAttackingRebel<R>::type = 0>
void attack(I &imperialShip, R &rebelShip){
    if(imperialShip.getShield() != 0)
        rebelShip.takeDamage(static_cast<typename R::valueType>(imperialShip.getAttackPower()));
}

template<typename I,
         typename R,
         typename enableIfImperial<I>::type = 0,
         typename enableIfAttackingRebel<R>::type = 0>
void attack(I &imperialShip, R &rebelShip){
    if(imperialShip.getShield() != 0 && rebelShip.getShield() != 0){
        rebelShip.takeDamage(static_cast<typename R::valueType>(imperialShip.getAttackPower()));
        imperialShip.takeDamage(static_cast<typename I::valueType>(rebelShip.getAttackPower()));
    }
}

#endif /* IMPERIAL_FLEET_H */
