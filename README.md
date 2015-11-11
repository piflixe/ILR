# ILR
iterative learning control on Arduino Due

This work was started for a master thesis at the Technical University of Berlin.
The aim of this work is to establish a closed loop control for the excitation of mechanical test setups.

Problem description:
When performing mechanical oscillating tests on non-linear structures, it is very hard to establish a 
sober excitation force of only one single frequency.

"Normal" computer-controlled measuring eqipment is not able to adjust the output very fast since information is buffered and the adjustment of the real-time module is very limited.

Idea:
Usage of iterative learning control to improve the signal used to excite the mechanical non-linear 
structure from period to period.

The control is implemented on an ArduinoDue with high real-time standards. The Hardware is cheap and real-time behaviour can be examined and influenced. If needed, high-performance DACs or ADCs can be added  
