#include "knight.h"
#include "orc.h"
#include "bear.h"

Knight::Knight(const std::string& n, double xPos, double yPos) 
    : NPC(n, xPos, yPos), alive(true) {}

std::string Knight::getType() const {
    return "Knight";
}

bool Knight::canDefeat(const NPC& other) const {
    return other.getType() == "Orc";
}

bool Knight::isAlive() const {
    return alive;
}

void Knight::die() {
    alive = false;
}

void Knight::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}