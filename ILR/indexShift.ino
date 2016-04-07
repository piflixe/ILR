unsigned int indexShift(unsigned int i)
/*
  Index Shift in a FIFO ring buffer manner
  input is current index
  returns shifted index
*/

{
  unsigned int shiftedIndex;
  if (i < (Nval - PhaseLead))         // rollover   
    { shiftedIndex = i + PhaseLead; }
  else                                // no rollover needed 
    { shiftedIndex = i - (Nval - PhaseLead); }
        
  return shiftedIndex;
}
