#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
  GFX.begin();
}

void loop() {
  GFX.move(30, 20);
  GFX.set_proportional(0);
  GFX.print_text("hello world!");
  GFX.move(30, 30);
  GFX.set_proportional(1);
  GFX.print_text("hello world!");
  delay(1000);
}

