#include "AutonManager.h"
#include <vector>
#include <string>

AutonManager::AutonManager()
{
    //not really necessary, but I'm doing it anyway
    AutonManager::names.clear();
    AutonManager::descriptions.clear();
    AutonManager::positions.clear();
    AutonManager::autons.clear();
}

void AutonManager::addAuton(void(*auton)(), std::string name, std::string desc, bool one, bool two, bool three, bool four, bool five)
{
    bool pos[5] = {one, two, three, four, five};

    AutonManager::names.push_back(name);
    AutonManager::descriptions.push_back(desc);
    AutonManager::positions.push_back(pos);
    AutonManager::autons.push_back(auton);
}

int AutonManager::numAutons()
{
    return descriptions.size();
}

std::string AutonManager::getName(int autonNum)
{
    return AutonManager::names.at(autonNum);
}

std::string AutonManager::getDescription(int autonNum)
{
    return AutonManager::descriptions.at(autonNum);
}

bool * AutonManager::getPositions(int autonNum)
{
    return AutonManager::positions.at(autonNum);
}

bool AutonManager::runAuton(int num)
{
    AutonManager::autons.at(num)();

    return true;
}