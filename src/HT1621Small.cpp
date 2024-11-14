#include "HT1621Small.h"
#include <Arduino.h>

HT1621Small::HT1621Small() : HT1621() {

}

void HT1621Small::print(const char* str){
  for(int i = 0; i < 4; i++){
    setCharInDisplay(str[i], i+1);
  }

  update();
}

void HT1621Small::setDegree(bool state) {
  setClearBit(0, state);
  wrone(6, _buffer[3]);
}

void HT1621Small::setMiddleDot(bool state) {
  setClearBit(1, state);
  wrone(6, _buffer[3]);
}

void HT1621Small::setDecimal(bool first, bool second) {
  setClearBit(3, first);
  setClearBit(2, second);
  wrone(6, _buffer[3]);
}

void HT1621Small::update() {
  wrone(0, _buffer[0]);
  wrone(2, _buffer[1]);
  wrone(4, _buffer[2]);
  wrone(6, _buffer[3]);
}

void HT1621Small::setClearBit(uint8_t bitnum, bool state) {
  if (state) {
    _buffer[3] |= (1 << bitnum);
  } else {
    _buffer[3] &= ~(1 << bitnum);
  }
}

void HT1621Small::setCharInDisplay(char ch, uint8_t position) {
    if (position > 4) {
        return;
    }

    uint8_t segments = charToSegBits(ch);
    uint8_t common = 4 - position;  // C3 for position 1, C2 for position 2, etc.

    // Clear the bits for segments 0-5
    for (int i = 0; i < 3; i++) {
        _buffer[i] &= ~(1 << common);            // Clear bits in each byte for this position
        _buffer[i] &= ~(1 << (common + 4));      // Clear bits for higher segment
    }

    // Skip clearing segment 6, dots, degree
    // Clear segment 7
    _buffer[3] &= ~(1 << (common + 4));      // Clear bits for higher segment

    // Map each segment to the correct byte and bit within _buffer based on segmentPatterns
    if (segments & 0b00000001) _buffer[0] |= (1 << (common + 4)); // A -> S1
    if (segments & 0b00000010) _buffer[0] |= (1 << common);       // B -> S0
    if (segments & 0b00000100) _buffer[1] |= (1 << (common + 4)); // C -> S3
    if (segments & 0b00001000) _buffer[1] |= (1 << common);       // D -> S2
    if (segments & 0b00010000) _buffer[2] |= (1 << (common + 4)); // E -> S5
    if (segments & 0b00100000) _buffer[2] |= (1 << common);       // F -> S4
    if (segments & 0b01000000) _buffer[3] |= (1 << (common + 4)); // G -> S7
    
    // Skip setting dots, degrees, will do by other functions
    //if (segments & 0b10000000) _buffer[3] |= (1 << common);       // P -> S6 (dot segment, if used)
}

const uint8_t segmentPatterns[10] = {
    0b00111111, // 0: A, B, C, D, E, F
    0b00000110, // 1: B, C
    0b01011011, // 2: A, B, D, E, G
    0b01001111, // 3: A, B, C, D, G
    0b01100110, // 4: B, C, F, G
    0b01101101, // 5: A, C, D, F, G
    0b01111101, // 6: A, C, D, E, F, G
    0b00000111, // 7: A, B, C
    0b01111111, // 8: A, B, C, D, E, F, G
    0b01101111  // 9: A, B, C, D, F, G
};

char HT1621Small::charToSegBits(char character) {
switch (character) {
  case '*': // For degree for now
    return 0b01100011;
  case '|':
    return 0b00110000;
  case '-':
    return 0b01000000;
  case '_':
    return 0b00001000;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return segmentPatterns[character-'0'];
  case 'A':
  case 'a':
    return 0b01110111;
  case 'b':
  case 'B':
    return 0b01111100;
  case 'c':
  case 'C':
    return 0b00111001;
  case 'd':
  case 'D':
    return 0b01011110;
  case 'e':
  case 'E':
    return 0b01111001;
  case 'f':
  case 'F':
    return 0b01110001;
  case 'r':
  case 'R':
    return 0b01010000;
  case ' ':
  default:
    return 0b00000000;
  }
}
