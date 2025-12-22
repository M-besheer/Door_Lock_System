# Door Lock System

Embedded door lock project using two TI Tiva C microcontrollers (LaunchPad). The system separates the Human-Machine Interface (HMI) from the control/processing unit and demonstrates a simple secured door control with password management.

## Overview

This project uses two Tiva C boards:
- HMI_ECU — the Human Machine Interface board. It handles user input (keypad/buttons), displays information to the user (LCD/segment display), sends commands to the control unit, and prints responses.
- CONTROL_ECU — the control/processing board. It verifies passwords stored in memory, opens/closes the door (via a motor/actuator), supports changing the password, and enforces a timeout/lockout behavior.

The HMI and Control ECUs communicate over a UART communication module. The HMI sends user commands and the CONTROL_ECU responds with status/result messages.

> NOTE: Exact hardware connections (pins, sensors, actuator connections) are declared in the HAL (Hardware Abstraction Layer) files in the repository — consult those files for wiring details.

## Features

- Password verification against stored memory
- Open/Close door command
- Change password flow
- Timeout/lockout to prevent brute-force attempts
- HMI-driven input and display logic separated from control logic
- Hardware mappings concentrated in HAL for portability

## Repository structure (high-level)

- /HMI_ECU/ — firmware and HAL for the human-interface board
- /CONTROL_ECU/ — firmware and HAL for the control/processing board
- /HAL/ — hardware abstraction layer files (pin definitions, peripheral inits) — inside each ECU folder

## Building and Flashing

Prerequisites (typical):
- IAR embedded workbench or another ARM/Tiva-compatible toolchain that supports TM4C microcontrollers
- Tiva C Series LaunchPad boards (TM4C123gh6pm)
- USB cable for flashing and serial communication

Basic steps:
1. Open the ECU project (HMI_ECU or CONTROL_ECU) in your IDE (e.g., CCS).
2. Select the correct target device (Tiva c TexasInstruments TM4C123gh6pm).
3. Build the project.
4. Flash the binary to the corresponding LaunchPad.

## Wiring and Hardware

Refer to the HAL files for exact pin mappings — they include the definitions for:
- Keypad connected to the HMI board
- LCD and I2C display connections for the HMI board
- UART/communication pins between the two boards
- Solenoid Actuator on the CONTROL_ECU
- LEDs, buzzer used for feedback on the CONTROL_ECU

## Usage

Typical user flows:
- Enter password on the HMI; HMI sends the password to CONTROL_ECU; CONTROL_ECU checks memory and responds with success/failure.
- On success, CONTROL_ECU actuates the door motor and returns status to HMI to show on display.
- Change password: HMI collects new password from user, sends change request to CONTROL_ECU, which updates stored memory after successful validation.
- Lockout: After a configurable number of failed attempts or a defined timeout period, CONTROL_ECU will enforce a lockout.
- Timeout: time set using a potentiometer is sent from the HMI to the CONTROL_ECU that is then stored in memory to be fetched when door opens and closes after the *set* timeout.

## Where to find hardware details

All hardware and pin mappings are kept in the HAL and MCAL files. If you need to change pins or the peripheral setup (timers, UART, GPIO), modify the relevant HAL or MCAL file(s) and rebuild.
