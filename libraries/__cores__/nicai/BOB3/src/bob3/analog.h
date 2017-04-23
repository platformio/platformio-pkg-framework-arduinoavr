/*  BSD-License:

Copyright (c) 2016 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*! @file    analog.h
 *  @brief   Die Funktionen ermöglichen den Zugriff auf die analogen Eingänge (Sensorbricks, KEY und Versorgungsspannung).
 * 
 *  Die analogen Eingänge werden automatisch im Interrupt gemessen. 
 *  Damit die anlogen Daten gemessen werden können, muss zuerst (zum Beispiel in der setup() Funktion) die Funktion analog_init() aufgerufen werden.
 *  Falls nicht die Processing-Umgebung <niboburger/robomain.h> verwendet wird, müssen auch die globalen Interrupts manuell eingeschaltet werden.
 * 
 *  <b>Anhaltswerte:</b> Die IR-Sensoren liefern für nahe Objekte Werte >50 und für Freiräume Werte <10.
 */

#ifndef _BOB3_ANALOG_H_
#define _BOB3_ANALOG_H_


#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  Symbolische Namen für den Parameter idx der Funktionen
 *  analog_getValue(), analog_getValueExt() und analog_setExtToggleMode()
 */
enum {
  ANALOG_L0,
  ANALOG_L1,
  ANALOG_L2,
  ANALOG_L3,
  ANALOG_R0,
  ANALOG_R1,
  ANALOG_R2,
  ANALOG_R3,
  ANALOG_IR,
  ANALOG_VOLT, //!< Versorgungsspannung
  ANALOG_TEMP,
  ANALOG_CNT   //!< Anzahl der enums, if(idx < ANALOG_CNT) {/* Index OK */}
};

/*!
 *  Symbolische Namen für den Parameter mode der
 *  analog_getValueExt() - Funktionen
 */
enum {
  ANALOG_PASSIVE       = 0, //!< = 0: passive Messung (LED aus)
  ANALOG_ACTIVE        = 1, //!< = 1: aktive Messung (LED an)
  ANALOG_DIFFERENTIAL  = 2  //!< = 2: aktive - passive Messung (Differenz)
};



/*!
 * Initialisierung des Liniensensors.
 * Der AD-Wandler misst im Interruptmodus automatisch alle Werte, und schaltet
 * auch die IR-LEDs der Bodensensoren bei Bedarf ein und aus, wenn mit analog_setExtToggleMode()
 * der Togglemode für den jeweiligen Eingang aktiviert wurde. 
 * @note Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 */
void analog_init();


/*!
 * Analoge Messungen deaktivieren. Durch einen Aufruf der Funktion analog_reenable() werden die Messungen wieder aktiviert.
 */
uint8_t analog_enable(uint8_t enable);


/*!
 * Liefert den Wert eines analogen Eingangs mit dem index @param idx zurück.
 * @note Die Interrupts müssen aktiviert sein, damit die Werte gemessen werden!
 */
uint16_t analog_getValue(uint8_t idx);


/*!
 * Liefert den Wert eines analogen Eingangs mit dem index @param idx zurück.
 * Der Parameter @param mode bestimmt die Sensor Messwerte Quelle:
 * @ref ANALOG_PASSIVE = bei abgeschalteter Sensor LED, 
 * @ref ANALOG_ACTIVE  = bei eingeschalteter Sensor LED,
 * @ref ANALOG_DIFFERENTIAL = Differenzwert ANALOG_ACTIVE - ANALOG_PASSIVE.
 * Falls die Differenz negativ wäre, wird eine Null zurückgeliefert!
 * Die Werte für ANALOG_ACTIVE und ANALOG_DIFFERENTIAL gelten nur für die Sensorbricks
 * und sind nur gültig, wenn der Toggle-Modus aktiviert ist!
 * @return Value ADC Wert 0..1024
 * 
 * @code
#include <niboburger/robomain.h>
#include <niboburger/analog.h>

// Minimaler Reflektionswert bei Bodenkontakt:
#define BC_MINVAL 40

void setup() {
  // Toggle-Mode für mittleren Bodensensor aktivieren:
  analog_setExtToggleMode(ANALOG_BC, ON);
  // ...
}

void loop() {
  if (analog_has_update()) {
    // wenn neue Messwerte vorhanden sind
    // Differenz Mittlerer Bodensensor bei LED aus / LED an -> reflektierter Anteil
    uint16_t bc = analog_getValueExt(ANALOG_BC, ANALOG_DIFFERENTIAL);
    if (bc < BC_MINVAL) {
      // Aktion bei Absturzgefahr
      engine_stop();
    }
    // ...
  }
}
@endcode
 * @note Die Interrupts müssen aktiviert sein, damit die Werte gemessen werden!
 */
