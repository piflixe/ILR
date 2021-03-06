float updateLaw(unsigned int i)
/* 
 * ILC update law 
 * derived from Longman:
 * Iterative learning control and repitive control for engineering practice
 * extended to form a PI-style update law (coefficients Kp and Ki)
 * 
 * input: current time index
 * returns: computed output signal
 */
{
  float newOutputSignal;
  int smoothedError = 0;

  // smoothing error values
  for (unsigned int j=0; j<Nsmooth; j++)
  {
    smoothedError = smoothedError + error[indexShift(i + j)];
  } 
  smoothedError = smoothedError / Nsmooth;
  errorSum[i] = errorSum[i] + smoothedError;

  // applyings PI style update law
  newOutputSignal = 2048.0 + Kp * (float)smoothedError + Ki * (float)errorSum[i];

  if (newOutputSignal > 4095) // Signal runs into upper limit of DAC
  {
    newOutputSignal = 4095;  
    // DEBUGPRINT("Signal on upper limit of DAC");
    digitalWrite(PIN_DAC_OVERRANGE, HIGH); 
  }
  if (newOutputSignal < 0)
  {
    newOutputSignal = 0;      // Signal runs into lower limit of DAC
    // DEBUGPRINT("Sinal on lower limit of DAC");
    digitalWrite(PIN_DAC_OVERRANGE, HIGH);
  }
    
  return newOutputSignal;
}
