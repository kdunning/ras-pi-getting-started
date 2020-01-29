# Getting started with the Raspberry Pi

This repository goes hand in hand with this [blog post](https://www.itdev.co.uk/blog/raspberry-pi-getting-started) and contains some basic examples of controlling GPIO lines on the Raspberry Pi with the use of an RGB LED using C and C++.

This is all very basic stuff, but should provide a very simple starting block for anyone new to the world of Raspberry Pi.

## Contents
- [Repository Contents](#repositorycontents)
- [Building](#building)
    - [Blog only](#blogonly)
    - [Demo only](#demoonly)
- [Running](#running)
    - [led_select (C application)](#led_select)
    - [led_demo (C++ application)](#led_demo)

## Repository Contents
This repository contains a very small number of files:
- led_demo.cpp - Demo C++ application source
- led_select.c - Code used within the blog post
- Makefile  - The makefile for the C and C++ applications
- Readme.md - This Readme file

## Building
This repository contains a Makefile, which should allow a simple call to `make`:

```sh
pi@raspberrypi:~ $ make
Building led_demo
Successfully built led_demo
Building led_select
Successfully built led_select
```
and to clean, simply run `make clean`:
```sh
pi@raspberrypi:~ $ make clean
Clean complete.
```
### Blog only
To build just the blog application (led_select), run:
```sh
pi@raspberrypi:~ $ make led_select
Building led_select
Successfully built led_select
```
or
```sh
pi@raspberrypi:~ $ gcc led_select.c -Wall -Wshadow -Wpedantic -lwiringP -o led_select
```

### Demo only

If you wish to build the demo application (led_demo), run:
```sh
pi@raspberrypi:~ $ make led_demo
Building led_demo
Successfully built led_demo
```
or
```sh
pi@raspberrypi:~ $ g++ led_demo.cpp -Wpedantic -Wshadow -Wall -lwiringPi -lpthread -o led_demo

```
Note the library requirements: [wiringPi](http://wiringpi.com/) and pthread (required for [software PWM](http://wiringpi.com/reference/software-pwm-library/))

## Running
### led_select
This is the source used to build the blog example C application. This provides a very basic application that takes a single argument: red, green or blue.

```sh
pi@raspberrypi:~ $ ./led_select red
Turning the red LED on.
```

### led_demo
This is the source used to build a C++ application, which gives a more detailed example of things that can be done with an LED, including:
- mixing colours
- hardware PWM
- software PWM

Run the application `./led_demo` and follow the instructions on screen.
```sh
pi@raspberrypi:~ $ ./led_demo
Enter a letter from the following:
* R (Red)
* G (Green)
* B (Blue)
* C (Cyan)
* M (Magenta)
* Y (Yellow)
* W (White)
* F  Free run - changes colour forever.
* P  Pulse - pulses the green LED forever (hardware PWM).
* S  Software PWM pulse - changes colour forever with software PWM fades.
* X  Exit
```
