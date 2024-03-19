/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControl_h
#define LedControl_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
const static byte charTable[128] = {
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000,
  B01111111, B01111011, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B10000000, B00000001, B10000000, B00000000,
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000,
  B01111111, B01111011, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B00000000,
  B00110111, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B00000000,
  B01100111, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001000,
  B00000000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B00000000,
  B00110111, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B00000000,
  B01100111, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};

#define CHAR_WIDTH 6


const static byte alphabetBitmap[41][CHAR_WIDTH]={
	{0x7E,0x81,0x81,0x81,0x7E,0x0}, //0
	{0x4,0x82,0xFF,0x80,0x0,0x0},  //1
	{0xE2,0x91,0x91,0x91,0x8E,0x0},//2
	{0x42,0x89,0x89,0x89,0x76,0x0},//3
	{0x1F,0x10,0x10,0xFF,0x10,0x0},//4
	{0x8F,0x89,0x89,0x89,0x71,0x0},//5
	{0x7E,0x89,0x89,0x89,0x71,0x0},//6
	{0x1,0x1,0xF9,0x5,0x3,0x0},//7
	{0x76,0x89,0x89,0x89,0x76,0x0},//8
	{0x8E,0x91,0x91,0x91,0x7E,0x0},//9
	{0x0,0x0,0x0,0x0,0x0,0x0},// blank space
	{0x0,0x0,0x90,0x0,0x0,0x0}, //:
	{0x0,0x10,0x10,0x10,0x10,0x0},// -
	{0x0,0x0,0x80,0x0,0x0,0x0},// .
	{0xFC,0x9,0x11,0x21,0xFC,0x0},//�
	{0xFE,0x11,0x11,0x11,0xFE,0x0},//A
	{0xFF,0x89,0x89,0x89,0x76,0x0},//B
	{0x7E,0x81,0x81,0x81,0x42,0x0},//C
	{0xFF,0x81,0x81,0x81,0x7E,0x0},//D
	{0xFF,0x89,0x89,0x89,0x81,0x0},//E
	{0xFF,0x9,0x9,0x9,0x1,0x0},//F
	{0x7E,0x81,0x81,0x91,0x72,0x0},//G
	{0xFF,0x8,0x8,0x8,0xFF,0x0},//H
	{0x0,0x81,0xFF,0x81,0x0,0x0},//I
	{0x60,0x80,0x80,0x80,0x7F,0x0},//J
	{0xFF,0x18,0x24,0x42,0x81,0x0},//K
	{0xFF,0x80,0x80,0x80,0x80,0x0},//L
	{0xFF,0x2,0x4,0x2,0xFF,0x0},//M
	{0xFF,0x2,0x4,0x8,0xFF,0x0},//N
	{0x7E,0x81,0x81,0x81,0x7E,0x0},//O
	{0xFF,0x11,0x11,0x11,0xE,0x0},//P
	{0x7E,0x81,0x81,0xA1,0x7E,0x80},//Q
	{0xFF,0x11,0x31,0x51,0x8E,0x0},//R
	{0x46,0x89,0x89,0x89,0x72,0x0},//S
	{0x1,0x1,0xFF,0x1,0x1,0x0},//T
	{0x7F,0x80,0x80,0x80,0x7F,0x0},//U
	{0x3F,0x40,0x80,0x40,0x3F,0x0},//V
	{0x7F,0x80,0x60,0x80,0x7F,0x0},//W
	{0xE3,0x14,0x8,0x14,0xE3,0x0},//X
	{0x3,0x4,0xF8,0x4,0x3,0x0},//Y
	{0xE1,0x91,0x89,0x85,0x83,0x0}//Z
};


class LedControl {
private:
  byte dataPin;
  byte loadPin;
  byte clockPin;
  byte num;
  byte buffer[80];

  void reload();

  //Returns the array number in the alphabetBitmap array 
	int getCharArrayPosition(char c);

public:
  LedControl(byte data, byte load, byte clock, byte num);

  void init();
  void clear();
  void setCommand(byte command, byte value);
  void setIntensity(byte intensity);
  void setColumn(byte col, byte value);
  void setColumnAll(byte col, byte value);
  void setRow(byte row, byte value);
  void setDot(byte col, byte row, byte value);
  void writeSprite(const byte* sprite, int x=0);
  void displayChar(const char c, int x=0);

  void shiftLeft(bool rotate = false, bool fill_zero = true);
  void shiftRight(bool rotate = false, bool fill_zero = true);
  void shiftUp(bool rotate = false);
  void shiftDown(bool rotate = false);
};

#endif  //LedControl.h
