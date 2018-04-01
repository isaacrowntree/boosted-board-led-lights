# Boosted Board LED light strip with accelerometer

I decided I wanted to build a crazy LED light strip for my boosted board with a brake light.

This code kind of works, but I need to do a lot more fine-tuning.

Things left:

- discover the right threshold and maybe a better algorithm for detecting braking
- miniaturise the electronics
- figure out a way to duct-tape everything to my board
- decide if I want to use only the rear LEDs for the brake, or all of them
- find out if my new Cygnett powerbank will power the Arduino and LEDs without frying them

Parts list:

- Arduino Uno
- +/-3g 3-axis accelerometer
- 1m x 30 WS1218B Digital RGB LEDs
- 1 1000uF capacitor
- 1 220 ohm resistor
- 1 Cygnett 5000mAh 5V 2.4amp powerbank

Thanks to:

- FastLED for the sweet Cylon-style light code/ideas
