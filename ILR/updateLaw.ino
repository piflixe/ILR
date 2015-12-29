float updateLaw(unsigned int i)
/* 
 * ILC update law 
 * derived from Longman:
 * Iterative learning control and repitive control for engineering practice
 * 
 * input: current time index
 * returns: computed output signal
 * 
 * usage: outputSignal[shiftedTimeIndex] = updateLaw(currentTimeIndex)
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
  if (errorSum[i] > iWindup)  {errorSum[i] = iWindup;}  // anti-windup set to 2^12
  if (errorSum[i] < -iWindup) {errorSum[i] = -iWindup;} // negative anti windup
  
  // now comes PI style update law
  newOutputSignal = Kp * (float)smoothedError + Ki * (float)errorSum[i];

  if (newOutputsignal > 4095) // Signal runs into upper limit of DAC
    newOutputSignal = 4095;   
  if (newOutputsignal < 0)
    newOutputSingal = 0;      // Signal runs into lower limit of DAC
  
  return newOutputSignal;
}
