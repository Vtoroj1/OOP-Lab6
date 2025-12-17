#include "npc.h"
#include <cmath>

NPC::NPC(const std::string& n, double xPos, double yPos) : name(n), x(xPos), y(yPos) {}

std::string NPC::getName() const {
    return name;
}

double NPC::getX() const {
    return x;
}

double NPC::getY() const {
    return y;
}

double NPC::distanceTo(const NPC& other) const {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}