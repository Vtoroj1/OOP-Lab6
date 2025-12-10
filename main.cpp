#include <iostream>
#include "dungeon.h"
#include "observer.h"
#include "factory.h"

int main() {
    Dungeon editor;

    ConsoleObserver consoleObs;
    FileObserver fileObs("log.txt");
    editor.addObserver(&consoleObs);
    editor.addObserver(&fileObs);


    editor.addNPC(NPCFactory::createNPC("Knight", "Сэр_Ланселот", 100, 100));
    editor.addNPC(NPCFactory::createNPC("Knight", "Рыцарь_Дракона", 200, 200));

    editor.addNPC(NPCFactory::createNPC("Orc", "Громозуб", 150, 150));
    editor.addNPC(NPCFactory::createNPC("Orc", "Кровавый_Топор", 250, 250));

    editor.addNPC(NPCFactory::createNPC("Bear", "Бурый_Гигант", 120, 80));
    editor.addNPC(NPCFactory::createNPC("Bear", "Северный_Воевода", 220, 180));

    std::cout << "=== Редактор подземелья Balagur Fate 3 ===" << std::endl;
    std::cout << "Логика боя: Рыцарь > Орк > Медведь > Рыцарь" << std::endl;
    std::cout << "\nДо битвы:\n";
    editor.printNPCs();



    std::cout << "\n=== Начинается битва! (дальность: 150м) ===" << std::endl;
    editor.battle(150.0);

    std::cout << "\nПосле битвы:\n";
    editor.printNPCs();

    editor.saveToFile("dungeon.txt");
    
    std::ofstream testFile("test_dungeon.txt");
    testFile << "Knight Артур 50 50\n";
    testFile << "Orc Зубодробитель 150 150\n";
    testFile << "Bear Лесной_Царь 250 250\n";
    testFile.close();
    
    std::cout << "\n=== Загрузка из файла test_dungeon.txt ===" << std::endl;
    Dungeon editor2;
    editor2.addObserver(&consoleObs);
    editor2.loadFromFile("test_dungeon.txt");
    editor2.printNPCs();

    return 0;
}