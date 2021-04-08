# ARGBfanxGetCurrent
Controlling an argb pc fan and controlling the tension and current delivered to this one w/ INA219


I use an Arduino nano to control an INA219 via I2C protocol and an argb fan with 14 ws2811 and I switch on/off/cycles of the argb ring of the fan via a shell script from a linux computer on the /dev/ttyUSB0 serial connection. Can also be done from WD10 but it is simpler to use arduino IDE and ctrl+shit+M.

ATM, there is only flowing schemes for the colors, and 4 themes, colors can be changed quite easily if you're familiar with the Arduino coding and if you like to tinker.
