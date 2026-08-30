# Sharky Jones

<img width="382" height="322" alt="5235798393957850157" src="https://github.com/user-attachments/assets/53728bdd-1150-43e4-86fb-0bb304f2ee7e" />

*Sharky Jones — completed robot*

Sharky Jones is an ESP32-based two-wheel robot and a winner of a university competition, featuring L293D motor control, a servo-powered forklift-style gripper, and an ELRS radio control system.

## About

Sharky Jones was created by my friends and me for a university competition. The robot is based around an ESP32 and uses two DC motors for movement, an L293D motor driver, and a servo-powered forklift-style gripper.

The project went through several different control methods during development. We initially used a PS4 controller to communicate with the ESP32, but after discovering that this solution was unreliable, we switched to an ELRS-based control system.

The project also includes several hardware test programs that can be used during the build to verify individual components and wiring before running the complete robot software.

## Hardware

* Robot chassis
* ESP32
* L293D motor driver
* 2 × DC motors with wheels
* Servo motor for forklift
* ELRS receiver
* Breakout board
* Power distribution board
* Battery

## Controls

The final version of Sharky Jones is controlled using an ELRS connection.

| Control      | Function                 |
| ------------ | ------------------------ |
| Right stick  | Forward / reverse        |
| Left stick   | Steering                 |
| Scroll wheel | Forklift servo up / down |
| Boost button | Full motor PWM range     |

By default, the maximum motor PWM is limited to roughly half of the available range. The boost button temporarily allows the full PWM range to be used.

Another control method is a PS4 controller.

Several experimental PS4 controller versions were developed during the project. They are available in separate branches.

Note that they do not have the full implementation of the final version.

## Wiring 
<img width="409" height="457" alt="Screenshot_20260830_230757" src="https://github.com/user-attachments/assets/00d798a6-a502-455b-97d6-b842189cc0f7" />

*Electrical wiring diagram*

A more detailed interactive Wokwi project is also included in the repository. The archive was downloaded from the Wokwi website and contains the instructions and files required to use the simulation.

## ELRS Setup

<img width="323" height="327" alt="5235798393957850145" src="https://github.com/user-attachments/assets/86adf544-5668-4cbe-843c-848798178781" />
<img width="292" height="213" alt="5235798393957850146" src="https://github.com/user-attachments/assets/45456074-17ec-4efe-91f3-c0499989bb05" />

Previously, I had only seen ELRS on YouTube and had some basic knowledge about the different band versions. After checking which bandwidths are allowed in my country and wanting to avoid interference with Wi-Fi and Bluetooth, I selected the 915 MHz version, which can be configured to 868 MHz via a software update.

The entire setup was quite easy.

The receiver arrived first, so I started with its configuration. Some time after powering it on, it automatically turns into access point mode, so you can connect to it using wifi and visit its website for configuration. For more details, check the documentation for your specific model.

I updated the firmware. I used the BrotherHobby 900 MHz Nano RX module, and after setting up my pairing phrase, it was ready to work.

Setting up the remote was quite similar. I updated EdgeTX and ELRS software, set up the pairing phrase, and got a connection.

## Hardware Tests

In this folder, you can find scripts dedicated to testing certain parts of your robot during the build, such as:

* `esp32-L293D-gamepad-test.ino`
* `esp32-elrs-rx-test.ino`
* `esp32-servo-test.ino`



## Development Story

Sharky Jones was created by my friends and me for a university competition.

We were provided with a base consisting of a PCB with DC motors, an ESP32, a power distribution board, and a 9 V battery.

Initially, we used a PS4 controller to communicate with the ESP32, but quickly discovered its unreliability and decided to switch to an ELRS module and remote.

Wiring the motors and testing them with the PS4 controller took around 3–4 days. Another 2–3 days were dedicated to figuring out how to work with the servo and how to make it work together with the wheels. It turns out that the ESP32 is very pin-sensitive so you must check it with pinout documentation. Another 2–3 days went into transforming the robot to work with ELRS and the remote.

My friend was responsible for the design of the shell, and another was responsible for the design of the gripper.

## Tips & Tricks

### Servo calibration

Because my particular remote has a scroll wheel with a predefined center position, you need to calibrate the servo's initial position.

To fine-tune it, you can adjust the PWM signal sent by the remote inside its EdgeTX operating system. This way, the scroll wheel on the remote will be centered and it will represent the center position of the gripper.

This means you do not need to recompile the program or write any strange software corrections.

The same approach can be used for the end positions to avoid destroying the servo. Yes, that happened to us.

### L293D motor supply

The L293D can be supplied with a higher voltage for motors (9 V in my case) directly to its VCC2 pin.

For more information, check the L293D datasheet.
