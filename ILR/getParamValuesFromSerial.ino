void getParamValuesFromSerial(boolean rich)
{
  int userInput1;
  int userInput2; 
  if (stringComplete) {
    stringComplete = false;
    userInput1 = atoi(inputString1.c_str()); // convert String from Serial to int
    userInput2 = atoi(inputString2.c_str());

    switch(userInput1)
    { 
      // START / STOP MENU
      case 1: // set value for stop variable
        menuState = 1;
        if (rich==true) Serial.println("start [1], pause [2] or reset [3] experiment");
        switch (userInput2)
        {
          case 0:
            break; // no second input yet
          case 1:
            Stop = false;
            if (rich==true) Serial.println("starting Experiment...");
            returnToMenu();
            break;
          case 2:
            Stop = true;
            if (rich==true) Serial.println("pausing Experiment...");
            returnToMenu();
            break;
          case 3:
            Stop = true;
            if (rich==true) Serial.println("reseting Experiment...");
            for (int j = 0; j<Nval; j++) // MOVE THIS CODE IN A SETUP FUNCTION LATER!
            {
              error[j] = 0;               // initialising error values
              outputSignal[j] = table[j]; // setting DAC values to data table stored in progmem
            }
            returnToMenu();
            break;
          default:
            userInput2 = 0;
            returnToMenu();
            if (rich==true) Serial.println("invalid input in stop parameter field");
        }
        break;

      // SETTING SAMPLE FREQUENCY  
      case 2: // set value for Tsmic
        menuState = 2;
        if (rich==true) {Serial.println("set value for sample Intervall");}
        switch (userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            if (userInput2 < 30) 
            { 
              if (rich==true) Serial.println("value is too low (System would freeze)"); 
            }
            else
            {
              Tsmic = (unsigned int)userInput2;
              Timer3.attachInterrupt(changeIndex).start(Tsmic); // reattach interrupt with new value
              if (rich==true)
              {
                Serial.print("sample rate changed to ");
                Serial.print(Tsmic);
                Serial.print(" micro seconds\n");
              }
            }
            returnToMenu();
            userInput2 = 0;
            break;
        }
        break;

      // SETTING ILC GAIN  
      case 3:
        menuState = 3;
        if (rich==true) Serial.println("set value for control gain (times 1000, type 1 for 0.001)");
        switch(userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            ILCgain = (float)userInput2;
            ILCgain = ILCgain / 1000;
            if (rich==true)
            {
              Serial.print("ILC gain changed to ");
              Serial.print(ILCgain,3);
              Serial.print("\n");
            }
            userInput2 = 0;
            returnToMenu();
            break;
        }
        break;

      // SETTING PHASE LEAD  
      case 4:
        menuState = 4;
        if (rich==true) Serial.println("set value for PhaseLead of ILC");
        switch(userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            PhaseLead = (int)userInput2;
            if (rich==true)
            {
              Serial.print("ILC Phase Lead changed to ");
              Serial.print(PhaseLead);
              Serial.print(" time steps\n");
            }
            returnToMenu();
            break;
        }
        break;

      // PRINT MENU IF SOMETHING WEIRD WAS ENTERED  
      default: 
        returnToMenu();   
        if (rich==true) 
        { 
          Serial.println("invalid input in menu");
          Serial.print(" \
            \n usage: \
            \n 1: Start / Stop / Reset \
            \n 2: set sample rate \
            \n 3: set ILC gain \
            \n 4: set PhaseLead (digital LowPass equivalent) \n \
            ");
        }
        break;
    } 
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (menuState==0) // base menu
    { 
      inputString1 += inChar; // add it to the inputString1:
    }
    else  // parameter setting
    {
      inputString2 +=inChar;
    }
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void returnToMenu() {
  menuState = 0; // return to main menu
  inputString1 = ""; // clear the Serial communication string
  inputString2 = ""; 
}

