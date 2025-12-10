#pragma once

#include <string>
#include <fstream>

class DeathObserver {
public:
    virtual void onDeath(const std::string& killer, const std::string& victim) = 0;
    virtual ~DeathObserver() = default;
};

class ConsoleObserver : public DeathObserver {
public:
    void onDeath(const std::string& killer, const std::string& victim) override;
};

class FileObserver : public DeathObserver {
    std::ofstream logFile;
    
public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    
    void onDeath(const std::string& killer, const std::string& victim) override;
};

