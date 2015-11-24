void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changeIndex).start(Tsmic);
  
  // using Serial Interface for debugging
  Serial.begin(115200);  
  inputString1.reserve(3); // reserve some bytes for the inputString
  inputString2.reserve(3); //

  // configuring PINs
  pinMode(PIN_ADC, INPUT);
  pinMode(PIN_DAC, OUTPUT);
  analogWrite(PIN_DAC, 4094/2);
  pinMode(PIN_HARDWAREDEBUG, OUTPUT);
  digitalWrite(PIN_HARDWAREDEBUG, LOW);
  
  DEBUGPRINT("gelesene Werte:")
  // initialise variables

  // initialise error and outputSignal 
  for (int j = 0; j < Nval; j++)
  {
    error[j] = 0;                     // setting ADC and error values to 0
    outputSignal[j] = table[j];       // setting DAC values to data table stored in progmem
    //pgm_read_word(&table[j]);

    // printing outputSignal on Serial Console for debugging
    DEBUGPRINT(outputSignal[j]);
  }

  for (int j = 0; j < Nsmooth; j++)
  {
    SmoothingWeight[j] = ILCgain/Nsmooth;
    if(Nsmooth>6)  // apply linear window
    {
      if ( (j==0) || (j==Nsmooth-1) ) SmoothingWeight[j] = SmoothingWeight[j] / 2;
    }
  }
  if(debug==true) delay(1000);
}
