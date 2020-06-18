#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_FRAM_SPI.h"

#define FRAM_CHIP_SELECT 5
#define SD_CHIP_SELECT 6

int pin_address_book[] = {5, 6, 16, 17, 18, 19};
int i2c_address_book[] = {0x1F, 0x70, 0x76, 0x21, 0x68, 0x40};

Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CHIP_SELECT);

uint8_t           addrSizeInBytes = 2; //Default to address size of two bytes
uint32_t          memSize;

int32_t readBack(uint32_t addr, int32_t data) {
    int32_t check = !data;
    int32_t wrapCheck, backup;

    fram.read(addr, (uint8_t*)&backup, sizeof(int32_t));
    fram.writeEnable(true);
    fram.write(addr, (uint8_t*)&data, sizeof(int32_t));
    fram.writeEnable(false);
    fram.read(addr, (uint8_t*)&check, sizeof(int32_t));
    fram.read(0, (uint8_t*)&wrapCheck, sizeof(int32_t));
    fram.writeEnable(true);
    fram.write(addr, (uint8_t*)&backup, sizeof(int32_t));
    fram.writeEnable(false);

    // Check for warparound, address 0 will work anyway
    if (wrapCheck==check)
        check = 0;

    return check;
}

bool testAddrSize(uint8_t addrSize) {
    fram.setAddressSize(addrSize);

    if (readBack(4, 0xbeefbead) == 0xbeefbead)
        return true;
    return false;
}

void setup()
{
    //Set pin modes
    for(int i = 0; i < sizeof(pin_address_book)/sizeof(int); i++)
    {
        pinMode(pin_address_book[i], OUTPUT);
    }

    //Start Wire
    Wire.begin();

    //Wait for serial
    while(!Serial);
    Serial.begin(115200);

    //Print welcome
    Serial.println("Icarus V1.0 Board Test Program");
    Serial.println("--------------------------------");
    Serial.println();
    delay(50);
}

void loop()
{
    int pass_count = 0;
    int fail_count = 0;

    //Check all I2C addresses
    Serial.println("TEST - I2C Address Scan");
    for(int i = 0; i < sizeof(i2c_address_book)/sizeof(int); i++)
    {
        Serial.print("Checking Address: 0x");
        Serial.print(i2c_address_book[i], HEX);
        Serial.print("...    ");

        Wire.beginTransmission(i2c_address_book[i]);
        int error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.println("PASS");
            pass_count++;
        }
        else
        {
            Serial.println("FAIL");
            fail_count++;
        }
    }
    Serial.println();

    //Toggle all output pins and check for function
    Serial.println("TEST - Output Pin Toggle");
    for(int i = 0; i < sizeof(pin_address_book)/sizeof(int); i++)
    {
        Serial.print("Checking Pin: ");
        Serial.print(pin_address_book[i], DEC);
        Serial.print("...    ");

        digitalWrite(pin_address_book[i], HIGH);
        delay(10);
        bool pin_result = digitalRead(pin_address_book[i]);
        digitalWrite(pin_address_book[i], LOW);

        if(pin_result)
        {
            Serial.println("PASS");
            pass_count++;
        }
        else
        {
            Serial.println("FAIL");
            fail_count++;
        }
    }
    Serial.println();

    //Check SD card slot
    Serial.println("TEST - SD Card");
    Serial.print("Initializing card...    ");
    if (SD.begin(SD_CHIP_SELECT)) 
    {
        Serial.println("PASS");
        pass_count++;

        File test_file = SD.open("test.txt", FILE_WRITE);

        Serial.print("Creating file...    ");
        if(test_file)
        {
            Serial.println("PASS");
            pass_count++;

            Serial.print("Deleting file...    ");
            test_file.close();
            if(SD.remove("test.txt"))
            {
                Serial.println("PASS");
                pass_count++;
            }
            else
            {
                Serial.println("FAIL");
                fail_count++;
            }
        }
        else
        {
            Serial.println("FAIL");
            fail_count++;
        }
    }
    else
    {
        Serial.println("FAIL");
        fail_count++;
    }
    Serial.println();

    //Check FRAM
    Serial.println("TEST - FRAM Chip");
    Serial.print("Initializing chip...    ");
    if (fram.begin()) 
    {
        Serial.println("PASS");
        pass_count++;

        Serial.print("Testing chip address size...    ");
        if (testAddrSize(2))
        {
            Serial.println("PASS");
            pass_count++;
            addrSizeInBytes = 2;
        }
        else if (testAddrSize(3))
        {
            Serial.println("PASS");
            pass_count++;
            addrSizeInBytes = 3;
        }
        else if (testAddrSize(4))
        {
            Serial.println("PASS");
            pass_count++;
            addrSizeInBytes = 4;
        }
        else
        {
            Serial.println("FAIL");
            fail_count++;
        }

        Serial.print("Testing chip capacity...    ");
        memSize = 0;

        while (readBack(memSize, memSize) == memSize) 
        {
            memSize += 256;
        }

        if((memSize/0x400) == 256)
        {
            Serial.println("PASS");
            pass_count++;
        }
        else
        {
            Serial.println("FAIL");
            fail_count++;
        }
    }
    else
    {
        Serial.println("FAIL");
        fail_count++;
    }
    Serial.println();

    //Print results
    Serial.println("Board Test Results");
    Serial.println("--------------------------------");
    Serial.print("Passed: ");
    Serial.println(pass_count);
    Serial.print("Failed: ");
    Serial.println(fail_count);

    while(1);
}