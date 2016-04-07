/*
  Arduino Due Sketch for ILC

  Author: Felix Piela
  Feb 2016
*/

/* DEFINES */
// short call to debug messages
#define DEBUGPRINT(x) if(debug==true) Serial.println( x );    

/* INCLUDING LIBRARIES */
// #include <avr/pgmspace.h> // accessing program memory
#include "DueTimer.h"
#include <stdio.h>
#include <stdlib.h>

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// DECLARING CONSTANTS -----------------------------------------
// for PINs
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
const unsigned int PIN_HARDWAREDEBUG = 22;
const unsigned int PIN_ADC_OVERRANGE = 52;
const unsigned int PIN_DAC_OVERRANGE = 53;

// DECLARING VARIABLES -----------------------------------------
// core ILC
int Nsmooth = 6;                           // number of values used as smoothing in update law (must be smaller than NsmoothMax
float Ki = 0.001;                          // I gain of ILC
long errorSum[Nval];                       // error sum for I part of control
float Kp = 0.000;                          // P gain of ILC
int PhaseLead = 4;                         // discrete Phase Lead for digital smoothing in update law (must be in the range of [1:Nval-Nsmooth]
boolean Stop = true;                       // start and stop the experiment

float outputSignal[Nval];                  // array of values to be written on DAC
int error[Nval];                           // array of values with errors (desired value - real value)
volatile unsigned int timeIndex = 0;       // index to be incremented by ISR

// Variables for Serial Communication
String inputString1 = "";                  // a string to hold incoming data for menu
String inputString2 = "";                  // a string to hold incoming data for menu
boolean stringComplete = false;            // whether the string is complete
int menuState = 0;


// debugging variables
unsigned int badness = 0;                // sum of all quadratic errors for measuring overall control performance (debug only)
const boolean debug = false;             // debugging with Serial Console (works only for very high Tsmic -> low frequency)
const boolean hardwareDebug = false;     // debugging with measuring certain timings via digital i/o PINs
unsigned long slowEventTime = 0;

void loop() {
  getParamValuesFromSerial(true);        // use with true for verbose output

  if ((millis() - slowEventTime) > 1000)
  {
    slowEventTime = millis();             // reset overrange indicator PINs after one second
    digitalWrite(PIN_DAC_OVERRANGE, LOW);
    digitalWrite(PIN_ADC_OVERRANGE, LOW);
  }

  delay(10);
}


