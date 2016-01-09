void changeIndex() {
  int ADCvalue;
  ADCvalue = analogRead(PIN_ADC);
  if(ADCvalue < 100) digitalWrite(PIN_ADC_OVERRANGE,HIGH);
  if(ADCvalue > 4000) digitalWrite(PIN_ADC_OVERRANGE,HIGH);
  
  error[timeIndex] = table[timeIndex] - ADCvalue;

  outputSignal[timeIndex] = updateLaw(timeIndex); //applaying update law

  analogWrite(PIN_DAC, (int)outputSignal[timeIndex]); // setting DAC value


  timeIndex = timeIndex + 1;
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0; 
    //Serial.println(errorSum[timeIndex]);
  }
}

