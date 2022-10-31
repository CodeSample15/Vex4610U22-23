#pragma once

#include <vector>
#include <string>

class AutonManager
{
    public:
        AutonManager();

        void addAuton(void(*auton)(), std::string name, std::string desc, bool one, bool two, bool three, bool four, bool five); //add an auton
        int numAutons();

        std::string getName(int autonNum);
        std::string getDescription(int autonNum);
        bool * getPositions(int autonNum);

        bool runAuton(int num);

    private:
        std::vector<std::string> names;
        std::vector<std::string> descriptions;
        std::vector<bool*> positions;
        std::vector<void(*)()> autons;
};