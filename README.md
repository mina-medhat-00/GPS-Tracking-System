# GPS Tracking System Readme

## Table of Contents
- [Introduction](#introduction)
- [Functional Requirements](#functional-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Results](#results)
- [Contributing](#contributing)
- [License](#license)

## Introduction
In this project, you will develop a GPS tracking system using TM4C123G LaunchPad. The system is designed to store the trajectory of movement from power-on until a destination point is reached and calculate the total distance traveled.

### Options for Defining the Destination Point
1. Once the moved distance exceeds 100m, stop adding new points to the trajectory, and the last point added will be the target destination.
2. Stop adding new points when a push button is pressed, and the last point added will be the last destination.
3. Predefine the destination point in your code, and when the system reaches this point, stop adding new points to the trajectory.

Other functionalities include turning on the built-in LED of the LaunchPad when the destination point is reached and displaying the calculated distance on three 7-segment displays.

## Functional Requirements
To set up and use the GPS tracking system, the following functional requirements must be met:

1. **Interfaces Configuration**: After power-on, the system should configure the following interfaces:
   - UART to interface with the GPS module
   - Digital Output for built-in LED
   - Digital Output for 7-segment displays

2. **GPS Data Acquisition**: The system should read GPS module data and wait until there is a GPS fix before proceeding. The first point read with GPS fix will be considered the start point and will be appended to the trajectory.

3. **Periodic GPS Data Reading**: The system must read coordinates from the GPS module periodically to update the trajectory. A software delay or Systick timer should be used to control the rate of execution depending on the GPS module's update rate.

4. **Destination Point**: Once the destination point is reached, the system should stop appending new points to the trajectory based on the chosen option (Option 1, Option 2, or Option 3).

5. **Output Indication**:
   - Stage 1: The built-in LED will be turned on when the target destination is reached or when the moved distance exceeds 100 meters.
   - Stage 2: The taken distance will be displayed on the 3-digit 7-segment displays.

6. **Trajectory Criteria**:
   - The total distance between the start and end points should be greater than 100 meters.
   - The path from the start point to the end point should form a non-straight line similar to the provided baseline path.
   - The software should calculate the total taken distance, and the calculated distance should have a maximum deviation (error margin) of 5% compared to the distance obtained from Google Maps.

## Installation
To use the GPS tracking system on the TM4C123G LaunchPad, follow these steps:

1. Clone the repository to your local machine.
2. Connect the TM4C123G LaunchPad to your computer via USB.
3. Install the required software and development environment (provide details on how to set up the development environment, if applicable).
4. Build the project and flash it onto the LaunchPad.

## Usage
To run the GPS tracking system, follow these steps:

1. Power on the TM4C123G LaunchPad.
2. Wait for the GPS module to acquire a fix and determine the start point.
3. Start moving with the LaunchPad while following a non-straight path as described in Figure 1 (provide Figure 1 with the path).
4. The system will store the trajectory until the destination point is reached based on the chosen option (Option 1, Option 2, or Option 3).
5. Once the destination point is reached, the built-in LED will turn on, indicating the completion of the tracking.
6. The 7-segment displays will show the calculated distance traveled if they are not updated continuously.

## Results
After successfully running the GPS tracking system and reaching the destination point, you should observe the following:

1. The built-in LED of the LaunchPad will be turned on.
2. The 7-segment displays will show the calculated distance traveled (if not updated continuously).
