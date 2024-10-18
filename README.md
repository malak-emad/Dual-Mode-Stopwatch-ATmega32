# Dual-Mode Stopwatch with ATmega32 

This repository contains the code and documentation for a **Dual-Mode Stopwatch** developed using the ATmega32 microcontroller. The stopwatch features both **Increment** and **Countdown** modes, making it a versatile timer for various applications. This project is part of the Embedded Diploma at **Edges For Training**.

## Project Overview

The stopwatch operates in two modes:
- **Increment Mode**: The stopwatch counts upwards from zero, displaying hours, minutes, and seconds.
- **Countdown Mode**: The user sets a countdown time, and an alarm (buzzer) is triggered when the countdown reaches zero.

### Features:
- **Increment Mode** (Default): Stopwatch counts upwards, showing hours, minutes, and seconds.
- **Countdown Mode**: Set a custom countdown time; when it reaches zero, the alarm is triggered.
- **Reset**: Reset the stopwatch to zero.
- **Pause & Resume**: Pause the stopwatch and resume from where it was stopped.
- **Mode Toggle**: Switch between Increment and Countdown modes.
- **Time Adjustments**: Adjust the hours, minutes, and seconds using push buttons.

## Hardware Components
- **Microcontroller**: ATmega32
- **Display**: 6 multiplexed seven-segment displays (common anode)
- **Input**: 10 push buttons for Reset, Pause, Resume, Mode Toggle, and time adjustments
- **Alarm/Buzzer**: Activated when countdown reaches zero
- **LED Indicators**: Red LED for Increment mode, Yellow LED for Countdown mode

## System Design

- **Timer1** in **CTC mode** for precise timing.
- **External Interrupts**:
  - **INT0** for Reset.
  - **INT1** for Pause.
  - **INT2** for Resume.
- **Multiplexing**: The 6 seven-segment displays are driven using a 7447 BCD decoder and multiplexing technique to save MCU pins.

## How to Use

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/dual-mode-stopwatch.git
    cd dual-mode-stopwatch
    ```

2. **Open the project** in your preferred IDE (e.g., Atmel Studio).

3. **Build and upload the code** to the ATmega32 microcontroller.

4. **Connect the hardware** as per the circuit diagram provided in the documentation.

5. **Use the stopwatch**:
    - Default mode is Increment.
    - Use Mode Toggle button to switch to Countdown mode.

## Circuit Diagram

Include a circuit diagram or link to it here.

## Demo

Check out the [project demo video](https://youtu.be/-7c2NCQg5r4) to see the stopwatch in action!

## Acknowledgments

Special thanks to **Eng. Mohamed Tarek** for his guidance throughout the project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
