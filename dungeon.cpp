#include "dungeon.h"
#include "factory.h"
#include "visitor.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

void Dungeon::addNPC(std::unique_ptr<NPC> npc) {
    if (npc) {
        npcs.push_back(std::move(npc));
    }
}

void Dungeon::printNPCs() const {
    if (npcs.empty()) {
        std::cout << "В подземелье нет NPC." << std::endl;
        return;
    }
    
    int aliveCount = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) aliveCount++;
    }
    
    std::cout << "Список NPC (всего: " << npcs.size() << ", живых: " << aliveCount << ")" << std::endl;
    
    for (size_t i = 0; i < npcs.size(); i++) {
        const auto& npc = npcs[i];
        std::cout << i+1 << ". "
                  << npc->getType() << " "
                  << npc->getName() << " "
                  << "(" << npc->getX() 
                  << ", " << npc->getY() << ") "
                  << (npc->isAlive() ? "ЖИВ" : "МЁРТВ") << std::endl;
    }
}

void Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
        return;
    }
    
    int savedCount = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << npc->getType() << " " 
                 << npc->getName() << " "
                 << npc->getX() << " " 
                 << npc->getY() << std::endl;
            savedCount++;
        }
    }
    
    file.close();
    std::cout << "Сохранено " << savedCount << " NPC в файл: " << filename << std::endl;
}

void Dungeon::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return;
    }
    
    npcs.clear();
    
    std::string line;
    int loadedCount = 0;
    int lineNum = 0;
    
    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;
        
        auto npc = NPCFactory::loadFromString(line);
        if (npc) {
            npcs.push_back(std::move(npc));
            loadedCount++;
        } else {
            std::cerr << "Ошибка в строке " << lineNum << ": " << line << std::endl;
        }
    }
    
    file.close();
    std::cout << "Загружено " << loadedCount << " NPC из файла: " << filename << std::endl;
}

void Dungeon::battle(double range) {
    if (npcs.empty()) {
        std::cout << "Нет NPC для битвы!" << std::endl;
        return;
    }
    
    std::cout << "Начинается битва на дистанции " << range << " метров..." << std::endl;
    
    BattleVisitor visitor(range, npcs, observers);
    
    std::vector<NPC*> aliveNPCs;
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            aliveNPCs.push_back(npc.get());
        }
    }
    
    for (auto npcPtr : aliveNPCs) {
        if (npcPtr->isAlive()) {
            npcPtr->accept(visitor);
        }
    }
    
    size_t before = npcs.size();
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(), [](const std::unique_ptr<NPC>& n) { return !n->isAlive(); }), npcs.end());
    
    size_t killed = before - npcs.size();
    std::cout << "Битва окончена. Убито: " << killed << " NPC." << std::endl;
}

void Dungeon::addObserver(DeathObserver* obs) {
    observers.push_back(obs);
}