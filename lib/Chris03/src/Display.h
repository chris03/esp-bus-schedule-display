#ifndef CHRIS03_DISPLAY_H
#define CHRIS03_DISPLAY_H

#include <SPI.h>
#include <TFT_eSPI.h>

namespace Chris03
{
class Display
{
private:
  int16_t lastPrintX;
  int16_t lastPrintY;
  uint8_t lastPrintFont;
  String lastPrintText;

  void undoLastPrint();
  void print();

public:
  Display();
  ~Display();
  void init(uint8_t rotation);
  void printLastText();
  void clear();
  void setTextColor(uint16_t color);
  void printCenterBottom(String text, uint8_t fontSize);
  void printCenter(String text, uint8_t fontSize);

  TFT_eSPI *tft;
};
}
#endif