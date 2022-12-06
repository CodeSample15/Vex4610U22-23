#pragma once
#include "main.h"
#include "methods.h"
#include "points.h"
#include <string>

class Replay
{
    public:
        Replay(); //default settings
        Replay(int delay);
        Replay(std::string folderName);
        Replay(std::string folderName, int delay);
        ~Replay();

        void record();
        void save();
    private:
        std::string fileName;
        int delay; //delay in milliseconds between each log
        bool recording;

        FILE* usd_write_file;
};