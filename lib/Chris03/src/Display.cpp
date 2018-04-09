#include "Display.h"

namespace Chris03
{

Display::Display()
{
    tft = new TFT_eSPI();
}

Display::~Display()
{
    delete tft;
}

void Display::init(uint8_t rotation)
{
    tft->init();
    tft->setRotation(rotation);
    tft->fillScreen(TFT_BLACK);
    tft->setTextWrap(false);
}

void Display::undoLastPrint()
{
    if (lastPrintText != "")
    {
        // Undo last print
        tft->setTextColor(TFT_BLACK);
        tft->setTextFont(lastPrintFont);
        tft->setCursor(lastPrintX, lastPrintY);
        tft->print(lastPrintText);
    }
}

void Display::print()
{
    tft->setTextColor(TFT_WHITE);
    tft->setTextFont(lastPrintFont);
    tft->setCursor(lastPrintX, lastPrintY);
    tft->print(lastPrintText);
}

void Display::printCenterBottom(String text, uint8_t fontSize)
{
    if (text == lastPrintText && fontSize == lastPrintFont)
    {
        return;
    }
    undoLastPrint();

    const int16_t textWidth = tft->width() - tft->textWidth(text);
    lastPrintFont = fontSize == 0 ? 1 : fontSize;
    lastPrintX = textWidth < 0 ? 0 : (textWidth >> 1);
    lastPrintY = tft->height() - tft->fontHeight(lastPrintFont);
    lastPrintText = text;

    print();
}

void Display::printCenter(String text, uint8_t fontSize)
{
    if (text == lastPrintText && fontSize == lastPrintFont)
    {
        return;
    }
    undoLastPrint();

    const int16_t textWidth = tft->width() - tft->textWidth(text);
    lastPrintFont = fontSize == 0 ? 1 : fontSize;
    lastPrintX = textWidth < 0 ? 0 : (textWidth >> 1);
    lastPrintY = (tft->height() - tft->fontHeight(lastPrintFont)) >> 1;
    lastPrintText = text;

    print();
}

void Display::setTextColor(uint16_t color)
{
    tft->setTextColor(color);
}

void Display::clear()
{
    tft->setCursor(0, 0);
    lastPrintText = "";
    tft->fillScreen(TFT_BLACK);
}
}