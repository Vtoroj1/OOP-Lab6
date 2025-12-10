#pragma once
#include <string>

class BattleVisitor;

class NPC {
protected:
    std::string name;
    double x, y;
    
public:
    NPC(const std::string& n, double xPos, double yPos);
    virtual ~NPC() = default;

    virtual std::string getType() const = 0;
    std::string getName() const;
    double getX() const;
    double getY() const;
    
    double distanceTo(const NPC& other) const;
    
    virtual bool canDefeat(const NPC& other) const = 0;
    virtual bool isAlive() const = 0;
    virtual void die() = 0;
    
    virtual void accept(BattleVisitor& visitor) = 0;
};

