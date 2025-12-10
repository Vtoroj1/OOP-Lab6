// tests.cpp
#include <gtest/gtest.h>
#include "npc.h"
#include "knight.h"
#include "orc.h"
#include "bear.h"
#include "factory.h"
#include "visitor.h"
#include "observer.h"
#include "dungeon.h"
#include <sstream>
#include <fstream>
#include <memory>

using namespace std;

// ==================== Тесты NPC ====================

TEST(NPCTest, ConstructorAndGetters) {
    Knight knight("Артур", 100, 200);
    EXPECT_EQ(knight.getName(), "Артур");
    EXPECT_EQ(knight.getX(), 100);
    EXPECT_EQ(knight.getY(), 200);
    EXPECT_TRUE(knight.isAlive());
    EXPECT_EQ(knight.getType(), "Knight");
}

TEST(NPCTest, DistanceCalculation) {
    Knight knight("Артур", 0, 0);
    Orc orc("Гром", 3, 4);
    EXPECT_DOUBLE_EQ(knight.distanceTo(orc), 5.0);
}

TEST(NPCTest, DieMethod) {
    Bear bear("Бурый", 100, 100);
    EXPECT_TRUE(bear.isAlive());
    bear.die();
    EXPECT_FALSE(bear.isAlive());
}

// ==================== Тесты логики боя ====================

TEST(CombatTest, KnightBeatsOrc) {
    Knight knight("Рыцарь", 0, 0);
    Orc orc("Орк", 1, 1);
    EXPECT_TRUE(knight.canDefeat(orc));
    EXPECT_FALSE(orc.canDefeat(knight));
}

TEST(CombatTest, OrcBeatsBear) {
    Orc orc("Орк", 0, 0);
    Bear bear("Медведь", 1, 1);
    EXPECT_TRUE(orc.canDefeat(bear));
    EXPECT_FALSE(bear.canDefeat(orc));
}

TEST(CombatTest, BearBeatsKnight) {
    Bear bear("Медведь", 0, 0);
    Knight knight("Рыцарь", 1, 1);
    EXPECT_TRUE(bear.canDefeat(knight));
    EXPECT_FALSE(knight.canDefeat(bear));
}

TEST(CombatTest, SameTypeNoKill) {
    Knight k1("Артур", 0, 0);
    Knight k2("Ланселот", 1, 1);
    EXPECT_FALSE(k1.canDefeat(k2));
    EXPECT_FALSE(k2.canDefeat(k1));
}

// ==================== Тесты Factory ====================

TEST(FactoryTest, CreateNPC) {
    auto knight = NPCFactory::createNPC("Knight", "Артур", 100, 100);
    auto orc = NPCFactory::createNPC("Orc", "Гром", 150, 150);
    auto bear = NPCFactory::createNPC("Bear", "Бурый", 120, 80);
    
    ASSERT_NE(knight, nullptr);
    ASSERT_NE(orc, nullptr);
    ASSERT_NE(bear, nullptr);
    
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(orc->getType(), "Orc");
    EXPECT_EQ(bear->getType(), "Bear");
}

TEST(FactoryTest, InvalidType) {
    auto invalid = NPCFactory::createNPC("Dragon", "Смауг", 300, 300);
    EXPECT_EQ(invalid, nullptr);
}