uint16_t analog_getValueExt(uint8_t idx, uint8_t mode);


/*!
 * Liefert einen Zufallswert zurück. Der Wert errechnet sich aus der Summe
 * aller gemessenen Analog-Spannungen. Der Wert selbst sollte nur als Seed
 * (Entropiequelle) verwendet werden, da er monoton wachsend ist. Der Wert
 * wird um so zufälliger, je mehr analoge Messungen vorher durchgeführt
 * wurden.
 * @return Random Seed (16 Bit Zufallswert)
 * 
 * @code
#include <niboburger/robomain.h>
#include <niboburger/analog.h>
#include <stdlib.h>
  
void setup() {
  analog_init();
  // kurz abwarten, damit einige ADC Messungen durchgeführt werden können:
  delay(100); 
  // Startwert (seed) für den Zufallszahlengenerator (Funktion rand()) auf den echt
  // zufälligen Wert (analog_getRandomSeed()) setzen setzen:
  srand(analog_getRandomSeed());
}
 
void loop() {
  // Neue Zufallszahl mit dem Generator erzeugen:
  int zufallszahl = rand();
  // ...
}
@endcode
 * @note Die Interrupts müssen aktiviert sein, damit die Werte gemessen werden!
 */
uint16_t analog_getRandomSeed();


/*!
 * Aktiviert den automatischen Toggle-Mode (active/passive). Dabei wird die automatisch immer wieder ein und 
 * ausgeschaltet. Die Funktion analog_getValueExt() liefert dabei in Abhängigkeit vom Parameter mode 
 * die Werte bei eingeschalteter LED, bei ausgeschalteter LED oder auch die Differenz der beiden Werte zurück.
 * @ref ANALOG_BCL und @ref ANALOG_BCR müssens extra aktiviert werden, alle anderen sind nach dem Start bereits automatisch 
 * im Toggle-Modus.
 */
void analog_setExtToggleMode(uint8_t idx, uint8_t activate);


/*!
 * Diese Funktion wird nach einem kompletten Durchgang aller Sampling-Kanäle. 
 * Als 'weak-linking' Funktion kann sie einfach durch eine eigene Funktion void analog_irq_hook() {} 
 * ersetzt werden.
 * @note Achtung: Funktion wird aus dem IRQ Kontext aufgerufen!
 */
void analog_irq_hook();


/*!
 * Diese Funktion wartet bis ein neuer Satz analoger Werte gemessen wurde.
 */
void analog_wait_update();


/*!
 * Diese Funktion liefert TRUE wenn seit dem letzten Aufruf ein neuer Satz analoger Werte gemessen wurde.
 */
uint8_t analog_has_update();

#ifndef DOXYGEN
extern uint16_t analog_samples[];

#define analog_getValue_ISR(idx) (analog_samples[((idx))])

#define analog_getValueAct_ISR(idx) (analog_samples[((idx))+ANALOG_CNT])
#endif

#define ADC_BANDGAP_CHANNEL_VOLTAGE 1.1


#ifdef __cplusplus
} // extern "C"
#endif


#endif // _NIBOBURGER_ANALOG_H_

