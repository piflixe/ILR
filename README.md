# ILR
iterative learning control on Arduino Due

This work was started for a master thesis at the Technical University of Berlin.
The aim of this work is to establish a closed loop control for the excitation of mechanical test setups.

## Problem description:
When performing mechanical oscillating tests on non-linear structures, it is very hard to establish a 
sober excitation force of only one single frequency.

"Normal" computer-controlled measuring eqipment is not able to adjust the output very fast since information is buffered and the adjustment of the real-time module is very limited.

## Idea:
Usage of iterative learning control to improve the signal used to excite the mechanical non-linear 
structure from period to period.

The control is implemented on an ArduinoDue with high real-time standards. The Hardware is cheap and real-time behaviour can be examined and influenced. If needed, high-performance DACs or ADCs can be added  

# Usage
Copy all files in the ILR subfolder to your computer. Open ILR.ino with the Arduino IDE. Flash on an Arduino Due or compatible device.

The devices starts up in waiting mode and waits for commands on the serial interface. Just hitting enter on the serial will display help messages and the menu navigation. Parameters can be set and the experiment can be started / stopped / resetted.

In order to realise more complex tasks like a sweep experiment, external scripts can be used that send messages to the device on serial interface. I used simple matlab script for that. See SweepExperimentControl.m for more information.

Please note: An external electrical circuit might be needed to use the code on real measurement equipment. Please also be sure to have the right voltage levels. A IEPE compatible example can be found in the IEPE_Circuit subfolder.

# Documentation
Full documentation is in my master thesis which can be found in main.pdf
