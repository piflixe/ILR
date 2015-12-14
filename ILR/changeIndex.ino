void changeIndex() {
  analogWrite(PIN_DAC, (int)outputSignal[timeIndex]); // setting DAC value
  error[timeIndex] = table[timeIndex] - analogRead(PIN_ADC);

  outputSignal[indexShift(timeIndex)] = updateLaw(timeIndex); //applaying update law

  timeIndex = timeIndex + 1;
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0;
  }
}

