
#include <Arduino.h>

bool inQueue;
bool inAttendance;
bool inInital;
bool inInative;

void initializeGlobalVar(){
    inQueue = false;
    inAttendance = false;
    inInital = false;
    inInative = true;
}