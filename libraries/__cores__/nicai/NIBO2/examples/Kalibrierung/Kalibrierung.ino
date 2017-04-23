/*
  Dieses Programm dient zur Kalibrierung der Bodensensoren.
  Nach abgeschlossener Übertragung können Sie die Sensoren kalibrieren:
   ● Rechtes Rücklicht leuchtet: Stellen Sie den Roboter auf einen
     schwarzen Untergrund und drücken Sie den Taster S3.
   ● Linkes Rücklicht leuchtet: Stellen Sie den Roboter auf einen 
     weißen Untergrund und drücken Sie den Taster S3.
   ● Beide Rücklichter leuchten: Drücken Sie den Taster S3 um die 
     Parameter im EEPROM zu speichern.
*/

#include <NIBO2.h>
#include <NIBO_GFX.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

#define PORT_RED        PORTE
#define PORT_GREEN  PORTC

uint8_t mode;

float volt_flt = 10.0;

void printHexW(unsigned int value, unsigned char width) {
  unsigned int v2 = value;
  while (width--) {
    if (v2==0) {
      GFX.print('0');
    }
    v2 /= 0x10;
  }
  if (value) {
    GFX.print(value, HEX);
  }
}


/* Zeige aktuelle Funktion auf dem Display an */
void print_msg(uint8_t mode) {
  GFX.move(20, 45);
  GFX.set_proportional(1);
  switch (mode) {
    case 0: GFX.print("Press S3 for  "); break;
    case 1: GFX.print("Press S3 for  "); break;
    case 2: GFX.print("Press S3 to  "); break;
  };
  GFX.move(20, 55);
  switch (mode) {
    case 0: GFX.print("black calibration  "); break;
    case 1: GFX.print("white calibration  "); break;
    case 2: GFX.print("store calibration  "); break;
  };
}

void setup() {
  NIBO2.begin();
  GFX.begin();
  NIBO2.setDisplayIntensity(50);
  
  GFX.fill(0x00);
  GFX.move(21, 0);
  GFX.set_proportional(1);
  GFX.print_text("Sensor Calibration");
  GFX.set_proportional(0);

  print_msg(0);
}

void loop () {
    delay(20);

    // Spannung
    float volt = NIBO2.getVoltage() / 1000.0;
    GFX.move(29, 10);
    GFX.set_proportional(1);
    GFX.print("Supply: ");
    GFX.set_proportional(0);
    GFX.print(volt,1);
    GFX.print("V");

    // Spannung filtern: 90% alter Wert, 10% neuer Wert.
    volt_flt = 0.9*volt_flt+0.1*volt;

    // Wenn die gefilterete Spannung unter 9V sinkt sollten die Akkus geladen werden
    if (volt_flt<9.0) {
      GFX.fill(0x00);
      GFX.move(25, 20);
      GFX.set_proportional(1);
      GFX.print("Please recharge");
      GFX.move(35, 30);
      GFX.print("batteries!");
      GFX.set_proportional(0);
      while(1) {
        NIBO2.setHeadlightsIntensity(0);
        NIBO2.disableCopro();
        NIBO2.setDisplayIntensity(500);
        PORTE = 0xaa;
        PORTC = 0x00;
        delay(500);
        NIBO2.setDisplayIntensity(0);
        PORTE = 0x55;
        PORTC = 0x00;
        delay(500);
      }
    }
    
    // Bodensensoren aktualisieren:
    uint16_t value_afr = FloorSensor.getR1(1);
    uint16_t value_afl = FloorSensor.getL1(1);
    uint16_t value_alr = FloorSensor.getR0(1);
    uint16_t value_all = FloorSensor.getL0(1);

    uint16_t value_rfr = FloorSensor.getR1(0);
    uint16_t value_rfl = FloorSensor.getL1(0);
    uint16_t value_rlr = FloorSensor.getR0(0);
    uint16_t value_rll = FloorSensor.getL0(0);

    // Werte der Bodensensoren auf dem Display ausgeben:
    GFX.move(5, 20);
    printHexW(value_afr, 4);
    GFX.print_char(' ');
    printHexW(value_alr, 4);
    GFX.print_char(' ');
    printHexW(value_all, 4);
    GFX.print_char(' ');
    printHexW(value_afl, 4);
    
    GFX.move(5, 30);
    printHexW(value_rfr, 4);
    GFX.print_char(' ');
    printHexW(value_rlr, 4);
    GFX.print_char(' ');
    printHexW(value_rll, 4);
    GFX.print_char(' ');
    printHexW(value_rfl, 4);

    uint8_t ledr = 0x00;
    uint8_t ledg = 0x00;

    // vordere grüne LEDs in Abhängigkeit der Bodensensoren einschalten:
    if (value_rll>0x50) ledg |= 0x10;
    if (value_rlr>0x50) ledg |= 0x20;
    if (value_rfl>0x50) ledg |= 0x08;
    if (value_rfr>0x50) ledg |= 0x40;

    // hintere grüne LEDs in Abhängigkeit des aktuellen Zustands einschalten:
    switch (mode) {
      case 0: ledg |= 0x01; break;
      case 1: ledg |= 0x02; break;
      case 2: ledg |= 0x03; break;
    }

    PORT_RED = ledr;
    PORT_GREEN = ledg;

    // Wenn der Taster S3 gedrückt wurde, ...
    if (NIBO2.getS3()) {
      // ... 20 ms entprellen ...
      do {
        delay(20);
      } while (NIBO2.getS3());
      // ... und der Taster wieder losgelassen wurde:

      // Modus weiterschalten und mit den roten LEDs die Funktion anzeigen
      mode++;
        switch (mode) {
          case 1:
            PORT_GREEN = 0x00;
            PORT_RED = 0x01|0x78;
            FloorSensor.calibrateBlack();
            break;
          case 2:
            PORT_GREEN = 0x00;
            PORT_RED = 0x02|0x78;
            FloorSensor.calibrateWhite();
            break;
          case 3:
            PORT_GREEN = 0x00;
            PORT_RED = 0x04|0x78;
            FloorSensor.storeCalibration();
            mode=0;
            break;
        }
        print_msg(mode);
	// eine 
        delay(1000);	

    }

}


