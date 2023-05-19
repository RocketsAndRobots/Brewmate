# Brewmate
This project was intended as a support when brewing my own beer. Its main purpose is at the moment to control the temperature of a heated pot. This happens by measuring temperature with a affordable sensor and controlling the pot with a typical remote plug from the hardware store that uses 433MHz. This enables a simple two-point controller that swings around the temperature optimum.

This project is at the moment mainly meant as a project for myself so it is not sure whether I add detailed instructions and schematics in the near future. But it might be possible.
Also do not expect stable, documented and good tested code.

Current functions
-------------------
- Temperature control
- Follow mashing / cooking recipe
- Notify user via Serial about states
- Buzzer alarm when changes happen or user input is required via a connected button

Planned functions
------------------
- Add lcd display for current status / user interactions
- Add more buttons
- Allow to modify recipes on-the-fly
- Add casing
- Add schematics

Required components
-------------------
- Arduino Nano or similiar board
- Temperature sensor (DS18B20)
- 433MHz sender (FS-1000A)
- Button switch
- Resistors
- Buzzer
- Board to connect everything on
