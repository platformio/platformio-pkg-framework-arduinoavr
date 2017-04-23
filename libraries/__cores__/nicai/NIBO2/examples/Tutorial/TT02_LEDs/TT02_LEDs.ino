#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
}

void loop() {
  int farbe;
  for (int farbe=0; farbe<4; farbe++) {
    for (int led=0; led<8; led++) {
      NIBO2.setLed(led, farbe);
      delay(150);
    }
  }
}

