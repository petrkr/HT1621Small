#pragma once

#include <HT1621.h>
#include <Arduino.h>

class HT1621Small : public HT1621 {
public:
  HT1621Small();
  ~HT1621Small();

  void print(const char* str);
  void setDegree(bool state);
  void setMiddleDot(bool state);
  void setDecimal(bool first, bool second);

private:
  void setCharInDisplay(char ch, uint8_t position);
  void setClearBit(uint8_t bitnum, bool state);
  char charToSegBits(char character) override;
  void update() override;
};
