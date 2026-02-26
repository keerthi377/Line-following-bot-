eYRC Crop Drop Bot  - Task6 

Team ID: eYRC#1173 

Authors: Gowri Shankara Narayan, Joshua A, Keerthi Sheevani G, Nidhi Krishna D

OVERVIEW

This project implements line-following robot using Reinforcement Learning (Q-learning) integrated with STM32 HAL drivers.
The objective of this task is to develop a line-following algorithm (using Reinforcement Learning) with support for line switching, and to demonstrate a complete pick-and-place mechanism within the given arena.
The robot performs the following tasks:

•	Starts from the designated starting node.

•	Follows the line using 5 IR sensors.

•	Supports line switching (white-on-black and black-on-white tracks).

•	Detects boxes placed according to the configuration file.

•	Identifies the color of the detected box using a color sensor.

•	Indicates the detected color appropriately.

•	Picks up boxes using an electromagnet mechanism.

•	Maintains priority order of pickup:

1.	Red
2.	Blue
3.	Green
   
•	Navigates to the appropriate drop zone based on box color.

•	Places the box in the correct drop location.

•	Repeats the cycle until all boxes are delivered.

•	After placing the final box, traverses back to the starting node to indicate completion of the run.

SOFTWARE ARCHITECTURE

LINE DETECTION

Function: get_state_from_adc()
This function converts raw sensor readings into a compact 5-bit encoded state representing the robot’s position relative to the line.

Working Principle:

•	Reads 5 ADC values from line sensors.

•	Applies threshold comparison to convert analog values into binary (0 or 1).

•	Encodes the 5 binary values into a single 5-bit state.


Bit Mapping:

•	Bit4 → Left Corner (LC)
•	Bit3 → Left (L)
•	Bit2 → Middle (M)
•	Bit1 → Right (R)
•	Bit0 → Right Corner (RC)

If the track color is flipped, the state is inverted using:
state = 31 - state

This enables seamless switching between white-on-black and black-on-white tracks without changing the Q-table.

REINFORCEMENT LEARNING ACTION SELECTION

Function: choose_action(uint8_t state)

This function implements action selection using a pre-trained Q-table.

Working Principle:

•	Takes the current 5-bit state as input.

•	Reads Q-values corresponding to that state.

•	Selects the action with the maximum Q-value.

•	Returns the index of the best action.

Action Mapping:

0 → Sharp Left
1 → Forward
2 → Sharp Right
3 → Slight Left
4 → Slight Right
5 → Stop

This ensures optimal movement decisions based on learned policy.

MOTOR CONTROL

Function: set_motor(int left, int right)

This function converts high-level motion commands into motor control signals.

Behavior:

•	Positive values → Forward rotation
•	Negative values → Reverse rotation
•	Separate values are applied to left and right motors.
To compensate for hardware mismatch between motors, the right motor is slightly scaled:
right * 11 / 10

Speed Scaling:

Motor speeds are globally scaled using:

#define SCALE 90
This allows overall speed tuning without modifying individual action values.
Action Execution

Function: execute_action(uint8_t action)

This function maps action indices to motor commands.

Behavior:

•	Each action corresponds to a predefined motor configuration.

•	Sharp turns include small delay intervals for stable rotation.

•	Stop action sets both motor speeds to zero.

This creates smooth and stable navigation behavior.

OPERATING MODES

The robot operates using a structured multi-mode state machine.

MODE1 - PICKUP MODE

Track Type: White line on black background

Responsibilities:

•	Follow the line using Q-learning.

•	Detect boxes using IR sensing.

•	Perform stable color detection.

•	Pick box only if it satisfies priority condition:
box_picked == box_dropped + 1

Right-Side Box Handling:

If a box is placed on the right side:

•	Robot reaches end node (state == 0).

•	End detection is confirmed using black_count.

•	Performs a 180-degree in-place rotation.

•	Returns along the path to access the right-side box.

Mode switches to Mode 2 when track color flips.

MODE 2 - DROP MODE

Track Type: Black line on white background

Responsibilities:

•	Navigate to correct drop node based on box color.

•	Perform direction-based turning at nodes.

•	Stop at drop location.

•	Release box.

•	Increment box_dropped.

•	Transition to Mode 3.

MODE 3 - RETURN MODE

Responsibilities:

•	Return from drop location to the main track.

•	Detect track color flip.

•	Reset navigation parameters.

•	Switch back to Mode 1.

After the final box is dropped, the robot returns to the starting node to indicate completion of the run.

Box Priority Logic:

Boxes must be picked strictly in the following order:

1.	Red
2.	Blue
3.	Green
4.	
Priority enforcement condition:
if (box_picked == box_dropped + 1)

If the detected box does not match the expected priority:

•	The box is ignored.

•	The robot continues navigation.

This ensures correct sequential task execution.

