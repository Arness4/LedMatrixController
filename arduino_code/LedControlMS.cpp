/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
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


#include "LedControlMS.h"

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP 0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE 9
#define OP_INTENSITY 10
#define OP_SCANLIMIT 11
#define OP_SHUTDOWN 12
#define OP_DISPLAYTEST 15

LedControl::LedControl(byte _dataPin, byte _load, byte _clockPin, byte _num) {
  dataPin = _dataPin;
  loadPin = _load;
  clockPin = _clockPin;
  num = _num;
  for (int i = 0; i < 80; i++)
    buffer[i] = 0;
}

void LedControl::init() {

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  digitalWrite(clockPin, HIGH);

  setCommand(OP_SCANLIMIT, 0x07);
  setCommand(OP_DECODEMODE, 0x00);   // using an led matrix (not digits)
  setCommand(OP_SHUTDOWN, 0x01);     // not in shutdown mode
  setCommand(OP_DISPLAYTEST, 0x00);  // no display test

  // empty registers, turn all LEDs off
  clear();

  setIntensity(0x01);  // the first 0x0f is the value you can set
}

void LedControl::setIntensity(byte intensity) {
  setCommand(OP_INTENSITY, intensity);
}

void LedControl::clear() {
  for (int i = 0; i < 8; i++)
    setColumnAll(i, 0);

  for (int i = 0; i < 80; i++)
    buffer[i] = 0;
}

void LedControl::setCommand(byte command, byte value) {
  digitalWrite(loadPin, LOW);
  for (int i = 0; i < num; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, command);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
  }
  digitalWrite(loadPin, LOW);
  digitalWrite(loadPin, HIGH);
}

void setRow(byte row, byte value) {
}


void LedControl::setColumn(byte col, byte value) {
  int n = col / 8;
  int c = col % 8;
  //Rendre LOAD au niveau bas
  digitalWrite(loadPin, LOW);
  for (int i = 0; i < num; i++) {
    if (i == n) {
      shiftOut(dataPin, clockPin, MSBFIRST, c + 1);
      shiftOut(dataPin, clockPin, MSBFIRST, value);
    } else {
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
    }
  }
  digitalWrite(loadPin, LOW);
  //Remettre LOAD au niveau haut pour commencer la lecture des données envoyées.
  digitalWrite(loadPin, HIGH);

  buffer[col] = value;
}

void LedControl::setColumnAll(byte col, byte value) {
  digitalWrite(loadPin, LOW);
  for (int i = 0; i < num; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, col + 1);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    buffer[col * i] = value;
  }
  digitalWrite(loadPin, LOW);
  digitalWrite(loadPin, HIGH);
}

void LedControl::setDot(byte col, byte row, byte value) {
  bitWrite(buffer[col], row, value);

  int n = col / 8;
  int c = col % 8;
  digitalWrite(loadPin, LOW);
  for (int i = 0; i < num; i++) {
    if (i == n) {
      shiftOut(dataPin, clockPin, MSBFIRST, c + 1);
      shiftOut(dataPin, clockPin, MSBFIRST, buffer[col]);
    } else {
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
      shiftOut(dataPin, clockPin, MSBFIRST, 0);
    }
  }
  digitalWrite(loadPin, LOW);
  digitalWrite(loadPin, HIGH);
}

//Afficher une sprite prédéfinie à la colonne col
void LedControl::writeSprite(const byte* sprite, int col) {
  for(int i=0; i < 8;i++){
    setColumn(col+i, sprite[i]);
  }
}

//Afficher une lettre à la colonne col dans le tableau alphabetBitmap
void LedControl::displayChar(const char c, int col) {
  int charIndex = getCharArrayPosition(c);

  for(int i=0; i < CHAR_WIDTH;i++){
    setColumn(col+i, alphabetBitmap[charIndex][i]);
  }
}


int LedControl::getCharArrayPosition(char input) {
  if ((input == ' ') || (input == '+')) return 10;
  if (input == ':') return 11;
  if (input == '-') return 12;
  if (input == '.') return 13;
  if ((input == '(')) return 14;  //replace by '�'
  if ((input >= '0') && (input <= '9')) return (input - '0');
  if ((input >= 'A') && (input <= 'Z')) return (input - 'A' + 15);
  if ((input >= 'a') && (input <= 'z')) return (input - 'a' + 15);
  return 13;
}

void LedControl::reload() {
  for (int i = 0; i < 8; i++) {
    int col = i;
    digitalWrite(loadPin, LOW);
    for (int j = 0; j < num; j++) {
      shiftOut(dataPin, clockPin, MSBFIRST, i + 1);
      shiftOut(dataPin, clockPin, MSBFIRST, buffer[col]);
      col += 8;
    }
    digitalWrite(loadPin, LOW);
    digitalWrite(loadPin, HIGH);
  }
}

void LedControl::shiftLeft(bool rotate, bool fill_zero) {
  byte old = buffer[0];
  int i;
  for (i = 0; i < 80; i++) {
    buffer[i] = buffer[i + 1];
  }

  if (rotate) buffer[num * 8 - 1] = old;
  else if (fill_zero) buffer[num * 8 - 1] = 0;

  reload();
}

void LedControl::shiftRight(bool rotate, bool fill_zero) {
  int last = num * 8 - 1;
  byte old = buffer[last];
  int i;
  for (i = 79; i > 0; i--)
    buffer[i] = buffer[i - 1];
  if (rotate) buffer[0] = old;
  else if (fill_zero) buffer[0] = 0;

  reload();
}

void LedControl::shiftUp(bool rotate) {
  for (int i = 0; i < num * 8; i++) {
    bool b = buffer[i] & 1;
    buffer[i] >>= 1;
    if (rotate) bitWrite(buffer[i], 7, b);
  }
  reload();
}

void LedControl::shiftDown(bool rotate) {
  for (int i = 0; i < num * 8; i++) {
    bool b = buffer[i] & 128;
    buffer[i] <<= 1;
    if (rotate) bitWrite(buffer[i], 0, b);
  }
  reload();
}
