#include <NIBOburger.h>
 
void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.checkVoltage();
  unsigned int l = SensorFLL.get();
  unsigned int r = SensorFRR.get();
  
  NIBOburger.setLed(LED1, l>15);
  NIBOburger.setLed(LED2, l>25);
  NIBOburger.setLed(LED3, r>25);
  NIBOburger.setLed(LED4, r>15);
    
  delay(10);
}
