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
  errorSum = errorSum + smoothedError;
  if (errorSum > 4095) errorSum = 4095; // anti-windup set to 2^12

  // now comes PI style update law
  newOutputSignal = Kp * (float)smoothedError + Ki * (float)errorSum;
  
  return newOutputSignal;
}
