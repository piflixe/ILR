void changeIndex() {
  error[timeIndex] = table[timeIndex] - analogRead(PIN_ADC);

  outputSignal[timeIndex] = 2048 + updateLaw(timeIndex); //applaying update law

  analogWrite(PIN_DAC, (int)outputSignal[timeIndex]); // setting DAC value


  timeIndex = timeIndex + 1;
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0; 
    //Serial.println(errorSum[timeIndex]);
  }
}

