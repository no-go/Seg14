// For use with 14-segment displays driven by a HT16K33.
//
// Expected connections for font:
//
//       0
//   +-------+
//   |\  |  /|
// 5 | 8 9 A | 1
//   |  \|/  |
//   +-6-+-7-+
//   |  /|\  |
// 4 | B C D | 2
//   |/  |  \|    _
//   +-------+   |E|
//       3
//
// Other libraries:
// https://github.com/rileyjshaw/rileyjshaw/Seg16
// https://github.com/adafruit/Adafruit_LED_Backpack
// https://github.com/jonpearse/ht16k33-arduino

#ifndef Seg14_h
#define Seg14_h

#include "Arduino.h"
#include <Wire.h>
#include <avr/pgmspace.h>

// Commands from datasheet.
#define HT16K33_OSCILLATOR_ON       0x21
#define HT16K33_OSCILLATOR_STANDBY  0x20
#define HT16K33_DISPLAY_ON          0x81
#define HT16K33_DISPLAY_OFF         0x80
#define HT16K33_BRIGHTNESS          0xE0

class Seg14 {
  public:
    Seg14(uint8_t nDisplays = 4, uint8_t address = 0x70);
    void init();
    void setOscillatorOn(bool on = true);
    void setDisplayOn(bool on = true);
    void setBrightness(uint8_t brightness);
    void setBlinkMode(uint8_t mode);
    static uint16_t getCharacter(uint8_t index);
    static uint16_t getDecimal(uint8_t index);
    static uint16_t getHex(uint8_t index);
    static uint16_t getUpper(uint8_t index);
    static uint16_t getLower(uint8_t index);
    void write(uint16_t character, uint8_t displayIndex = 0);
    uint8_t writeStream(uint16_t character, uint8_t startIndex = 0);
    uint8_t writeStream(uint16_t character, uint8_t startIndex, uint8_t endIndex);
    void print(const char * letters, uint8_t i = 0);

  private:
    uint8_t _address;
    uint8_t _nDisplays = 8;
    uint8_t _streamIndex;
    void _writeCommand(uint8_t command);
};

#endif
