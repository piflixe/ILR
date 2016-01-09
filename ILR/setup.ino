void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.setFrequency(sampleFreq);
  Timer3.attachInterrupt(changeIndex).stop(); // init Timer in stop condition
  
  // using Serial Interface for debugging
  Serial.begin(115200);  
  Serial.println("Arduino is up and running");
  inputString1.reserve(3); // reserve some bytes for the inputString
  inputString2.reserve(3); //

  // configuring PINs
  pinMode(PIN_ADC, INPUT);
  pinMode(PIN_DAC, OUTPUT);
  pinMode(PIN_HARDWAREDEBUG, OUTPUT);
  digitalWrite(PIN_HARDWAREDEBUG, LOW);
  pinMode(PIN_ADC_OVERRANGE, OUTPUT);
  digitalWrite(PIN_ADC_OVERRANGE, LOW);
  pinMode(PIN_DAC_OVERRANGE, OUTPUT);
  digitalWrite(PIN_DAC_OVERRANGE, LOW);
  
  DEBUGPRINT("gelesene Werte:")
  // initialise variables

  // initialise error and outputSignal 
  initOutput();

  if(debug==true) delay(1000);
}

void initOutput()
{
  for (int j = 0; j<Nval; j++)  // MOVE THIS CODE IN A SETUP FUNCTION LATER!
  {
    error[j] = 0;               // initialising error values
    errorSum[j] = 0;            // resetting I sum
    outputSignal[j] = table[j]; // setting DAC values to data table
                                // use pgm_read_word(&table[j]) if table is stored in PROGMEN
    analogWrite(PIN_DAC, 2048); // Write mean value to DAC so that physics can settle

    // printing outputSignal on Serial Console for debugging
    DEBUGPRINT(outputSignal[j]);
  }  
}

