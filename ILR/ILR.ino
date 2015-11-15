/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 15.11.15

*/

#define DEBUGPRINT(x) if(debug==true) Serial.println( x );    // short call to debug messages

// including libraries
// #include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// DECLARING CONSTANTS -----------------------------------------
// for PINs
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
const unsigned int PIN_HARDWAREDEBUG = 53;

// constants for linear phase lead ILC
const unsigned int Nsmooth = 3;                                // number of values used as smoothing in update law
const float SmoothingWeight[Nsmooth] = {0.1 , 0.1 , 0.1};       // average weighting for smoothing used in update law
const int PhaseLead = 4;                                       // discrete Phase Lead for digital smoothing in update law

// DECLARING VARIABLES -----------------------------------------
// core ILC
// volatile unsigned int inputSignal[Nval]; // array of values read from ADC
float outputSignal[Nval];                  // array of values to be written on DAC
int error[Nval];                           // array of values with errors (desired value - real value)
volatile unsigned int timeIndex = 0;                  // index to be incremented by ISR

// debugging variables
unsigned int badness = 0;                // sum of all quadratic errors for measuring overall control performance (debug only)
const boolean debug = false;             // debugging with Serial Console (works only for very low Tsmic)
const boolean hardwareDebug = false;     // debugging with measuring certain timings via digital i/o PINs

void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changeIndex).start(Tsmic);
  
  // using Serial Interface for debugging
  Serial.begin(115200);

  // configuring PINs
  pinMode(PIN_ADC, INPUT);
  pinMode(PIN_DAC, OUTPUT);
  pinMode(PIN_HARDWAREDEBUG, OUTPUT);
  digitalWrite(PIN_HARDWAREDEBUG, LOW);
  
  DEBUGPRINT("gelesene Werte:")
  // initialise variables
  
  for (int j = 0; j < Nval; j++)
  {
    // setting ADC and error values to 0
    // inputSignal[j] = 0;
    error[j] = 0;
    // setting DAC values to data table stored in progmem
    outputSignal[j] = table[j]; //pgm_read_word(&table[j]);

    // printing outputSignal on Serial Console for debugging
    DEBUGPRINT(outputSignal[j]);
  }
  
  if(debug==true) delay(1000);

}

void loop() {
  /*
  // checking of Period is complete
  if (flagPeriodComplete == true)
  {
    swapIntPointers(&error, &errorCalc); // swapping error Pointers
    flagPeriodComplete = false;     // clearing flag
    
    if(hardwareDebug==true){
      // DEBUGPRINT("Setting HardwareDebug-PIN to HIGH")
      analogWrite(PIN_HARDWAREDEBUG, HIGH); // marking beginning of calculation on debug PIN
    }
    // applying update law
    int neighbour_left = 0;
    int neighbour_right = 0;
    for (int j = 0; j < Nval; j++)
    {
      if (j == 0) // first value
      {
        neighbour_left = Nval-1; // rollover left
        neighbour_right = j+1;
      }
      else if (j == Nval-1) // last value
      {
        neighbour_left = j-1;
        neighbour_right = 0; // rollover right
      }
      else // some values in the middle 
      {
        neighbour_left = j-1;
        neighbour_right = j+1;
      }
      
      outputSignalCalc[j] = outputSignal[j] + ( (float)(errorCalc[neighbour_right]) )*beta;

      // cap values to max / min
      if (outputSignalCalc[j] > 4095)
      {
        outputSignalCalc[j] = 4095;
        DEBUGPRINT("oberhalb Regelbereich")
      }
      else if (outputSignalCalc[j] < 0)
      {
        outputSignalCalc[j] = 0;
        DEBUGPRINT("unterhalb Regelbereich")
      }
    }
    
    if(hardwareDebug==true){
     //DEBUGPRINT("Re-Setting HardwareDebug-PIN to LOW")
     // delay();
      analogWrite(PIN_HARDWAREDEBUG, LOW); // marking end of calculation on debug PIN
    }
    
    flagCalculationReady = true;
    
    // printing debug messages for analog in and analog out
    if(debug==true){
      Serial.println("analoger Eingang:");
      for(int j=0; j<Nval; j++){
        Serial.print(inputSignal[j]); Serial.print(" ");  
      }
      Serial.print("\n");
      Serial.println("analoger Ausgang:");
      for(int j=0; j<Nval; j++){
        Serial.print((int)outputSignal[j]); Serial.print(" ");  
      }
      Serial.print("\n");
    }  
  }
  */
}

void changeIndex() {
  // setting DAC value
  analogWrite(PIN_DAC, ((int)outputSignal[timeIndex]));

  // measuring current value on ADC
  //DEBUGPRINT(i);
  error[timeIndex] = table[timeIndex] - analogRead(PIN_ADC);
  
  if(debug==true){ // calculating badness and printing error only in debug mode
    badness = badness + error[timeIndex]*error[timeIndex]; // incrementing badness by quadratic error
//    if(timeIndex==0) Serial.print("Fehler: ");
//    char errorBuffer[6];
//    sprintf(errorBuffer, " %4i", error[timeIndex]);
//    Serial.print(errorBuffer);
//    if(timeIndex==(Nval-1)) Serial.print("\n");    // new line at end of period
  }
 
  timeIndex = timeIndex + 1;  // incrementing i (time axis)
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0;       // resetting i for next period

    // debugging code
    badness = 0; // resetting badness for next period  
    DEBUGPRINT("Badness:")
    DEBUGPRINT(badness)
  }
}


/*
// Index Shift in a FIFO ring buffer manner
unsigned int indexShift(i)
{
  unsigned int shiftedIndex;
  if (i<

}
*/
