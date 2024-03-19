
//include the library
#include "LedControlMS.h"

/*
Configuring the LEDMatrix:
Digital 11 is conneted to DIN (Data IN)
Digital 13 is connected to CLK (CLocK)
Digital 7 is connected to CS (LOAD)
5V is connected to VCC
GND is connected to GND
There is only one MAX7219 display module.
*/
#include <avr/pgmspace.h>

#define NBR_MTX 1
LedControl lc(11,7,13, NBR_MTX);


String sentence= "ABCD";


byte A[] = { 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa };


char buffer[21];


void setup() { // initalizes and sets up the initial values. Declaring function setup.
  Serial.begin(9600);
  lc.init();
  lc.setIntensity(0x01);
  lc.displayChar(A);
}

void loop() { 

  /*
  Le type de données recues (N octet):
    - 1er octet : 
      .bit 7=0 => afficher un char; bit7=1 => afficher un texte
      .bit 6 - bit 0: => Le nombre de caractère dans le texte (7bits => 127 carac possibles)
    - 2 => Nème octet : data  
  */
  if(Serial.available()){
    byte header[1];
    int data = Serial.readBytes(header, 1);
    //Extraction du type de l'opération
    int opcode = *header >> 7;
    
    //Affichage d'un caractère ou d'un sprite
    if(opcode == 0){
      char c[1];
      data = Serial.readBytes(c, 1);
      lc.displayChar(c);
    }
    //Affichage d'une chaine de caractères
    if(opcode == 1){
      //initialisation d'une chaine de caractères avec la taille correspondante
      int size = *header & 0b01111111;
      char sentence[size];
      data = Serial.readBytes(sentence, size);
      String s(sentence);
      printStringWithShift(s, 300, 0);
    }
  }
  delay(100);

}


// Display=the extracted characters with scrolling
void printCharWithShift(char c, int shift_speed) {
  for(int i=9; i>=-2;i--){
    lc.clear();
    // delay(100);
    lc.displayChar(c,i);
    delay(shift_speed);
  }

}
// Extract the characters from the text string
void printStringWithShift(String s, int shift_speed, int space) {
  int length = s.length();
  int start_pos = 8;
  int end_pos = start_pos + (length-1) * space + (length-1) * CHAR_WIDTH; 
  int counter = 0;

  while(end_pos > -CHAR_WIDTH){

    for(int i=0; i<length; i++){
      int pos = start_pos + i*(CHAR_WIDTH+space) - counter;

      lc.displayChar(s.charAt(i), pos);
    }
    counter++;

    end_pos -= 1;

    delay(shift_speed);
    lc.clear();
  }
  
}