TEST(FactoryTest, LoadFromString) {
    string data = "Knight Артур 100 200";
    auto npc = NPCFactory::loadFromString(data);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Knight");
    EXPECT_EQ(npc->getName(), "Артур");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

// ==================== Тесты Observer ====================

class MockObserver : public DeathObserver {
public:
    string lastKiller;
    string lastVictim;
    int count = 0;
    
    void onDeath(const string& killer, const string& victim) override {
        lastKiller = killer;
        lastVictim = victim;
        count++;
    }
};

TEST(ObserverTest, MockObserverWorks) {
    MockObserver observer;
    observer.onDeath("Артур", "Гром");
    EXPECT_EQ(observer.lastKiller, "Артур");
    EXPECT_EQ(observer.lastVictim, "Гром");
    EXPECT_EQ(observer.count, 1);
}

TEST(FileObserverTest, FileCreation) {
    const char* filename = "test_log.txt";
    remove(filename);
    
    {
        FileObserver observer(filename);
        observer.onDeath("Артур", "Гром");
        observer.onDeath("Гром", "Бурый");
    }
    
    ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    EXPECT_GE(lines.size(), 2);
    remove(filename);
}

// ==================== Тесты Visitor ====================

TEST(VisitorTest, KnightKillsOrc) {
    vector<unique_ptr<NPC>> npcs;
    npcs.push_back(make_unique<Knight>("Артур", 0, 0));
    npcs.push_back(make_unique<Orc>("Гром", 5, 0));
    
    MockObserver observer;
    vector<DeathObserver*> observers = {&observer};
    
    BattleVisitor visitor(10.0, npcs, observers);
    npcs[0]->accept(visitor);
    
    EXPECT_EQ(observer.count, 1);
    EXPECT_FALSE(npcs[1]->isAlive());
    EXPECT_TRUE(npcs[0]->isAlive());
}

TEST(VisitorTest, NoFightWhenFar) {
    vector<unique_ptr<NPC>> npcs;
    npcs.push_back(make_unique<Knight>("Артур", 0, 0));
    npcs.push_back(make_unique<Orc>("Гром", 100, 100));
    
    MockObserver observer;
    vector<DeathObserver*> observers = {&observer};
    
    BattleVisitor visitor(50.0, npcs, observers);
    npcs[0]->accept(visitor);
    
    EXPECT_EQ(observer.count, 0);
    EXPECT_TRUE(npcs[0]->isAlive());
    EXPECT_TRUE(npcs[1]->isAlive());
}

// ==================== Тесты Dungeon ====================

class DungeonTest : public ::testing::Test {
protected:
    void SetUp() override {
        editor = make_unique<Dungeon>();
        mockObserver = make_unique<MockObserver>();
        editor->addObserver(mockObserver.get());
    }
    
    void TearDown() override {
        remove("test_save.txt");
        remove("test_load.txt");
    }
    
    unique_ptr<Dungeon> editor;
    unique_ptr<MockObserver> mockObserver;
};

TEST_F(DungeonTest, AddNPC) {
    editor->addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 100));
    editor->addNPC(NPCFactory::createNPC("Orc", "Гром", 150, 150));
    EXPECT_EQ(editor->getNPCCount(), 2);
}

TEST_F(DungeonTest, SaveAndLoad) {
    editor->addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 200));
    editor->addNPC(NPCFactory::createNPC("Orc", "Гром", 150, 250));
    
    editor->saveToFile("test_save.txt");
    
    Dungeon editor2;
    editor2.loadFromFile("test_save.txt");
    
    EXPECT_EQ(editor2.getNPCCount(), 2);
}

TEST_F(DungeonTest, BattleKnightVsOrc) {
    editor->addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 100));
    editor->addNPC(NPCFactory::createNPC("Orc", "Гром", 105, 105));
    
    editor->battle(10.0);
    
    EXPECT_EQ(mockObserver->count, 1);
    EXPECT_EQ(editor->getNPCCount(), 1);
}

TEST_F(DungeonTest, BattleSameType) {
    editor->addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 100));
    editor->addNPC(NPCFactory::createNPC("Knight", "Ланселот", 105, 105));
    
    editor->battle(10.0);
    
    EXPECT_EQ(mockObserver->count, 0);
    EXPECT_EQ(editor->getNPCCount(), 2);
}

TEST_F(DungeonTest, BattleChainReaction) {
    editor->addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 100));
    editor->addNPC(NPCFactory::createNPC("Orc", "Гром", 110, 110));
    editor->addNPC(NPCFactory::createNPC("Bear", "Бурый", 120, 120));
    
    editor->battle(50.0);
    
    EXPECT_GE(mockObserver->count, 1);
    EXPECT_LT(editor->getNPCCount(), 3);
}

// ==================== Интеграционный тест ====================

TEST(IntegrationTest, CompleteScenario) {
    Dungeon editor;
    MockObserver observer;
    editor.addObserver(&observer);
    
    editor.addNPC(NPCFactory::createNPC("Knight", "Артур", 100, 100));
    editor.addNPC(NPCFactory::createNPC("Orc", "Гром", 110, 110));
    editor.addNPC(NPCFactory::createNPC("Bear", "Бурый", 120, 120));
    
    editor.saveToFile("test.txt");
    
    editor.battle(30.0);
    
    EXPECT_GT(observer.count, 0);
    
    remove("test.txt");
}

