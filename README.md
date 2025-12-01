# Line-following-bot-
I participated in line follower bot competition hosted by the build club. 
The goal was to develop a line-following robot that accurately navigates the track while adhering to all specified rules and parameters.

Major Components used:

->ESP 32 WROOM

->TB6612FNG Motor driver

->QTR 8A IR sensor array

->N20 6v 600 rpm motor

->Lithium polymer Battery

The ESP32 acts as the brain of the system, reading real-time data from the QTR-8A IR sensor array to detect the path. A PID (Proportional-Integral-Derivative) controller is implemented to calculate the positional error and dynamically adjust the motor speeds, enabling smoother and more accurate line following. The TB6612FNG motor driver receives PWM signals from the ESP32 for precise speed and direction control of the N20 motors. Power is supplied via a LiPo battery, with voltage protection in place for the ESP32's analog inputs.
