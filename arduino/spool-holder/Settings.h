#pragma once
#include "Arduino.h"
#include <EEPROM.h>


class Settings
{
  public:
    Settings();
    void begin();
    void commit();
    
    char authKey[65];
    char accessToken[65];
    char refreshToken[65];


  private:
    int readInt(int address);
    void writeInt(int address, int value);

    void readString(int address, int length, char * str);
    void writeString(int address, int length, char * str);
    
};
