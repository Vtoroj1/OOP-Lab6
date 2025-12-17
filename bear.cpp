#include "bear.h"
#include "knight.h"
#include "orc.h"

Bear::Bear(const std::string& n, double xPos, double yPos) : NPC(n, xPos, yPos), alive(true) {}

std::string Bear::getType() const {
    return "Bear";
}

bool Bear::canDefeat(const NPC& other) const {
    return other.getType() == "Knight";
}

bool Bear::isAlive() const {
    return alive;
}

void Bear::die() {
    alive = false;
}

void Bear::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}