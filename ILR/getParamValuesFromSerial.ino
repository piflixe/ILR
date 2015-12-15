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
            Timer3.start(); // starting ISR on internal interrupt
            if (rich==true) Serial.println("starting Experiment...");
            returnToMenu();
            break;
          case 2:
            Timer3.stop(); // stopping ISR
            if (rich==true) Serial.println("pausing Experiment...");
            returnToMenu();
            break;
          case 3:
            Timer3.stop();
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
        if (rich==true) Serial.println("set value for I control gain (times 1000, type 1 for 0.001)");
        switch(userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            Ki = (float)userInput2;
            Ki = Ki / 1000;
            if (rich==true)
            {
              Serial.print("Ki changed to ");
              Serial.print(Ki,3);
              Serial.print("\n");
            }
            userInput2 = 0;
            returnToMenu();
            break;
        }
        break;
    
      case 4:
        menuState = 4;
        if (rich==true) Serial.println("set value for P control gain (times 1000, type 1 for 0.001)");
        switch(userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            Kp = (float)userInput2;
            Kp = Kp / 1000;
            if (rich==true)
            {
              Serial.print("Kp changed to ");
              Serial.print(Kp,3);
              Serial.print("\n");
            }
            userInput2 = 0;
            returnToMenu();
            break;
        }
        break;
      // SETTING PHASE LEAD  
      case 5:
        menuState = 5;
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
          Serial.print("\nusage:");
          Serial.print("\n 1: Start / Stop / Reset");
          Serial.print("\n 2: set sample rate - Tsmic = "); Serial.print(Tsmic);
          Serial.print("\n 3: set ILC I gain - Ki = "); Serial.print(Ki,3);
          Serial.print("\n 4: set ILC p gain - Kp = "); Serial.print(Kp,3);
          Serial.print("\n 5: set PhaseLead (digital LowPass equivalent) - PhaseLead = "); Serial.print(PhaseLead);
          Serial.print("\n");
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

