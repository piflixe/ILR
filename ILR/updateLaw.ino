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

float updateLaw(unsigned int i)
{
  float newOutputSignal;
  newOutputSignal = outputSignal[indexShift(i)];
  for (int j=0; j<Nsmooth; j++)
  {
    newOutputSignal = newOutputSignal + SmoothingWeight[j] * error[indexShift(i + j)];
  }
  return newOutputSignal;
}
