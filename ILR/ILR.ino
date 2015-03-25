/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 21.3.15

*/

#define DEBUGPRINT(x) if(debug==true) Serial.println( x );

// including libraries
// #include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// declaring constants
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
float alpha = 0.05;
float beta = 0.00;

// declaring variables
volatile unsigned int inputSignal[Nval]; // array of values read from ADC
float outputSignalStorageA[Nval]; // array of values to be written on DAC
float outputSignalStorageB[Nval]; // array of values to be written on DAC
int errorStorageA[Nval];  // array of values with errors (desired value - real value)
int errorStorageB[Nval];
unsigned int badness = 0;          // sum of all quadratic errors for measuring overall control performance
volatile unsigned int i = 0; // index to be incremented by ISR

// pointer for current output signal and error
float *outputSignal;
float *outputSignalCalc;
int *error;
int *errorCalc;

// switch for A/B selection (true -> A, false -> B)
boolean switchAB = true;

// flag to be set when new values for DAC are ready
volatile boolean flagCalculationReady = false;

// flag to be set when a periode of the signal is completed
volatile boolean flagPeriodComplete = false;

const boolean debug=true;

void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changei).start(Tsmic);
  
  // using Serial Interface for debugging
  Serial.begin(115200);

  if(debug==true) Serial.println("gelesene Werte:");
  // initialise variables
  
  for (int j = 0; j < Nval; j++)
  {
    // setting ADC and error values to 0
    inputSignal[j] = 0;
    errorStorageA[j] = 0;
    errorStorageB[j] = 0;
    // setting DAC values to data table stored in progmem
    outputSignalStorageA[j] = table[j]; //pgm_read_word(&table[j]);
    outputSignalStorageB[j] = 0;
    
    if(debug==true) Serial.println(outputSignalStorageA[j]);
  }
  // setting initial stats for pointers
  outputSignal = outputSignalStorageA;
  outputSignalCalc = outputSignalStorageB;
  error = errorStorageA;
  errorCalc = errorStorageB;
 
  if(debug==true) delay(1000);

}

void loop() {
  // checking of Period is complete
  if (flagPeriodComplete == true)
  {
    swapIntPointers(&error, &errorCalc); // swapping error Pointers
    flagPeriodComplete = false;     // clearing flag

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
      
      outputSignalCalc[j]=outputSignal[j] + ((float)error[j])*alpha + ( (float)(error[neighbour_right] + error[neighbour_left]) )*beta;

      // cap values to max / min
      if (outputSignalCalc[j]>4095)
      {
        outputSignalCalc[j] = 4095;
        DEBUGPRINT("oberhalb Regelbereich")
      }
      else if (outputSignalCalc[j] < 0)
      {
        outputSignalCalc[j] = 0;
        if(debug==true)
        Serial.println("unterhalb Regelbereich");
      }
    }
    flagCalculationReady=true;
  }
}

void changei() {
  // setting DAC value
  analogWrite(PIN_DAC, ((int)outputSignal[i]));

  // setting ADC value
  inputSignal[i] = analogRead(PIN_ADC);
  error[i] = table[i] - inputSignal[i];
  if(debug==true){
    badness = badness + error[i]*error[i]; // incrementing badness by quadratic error
    if(i==0) Serial.print("Fehler: ");
    char errorBuffer[6];
    sprintf(errorBuffer, " %4i", error[i]);
    Serial.print(errorBuffer);
    if(i==(Nval-1)) Serial.print("\n");
  }
 
  i = i + 1;  // incrementing i (time axis)
  if (i >= Nval) // checking if Period is complete
  {
    if(debug==true) {
      Serial.println("Badness:");
      Serial.println(badness);
    }
    badness = 0; // resetting badness for next period  
    i = 0;             // resetting i
    flagPeriodComplete = true; // setting flag
    if (flagCalculationReady==true)
    {
      swapFloatPointers(&outputSignal, &outputSignalCalc);
      flagCalculationReady=false;
    }
  }
}

void swapIntPointers(int** pointer1,int** pointer2) {
 /* if(debug==true) {
    Serial.println("Methode swapIntPointers wird aufgerufen...");
    Serial.println("alte Adressen: ");
    Serial.println((int)*pointer1,HEX);
    Serial.println((int)*pointer2,HEX);
  }*/
  
  int *tempPointer; // used for swapping pointers 
  tempPointer=*pointer1; // saving address of pointer 1
  *pointer1=*pointer2;    // swapping addresses of pointers
  *pointer2=tempPointer;

  /* if(debug==true) {
    Serial.println("neue Adressen:");
    Serial.println((int)*pointer1,HEX);
    Serial.println((int)*pointer2,HEX);
  } */
}


void swapFloatPointers(float** pointer1, float** pointer2) {
  /*if(debug==true) {
    Serial.println("swapFloatPointers...");
    Serial.println("alte Adressen:");
    Serial.println((int)*pointer1,HEX);
    Serial.println((int)*pointer2,HEX);
  }*/
  
  float *tempPointer; // used for swapping pointers 
  tempPointer=*pointer1; // saving pointer 1
  *pointer1=*pointer2;    
  *pointer2=tempPointer;
  
  /* if(debug==true) {
    Serial.println("neue Adressen:");
    Serial.println((int)*pointer1,HEX);
    Serial.println((int)*pointer2,HEX);
  } */
}
