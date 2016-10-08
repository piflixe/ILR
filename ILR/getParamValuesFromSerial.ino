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
            initOutput();
            if (rich==true) Serial.println("all values have been resetted");
            returnToMenu();
            break;
          default:
            userInput2 = 0;
            returnToMenu();
            if (rich==true) Serial.println("invalid input in stop parameter field");
        }
        break;

      // SETTING SAMPLE FREQUENCY  
      case 2: // set value for sampleFreq
        menuState = 2;
        if (rich==true) {Serial.println("set value for sample frequency");}
        switch (userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            if (userInput2 > 100000) 
            { 
              if (rich==true) Serial.println("value is too high (System would freeze)"); 
            }
            else
            {
              sampleFreq = (unsigned int)userInput2;
              Timer3.setFrequency(sampleFreq);
              Timer3.start();           // restart Timer since it seems to stop after setPeriod
              if (rich==true)
              {
                Serial.print("sample frequency changed to ");
                Serial.print(sampleFreq);
                Serial.print(" hertz\n");
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
            if (Ki == 999)
            {
              Ki = 0;
            }
            {
              Ki = Ki / 1000;
            }
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
            if (Kp == 999)
            {
              Kp = 0;
            }
            {
              Kp = Kp / 1000;
            }
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
      // SETTING MOVING AVERAGE FILTER WIDTH
      case 6:
        menuState = 6;
        if (rich==true) Serial.println("set width of moving average low pass filter");
        switch(userInput2)
        {
          case 0:
            break; // no second input yet
          default:
            Nsmooth = (float)userInput2;
            if (rich==true)
            {
              Serial.print("Nsmooth changed to ");
              Serial.println(Nsmooth);
            }
            userInput2 = 0;
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
          Serial.print("\n 2: set sample rate - sampleFreq = "); Serial.print(sampleFreq);
          Serial.print("\n 3: set ILC I gain - Ki = "); Serial.print(Ki,3);
          Serial.print("\n 4: set ILC p gain - Kp = "); Serial.print(Kp,3);
          Serial.print("\n 5: set PhaseLead  - PhaseLead = "); Serial.print(PhaseLead);
          Serial.print("\n 6: set moving average filter width (Lowpass) - Nsmooth = "); Serial.print(Nsmooth);
          Serial.print("\n\n actual sample rate is: "); Serial.print(Timer3.getFrequency()); Serial.print("   actual frequency is: "); Serial.print(Timer3.getFrequency()/Nval);
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

