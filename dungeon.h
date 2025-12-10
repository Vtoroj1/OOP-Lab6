#pragma once

#include <vector>
#include <memory>
#include <string>
#include "npc.h"
#include "observer.h"

class Dungeon {
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<DeathObserver*> observers;
    
public:
    void addNPC(std::unique_ptr<NPC> npc);
    void printNPCs() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void battle(double range);
    void addObserver(DeathObserver* obs);
    
    size_t getNPCCount() const { return npcs.size(); }
};

