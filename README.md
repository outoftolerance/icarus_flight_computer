# Icarus Flight Computer

## SERCOM
We make use of the SERCOM feature of the micro by definig two additional serial ports beyond those already defined by default, no other customised use of the SERCOM system is needed for this project.

*Note that when adding serial port interfaces using SERCOM, the TX pin may only reside on either PAD0 or PAD2 of a specific SERCOM interface! This limits the available SERCOM interfaces that we can choose from because some pads are being used for other interfaces (like I2C or SPI).*

The following table shows the pinout for the `ATSAMD21G18A-U` SERCOM and the uses that we are making of these pins for the project. This table was sourced from ADafruit's fantastic guide on using the Feather M0 that can be [found on their website](https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/overview).

|Microcontroller Pin|Arduino 'Pin'|SERCOM|SERCOM alt|Configured Interface|
|---|---|---|---|--|
|PA00|Used by xtal|SERCOM1.0|||
|PA01|Used by xtal|SERCOM1.1|||
|PB08|A1|SERCOM4.0|||
|PB09|A2|SERCOM4.1|||
|PA04|A3|SERCOM0.0|||
|PA05|A4|SERCOM0.1|||
|PA06|D8|SERCOM0.2|||
|PA07|D9|SERCOM0.3|||
|PA08|D4|SERCOM0.0|SERCOM2.0|Serial3 - Cellular|
|PA09|D3|SERCOM0.1|SERCOM2.1|Serial3 - Cellular|
|PA10|D1|SERCOM0.2|SERCOM2.2|Serial1 - GPS|
|PA11|D0|SERCOM0.3|SERCOM2.3|Serial1 - GPS|
|PB10|D23 / MOSI|SERCOM4.2||SPI - MicroSD|
|PB11|D24 / SCK|SERCOM4.3||SPI - MicroSD|
|PA12|D22 / MISO|SERCOM2.0|SERCOM4.0|SPI - MicroSD|
|PA13|Used by EDBC|SERCOM2.1|SERCOM4.1||
|PA14|D2|SERCOM2.2|SERCOM4.2||
|PA15|D5|SERCOM2.3|SERCOM4.3||
|PA16|D11|SERCOM1.0|SERCOM3.0|Serial2 - Radio|
|PA17|D13|SERCOM1.1|SERCOM3.1||
|PA18|D10|SERCOM1.2|SERCOM3.2|Serial2 - Radio|
|PA19|D12|SERCOM1.3|SERCOM3.3||
|PA20|D6|SERCOM5.2|SERCOM3.2||
|PA21|D7|SERCOM5.3|SERCOM3.3||
|PA22|D20 / SDA|SERCOM3.0|SERCOM5.0|I2C - Sensors|
|PA23|D21 / SCL|SERCOM3.1|SERCOM5.1|I2C - Sensors|
|PA24|Used by USB|SERCOM3.2|SERCOM5.2|Serial - Debug|
|PA25|Used by USB|SERCOM3.3|SERCOM5.3|Serial - Debug|
|PB22|D30 / EDBG TX|SERCOM5.2||Serial5 - APRS|
|PB23|D31 / EDBG RX|SERCOM5.3||Serial5 - APRS|
|PA30|Used by SWCLK|SERCOM1.2|||
|PA31|Used by SWDIO|SERCOM1.3|||
|PB02|A5|SERCOM5.0|||
|PB03|D25 / RX LED|SERCOM5.1|||
