#pragma once

#include <vector>
#include <memory>
#include "npc.h"
#include "observer.h"

class NPC;

class BattleVisitor {
    double range;
    std::vector<std::unique_ptr<NPC>>& npcs;
    std::vector<DeathObserver*>& observers;
    
public:
    BattleVisitor(double r, 
                  std::vector<std::unique_ptr<NPC>>& n, 
                  std::vector<DeathObserver*>& obs);
    
    void visit(NPC& npc);
};
