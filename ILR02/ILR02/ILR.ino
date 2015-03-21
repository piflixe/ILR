/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 9.3.15

Neu in 02:
- DAC-Werte als float - deshalb muss auch 
  error als float, nur ADC bleibt unsigned int
- alpha und beta sind floats, werden multiplikativ angewendet
- bugfix in update-Regel: Fall für j=Nval hat falschen Wert 
  gelesen, musste j-1 sein!
- Abfrage ob valDAC im Wertebereich liegt. Ansonsten zu Null oder 4095 setzen

Version 01:
- first running

*/

// including libraries
// #include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// declaring constants
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
float alpha = 0.05;
float beta = 0.01;

// declaring variables
volatile unsigned int valADC[Nval]; // array of values read from ADC
volatile float valDAC[Nval]; // array of values to be written on DAC
volatile float valDACn[Nval];// new DAC array
int error[Nval];  // array of values with errors (desired value - real value)

unsigned int i = 0;

// flag to be set when new values for DAC are ready
boolean flagReady = false;

// flag to be set when a periode of the signal is completed
volatile boolean flagPeriod = false;

boolean debug=false;

void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changei).start(Tsmic);
  
  // using Serial Interface for debugging
  Serial.begin(115200);

  if(debug==true){Serial.println("gelesene Werte:");}
  // initialise variables
  
  for (int j = 0; j < Nval; j++)
  {
    // setting ADC and error values to 0
    valADC[j] = 0;
    error[j] = 0;
    // setting DAC values to data table stored in progmem
    valDAC[j] = 0; //pgm_read_word(&table[j]);
    if(debug==true){Serial.println(valDAC[j]);}
  }
  if(debug==true){delay(1000);}

}

void loop() {
  // checking of Period is complete
  if (flagPeriod == true)
  {
    // clearing flag
    flagPeriod = false;

    // calculation error
    if(debug==true){Serial.println("Fehler:");}
    for (int j = 0; j < Nval; j++)
    {
      error[j] = table[j] - valADC[j];
      if(debug==true){Serial.println(error[j]);}
    }
    if(debug==true){delay(300);}

    // applying update law
    for (int j = 0; j < Nval; j++)
    {
      if (j == 0) // first value
      {
        valDACn[j]=valDAC[j] + ((float)error[j])*alpha + (  (float)(error[j+1] + error[Nval-1])  )*beta;
      }
      else if (j == Nval-1) // last value
      {
        valDACn[j]=valDAC[j] + (float)error[j]*alpha + ((float)error[0] + (float)error[j-1])*beta;
      }
      else // some values in the middle 
      {
        valDACn[j]=valDAC[j] + (float)error[j]*alpha + ((float)error[j+1] + (float)error[j-1])*beta;
      }
      
      if (valDACn[j]>4095)
      {
        valDACn[j] = 4095;
        if(debug==true){Serial.println("oberhalb Regelbereich");}
      }
      else if (valDACn[j] < 0)
      {
        valDACn[j] = 0;
        if(debug==true){Serial.println("unterhalb Regelbereich");}
      }
    }
    
    // setting old to new values
    if(debug==true){Serial.println("neue DAC Werte:");}
    for (int j = 0; j<Nval; j++)
    {
      valDAC[j]=valDACn[j];
      if(debug==true){Serial.println(valDAC[j]);}
    }
    if(debug==true){delay(300);}
  }
}

void changei() {
  // setting DAC value
  analogWrite(PIN_DAC, (int)valDAC[i]);

  // setting ADC value
  valADC[i] = analogRead(PIN_ADC);

  // incrementing i
  i = i + 1;
  if (i >= Nval) { // checking if Period is complete
    i = 0;       // resetting i
    flagPeriod = true; // setting flag
  }
}
