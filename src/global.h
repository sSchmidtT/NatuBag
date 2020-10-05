
#include <Arduino.h>

bool inQueue;
bool inAttendance;
bool inInital;

void initializeGlobalVar(){
    inQueue = false;
    inAttendance = false;
    inInital = true;
}