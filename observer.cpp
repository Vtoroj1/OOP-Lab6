#include "observer.h"
#include <iostream>

void ConsoleObserver::onDeath(const std::string& killer, const std::string& victim) {
    std::cout << "[БОЙ] " << killer << " убил " << victim << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    logFile.open(filename, std::ios::app);
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileObserver::onDeath(const std::string& killer, const std::string& victim) {
    if (logFile.is_open()) {
        logFile << "[БОЙ] " << killer << " убил " << victim << std::endl;
    }
}