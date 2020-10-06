# Dronecoria Smart Sower
# by Lot Amoros
# License GPL v3
Arduino Smart Sower for control the Dronecoria sowing device.

This code decodes a PPM signal from a RC Receiver and selects two channels to control the seed flow and the seed propulsion distance.
The seed flow is operated via Servo, and the seed distance by a brushed motor.

Ideally is desirable to use a small Arduino, like the Arduino Nano, and create a small shield to do the wiring.

# Download and print the hardware:
https://wikifactory.com/@aeracoop/dronecoria/files/seedDispenser

# PPM Decode from rinozen:
https://gist.github.com/rinozen/ab73d79dbe7be05d6adceb85872da84a

PPM Input conected to PIN2
PPM Input Channel 6: Sowing rate.
PPM Input Channel 7: Sowing distance.

PWM Output Sowing Rate Servo to D9
PWM Output Sowing Distance Motor to D6

