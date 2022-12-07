#include "Replay.h"
#include <iostream>

Replay::Replay() 
{
    Replay::fileName = "replay.txt";
    Replay::delay = 100;

    Replay::fileName = "/usd/" + Replay::fileName;
    Replay::usd_write_file = fopen(Replay::fileName.c_str(), "w");
    if(Replay::usd_write_file != 0) {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = true;
    }
    else {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = false;
    }
}

Replay::Replay(int delay) 
{
    Replay::fileName = "replay.txt";
    Replay::delay = delay;

    Replay::fileName = "/usd/" + Replay::fileName;
    Replay::usd_write_file = fopen(Replay::fileName.c_str(), "w");
    if(Replay::usd_write_file != 0) {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = true;
    }
    else {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = false;
    }
}

Replay::Replay(std::string fileName) 
{
    Replay::fileName = fileName;
    Replay::delay = 100;

    Replay::fileName = "/usd/" + Replay::fileName;
    Replay::usd_write_file = fopen(Replay::fileName.c_str(), "w");
    if(Replay::usd_write_file != 0) {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = true;
    }
    else {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = false;
    }
}

Replay::Replay(std::string fileName, int delay)
{
    Replay::fileName = fileName;
    Replay::delay = delay;

    Replay::fileName = "/usd/" + Replay::fileName;
    Replay::usd_write_file = fopen(Replay::fileName.c_str(), "w");
    if(Replay::usd_write_file != 0) {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = true;
    }
    else {
        fclose(Replay::usd_write_file);
        Replay::SDCardFound = false;
    }
}

Replay::~Replay()
{
    if(Replay::SDCardFound)
        fclose(Replay::usd_write_file);
}

void Replay::record()
{   
    if(Replay::SDCardFound) {
        Replay::usd_write_file = fopen(Replay::fileName.c_str(), "a");
        Points pos = getPositionXY();
        std::string x = std::to_string(pos.x);
        std::string y = std::to_string(pos.y);
        std::string rot = std::to_string(getRotation());

        std::string combined = x + "," + y + "," + rot + ",";

        fputs(combined.c_str(), Replay::usd_write_file);
        fclose(Replay::usd_write_file);
    }

    pros::delay(Replay::delay);
}