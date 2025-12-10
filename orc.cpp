#include "orc.h"
#include "knight.h"
#include "bear.h"

Orc::Orc(const std::string& n, double xPos, double yPos) 
    : NPC(n, xPos, yPos), alive(true) {}

std::string Orc::getType() const {
    return "Orc";
}

bool Orc::canDefeat(const NPC& other) const {
    return other.getType() == "Bear";
}

bool Orc::isAlive() const {
    return alive;
}

void Orc::die() {
    alive = false;
}

void Orc::accept(BattleVisitor& visitor) {
    visitor.visit(*this);
}