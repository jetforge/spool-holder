#include "Arduino.h"
#include "Settings.h"
#include <string.h>
#include <EEPROM.h>


Settings::Settings()
{

}

void Settings::begin()
{
  EEPROM.begin(512);
  
  int offset = 0;

  int control = readInt(offset);
  offset += 2;

  if (control != 1620)
  {
    Serial.println("invalid control");
    this->commit();
  }
  else
  {
    Serial.println("control ok");

    readString(offset, 64, authKey);
    offset += 64;

    readString(offset, 64, accessToken);
    offset += 64;

    readString(offset, 64, refreshToken);
    offset += 64;
  }

        
}



void Settings::commit()
{
  Serial.println("will commit");

  for (int i = 0; i < 256; i++)
  {
    EEPROM.write(i, 0);
  }

  int offset = 0;

  writeInt(0, 1620);
  offset += 2;

  writeString(offset, 64, authKey);
  offset += 64;

  writeString(offset, 64, accessToken);
  offset += 64;

  writeString(offset, 64, refreshToken);
  offset += 64;
  
  EEPROM.commit();
}


void Settings::writeString(int address, int length, char * str)
{
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, str[i]);
  }
}

void Settings::readString(int address, int length, char * str)
{
  memset(str, 0, sizeof(str));

  for (int i = 0; i < length; i++) {
    str[i] = EEPROM.read(address + i);
  }
}


void Settings::writeInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);
}

int Settings::readInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}
