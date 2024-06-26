/*********************************************************************
 * gb_SEN0169V2_ph_cpp
 *
 * Copyright (C)    20124   [GrowBot Open Source],
 * GitHub Link :XXXXX
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Description:Implement DFRobot Analog Industrial pH Sensor / Meter Pro Kit V2 into the growbot system
 *
 * Product Links：
 *
 * Sensor driver pin：
 * Connection:
 * VCC-5v
 * GND-GND

 *
 * author  :  GrowBot
 * version :  V1.0
 * date    :  2024-05-14
 **********************************************************************/

#include "gb_SEN0169V2_ph.h"

#include <Arduino.h>

//********************************************************************************************
// Function Name: gb_DS18B20_WaterproofTemperatureSensorKit()
// Function Declaration: create waterproof temperature sensor object
//********************************************************************************************
gb_SEN0169V2_ph::gb_SEN0169V2_ph(int DS18S20_Pin)
{
    this->DS18S20_Pin = DS18S20_Pin;
}

OneWire ds(DS18S20_Pin);

//********************************************************************************************
// Function Name: getWaterproofTemp()
// Function Declaration: get temperature and return value
//********************************************************************************************
double gb_SEN0169V2_ph::getWaterproofTemp()
{
    byte data[12];
    byte addr[8];

    if (!ds.search(addr))
    {
        ds.reset_search();
        return -1000;
    }

    if (OneWire::crc8(addr, 7) != addr[7])
    {
        Serial.println("Crc us bit valid!");
        return -1000;
    }

    if (addr[0] != 0x10 && addr[0] != 0x28)
    {
        Serial.println("Device is not recognized");
        return -1000;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);

    byte present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);

    for (int i = 0; i < 9; i++)
    {
        data[i] = ds.read();
    }

    ds.reset_search();

    byte MSB = data[1];
    byte LSB = data[0];

    float tempRead = ((MSB << 8) | LSB);
    float TemperatureSum = tempRead / 16;

    return String(TemperatureSum);
}