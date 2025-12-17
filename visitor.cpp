#include "visitor.h"
#include <iostream>
#include "knight.h"
#include "orc.h"
#include "bear.h"

BattleVisitor::BattleVisitor(double r, std::vector<std::unique_ptr<NPC>>& n, std::vector<DeathObserver*>& obs) : range(r), npcs(n), observers(obs) {}

void BattleVisitor::visit(NPC& npc) {
    if (!npc.isAlive()) return;
    
    std::vector<NPC*> targets;
    for (auto& target : npcs) {
        if (target.get() != &npc && target->isAlive()) {
            targets.push_back(target.get());
        }
    }

    for (auto target : targets) {
        if (!npc.isAlive()) break;
        if (!target->isAlive()) continue;
        
        if (npc.distanceTo(*target) <= range) {
            if (npc.canDefeat(*target)) {
                target->die();
                for (auto obs : observers) {
                    obs->onDeath(npc.getName() + " (" + npc.getType() + ")", target->getName() + " (" + target->getType() + ")");
                }
                continue;
            } 
            
            if (target->canDefeat(npc)) {
                npc.die();
                for (auto obs : observers) {
                    obs->onDeath(target->getName() + " (" + target->getType() + ")", npc.getName() + " (" + npc.getType() + ")");
                }
                break;
            }
            
        }
    }
}