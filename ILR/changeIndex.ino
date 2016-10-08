void changeIndex() {
  int ADCvalue;
  //if(hardwareDebug==true){ digitalWrite(PIN_HARDWAREDEBUG,HIGH); }
  ADCvalue = analogRead(PIN_ADC);
  if(ADCvalue < 100) digitalWrite(PIN_ADC_OVERRANGE,HIGH);
  if(ADCvalue > 4000) digitalWrite(PIN_ADC_OVERRANGE,HIGH);
  
  error[timeIndex] = table[timeIndex]- ADCvalue;

  outputSignal[timeIndex] = updateLaw(timeIndex); //applaying update law

  analogWrite(PIN_DAC, (int)outputSignal[timeIndex]); // setting DAC value

  timeIndex = timeIndex + 1;
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0; 
  }
  //if(hardwareDebug==true){ digitalWrite(PIN_HARDWAREDEBUG,LOW); }  
}

