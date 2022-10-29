#pragma once

#include <vector>

class Autons
{
    public:
        Autons();

        const char getDescription(int autonNum);
        int * getPositions(int autonNum);

        void autonOne();
        void autonTwo();
        void autonThree();
};