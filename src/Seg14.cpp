#include "Arduino.h"
#include "Seg14.h"

static const uint16_t characters[] PROGMEM = {
  0b0000110000111111, // 0
  0b0000010000000110, // 1
  0b0000100010001011, // 2
  0b0000000011001111, // 3
  0b0001001011100000, // 4
  0b0010000001101001, // 5
  0b0000000011111101, // 6
  0b0001010000000001, // 7
  0b0000000011111111, // 8
  0b0000000011100111, // 9
  
  0b0110000010000011, // ?    10
  0b0000001010111011, // @
  0b0000000011110111, // A    
  0b0000010011111101, // B
  0b0000000000111001, // C
  0b0010000100111000, // D
  // E C A98 65 3210
  0b0000000011111001, // E
  0b0001001011000001, // F
  0b0000000010111101, // G
  // E C A98 65 3210
  0b0000000011110110, // H
  0b0001001000000000, // I
  0b0000000000011110, // J
  // E C A98 65 3210
  0b0010010001110000, // K
  0b0000000000111000, // L
  0b0000010100110110, // M
  // E C A98 65 3210
  0b0010000100110110, // N
  0b0000000000111111, // O
  0b0000000011110011, // P
  // E C A98 65 3210
  0b0010000000111111, // Q
  0b0010000011110011, // R
  0b0000000110001101, // S
  // E C A98 65 3210
  0b0001001000000001, // T
  0b0000000000111110, // U
  0b0000110000110000, // V
  // E C A98 65 3210
  0b0010100000110110, // W
  0b0010110100000000, // X
  0b0001010100000000, // Y
  0b0000110000001001, // Z
  
  0b0000000000001000, // _   38
  0b0000000100000000, // ` 
  // E C A98 65 3210
  0b0001000101011000, // a
  0b0000000011111100, // b
  0b0000000011011000, // c
  0b0000000011011110, // d
  // E C A98 65 3210
  0b0000100001011000, // e
  0b0000000001110001, // f
  0b0000010011101101, // g
  0b0001000001110000, // h
  // E C A98 65 3210
  0b0001000000000000, // i
  0b0010000000000110, // j
  0b0011011000000000, // k
  0b0000100000110000, // l
  // E C A98 65 3210
  0b0001000011010100, // m
  0b0001000001010000, // n
  0b0000000011011100, // o
  0b0000000101110000, // p
  // E C A98 65 3210
  0b0010000011011100, // q
  0b0000000001010000, // r
  0b0010000010001000, // s
  0b0001001010000000, // t
  // E C A98 65 3210
  0b0000000000011100, // u
  0b0000100000010000, // v
  0b0010100000010100, // w
  // E C A98 65 3210
  0b0001101100000000, // x
  0b0000110100000000, // y
  0b0000100001001000, // z


  0b0000000000000000, // space    66
  0b0100000000000010, // !
  0b0000001000000010, // "

  0b0000001000000000, // '        69
  0b0010010000000000, // (
  0b0000100100000000, // )
  0b0011111111000000, // *
  0b0001001011000000, // +
  0b0000100000000000, // ,
  0b0000000011000000, // -
  0b0100000000000000, // .
  0b0000110000000000, // /

  0b0000010111011100, // ö        78
  0b0000010100011100, // ü
  0b0000000011001000, // =
  0b0000000011100011, // 176 0xb0 '°'
  0b0010000100000000, // backslash
};


void Seg14::_writeCommand(uint8_t command) {
  Wire.beginTransmission(_address);
  Wire.write(command);
  Wire.endTransmission();
}

static uint16_t Seg14::getCharacter(uint8_t index) {
  return pgm_read_word(characters + min(index, 82));
}

static uint16_t Seg14::getDecimal(uint8_t index) {
  return pgm_read_word(characters + min(index, 9));
}

static uint16_t Seg14::getHex(uint8_t index) {
  return pgm_read_word(characters + min(index, 15));
}

static uint16_t Seg14::getUpper(uint8_t index) {
  return pgm_read_word(characters + min(12 + index, 37));
}

static uint16_t Seg14::getLower(uint8_t index) {
  return pgm_read_word(characters + min(40 + index, 65));
}

void Seg14::setOscillatorOn(bool on) {
  _writeCommand(on ? HT16K33_OSCILLATOR_ON : HT16K33_OSCILLATOR_STANDBY);
}

void Seg14::setDisplayOn(bool on) {
  _writeCommand(on ? HT16K33_DISPLAY_ON : HT16K33_DISPLAY_OFF);
}

// Duty cycle = (brightness + 1) / 16, so brightness = 0 is still on.
void Seg14::setBrightness(uint8_t brightness) {
  _writeCommand(HT16K33_BRIGHTNESS | (brightness & 0xF));
}

// [OFF, 2HZ, 1HZ, 0.5HZ]
void Seg14::setBlinkMode(uint8_t mode) {
  _writeCommand(HT16K33_DISPLAY_ON | (min(mode, 3) << 1));
}

// Write a character to a specific display 0-7.
void Seg14::write(uint16_t character, uint8_t displayIndex) {
  writeStream(character, displayIndex, displayIndex);
}

// Write to several displays in the same transmission.
uint8_t Seg14::writeStream(uint16_t character, uint8_t startIndex) {
  return writeStream(character, startIndex, _nDisplays);
}
uint8_t Seg14::writeStream(uint16_t character, uint8_t startIndex, uint8_t endIndex) {
  if (_streamIndex == 0) {
    _streamIndex = startIndex;
    Wire.beginTransmission(_address);
    Wire.write(startIndex * 2);
  }
  Wire.write(character & 0xFF);
  Wire.write(character >> 8);
  if (++_streamIndex >= endIndex) {
    Wire.endTransmission();
    _streamIndex = 0;
  }
  return _streamIndex;
}

void Seg14::print(const char * letters, uint8_t i) {
    for (; i<_nDisplays; ++i) {
        uint8_t index = letters[i];
        if (index == 148) {
            index = 78;
        } else if (index == 129) {
            index = 79;
        } else if (index == '=') {
            index = 80;
        } else if (index == 176) {
            index = 81;
        } else if (index == '\\') {
            index = 82;
        } else if (index < ' ') {
            index = 66; // nothing
        } else if (index <= '"') {
            index = index - ' ' + 66;
        } else if (index <= '/') {
            index = index - '\'' + 69;
        } else if (index <= '9') {
            index = index - '0';
        } else if (index <= 'Z') {
            index = index-'?' + 10;
        } else {
            index = index-'_' + 38;
        }
        writeStream(pgm_read_word(characters + index), i, i);
    }
    
}

Seg14::Seg14(uint8_t nDisplays, uint8_t address) {
  _address = address;
  _nDisplays = nDisplays;
  _streamIndex = 0;
}

void Seg14::init() {
  Wire.begin();
  setOscillatorOn();
  while(writeStream(0x0000)) {};
  setBlinkMode(0);
  setBrightness(0);
  setDisplayOn();
}
