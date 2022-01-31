# Chess Timer

The goal of this project was to create a compact and satisfying chess timer to play over-the-board chess games with time control. It uses an Arduino Nano as the brains, two small OLED displays to show the time and configure the game, and three cherry mx switches to control the timer, all within a small 3d printed case.

![](img\Chess_Timer_2022-Jan-20_03-01-05AM-000_CustomizedView14700881950.png)

## Materials List

Switches - [Cherry MX Brown (digikey)](https://www.digikey.com/en/products/detail/cherry-americas-llc/MX1A-G1NA/7537122) x3

Power Switch - [DaierTek Rocker Switch (amazon)](https://www.amazon.com/gp/product/B07S2QJKTX/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1)

Key Caps - [SparkFun Translucent Black Keycaps (digikey)](https://www.digikey.com/en/products/detail/sparkfun-electronics/PRT-15307/10130912)

Voltage Booster - [Adafruit 5v mini booster (digikey)](https://www.digikey.com/en/products/detail/adafruit-industries-llc/4654/12697636)

Battery Holder - [AAA Battery Holder (digikey)](https://www.digikey.com/en/products/detail/keystone-electronics/2468/303817)

Prototype Board - [amazon](https://www.amazon.com/Prototype-Universal-Perfboard-Soldering-Compatible/dp/B07LF7N5K9)

OLED Display - [HiLetgo 0.96’’ OLED Display](https://www.amazon.com/gp/product/B06XRBTBTB/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&th=1) x2

Jumper Wires - [amazon](https://www.amazon.com/EDGELEC-Breadboard-Optional-Assorted-Multicolored/dp/B07GD2BWPY/ref=sr_1_1_sspa?crid=2DL50N9TG456D&keywords=jumper+wires&qid=1643639854&s=industrial&sprefix=jumper+wires%2Cindustrial%2C128&sr=1-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyT1hFVVFEOUUzTjQ1JmVuY3J5cHRlZElkPUEwNTY4OTk4M09SNDdXTzIxSkxFVyZlbmNyeXB0ZWRBZElkPUEwOTQ1NDM2MUpBN1RMSkJGREFMWiZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

Case - 3D Printed (Black PETG)

MCU - [Arduino Nano Every](https://store-usa.arduino.cc/products/arduino-nano-every-with-headers?gclid=Cj0KCQiArt6PBhCoARIsAMF5wahwZKMWHL1fq-DChFeSEfrtX608ick-iNuaEiH_mXz8zZ4XBC26nFgaAhvWEALw_wcB)

## Wiring

The wiring diagram below shows which pins to connect the wires to.

Note: You will need to solder the jumpers on the back of one of the OLED displays to change its I2C address.

## Code

If using an Arduino Nano (not Nano Every), the program memory is not big enough to hold both screens in the memory.
