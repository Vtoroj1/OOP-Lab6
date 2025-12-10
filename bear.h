#pragma once

#include "npc.h"
#include "visitor.h"

class Bear : public NPC {
    bool alive;
    
public:
    Bear(const std::string& n, double xPos, double yPos);
    
    std::string getType() const override;
    bool canDefeat(const NPC& other) const override;
    bool isAlive() const override;
    void die();
    
    void accept(BattleVisitor& visitor) override;
};
