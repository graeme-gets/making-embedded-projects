# Week Ten - Power Consumption  

### Overview
All devices should consume as little power as possible for both longevity of battery life as well as the environment. 
While much can be done in hardware selection and design, a vast amount can be and needs to be done in software.

The device being used for this exercise is the Final Project Dodeca Timer.

### Reducing Power

**Notes**
 1. Estimated values will be used as the project has not got to the stage of reducing power at time of writing. 
 2. As 'off the shelf' breakout/dev boards have been used, they all have their own LDO's which would not be the case in a production unit. 

**Software**
 
The two main areas where clock speed are critical  is the LED driver which needs to be run at an accurate 800Khz and the Serial communication. 
The RTC will need to run off the external crystal in order to maintain accurate time.

Looking at the hardware specifications, the device can run in two of the 4 low power modes.
1. Run mode for when the device is awake
2. sleep mode for then the device is idle and waiting a change of state

Min Clock Speed to cater for LED and Comms 

1. Serial Comms - A baud rate of 115200 can be achived with a low clock speed from 8Mhz. the minimum the device will run would be the internal oscillator which is 16Mhz. Accouding the the STMN32F411 family documentation, a 115200 baud rate at 16Mhz can be achieved with 0.08% error rate which is well within operational parameters.
2. LED driver at 800Khz - for accurate only teh external crystal should be used which is at a 25Khz for the Blackpill dev board. It snot possible to achieve an exact 800Kkz frequency with whole numbers, so PLL could to be used to bring the system clock to a 800Khz. Thought some calculation with the clock settings could achieve the same result at a lower clock. 

The device will only need to be run in 'Run Mode' while obtaining the orientation and setting, LED Face and logging events. Or during configuration. 

This allows the device to be run in sleep mode for 90% of the time. With some hardware design changes, the device could run in STOP mode. 

**Peripherals**  
Most of the peripherals on the processor will not be used and can be switched off. This is done by disabling any unused peripherals and enuring that pins a re tied low.

**Clock Busses**  
Not all clock buses are used and can be disabled. 

**Clock**
The clock can be lowered as this appears to have the largest impact on consumption.

**Gyro and RTC Interrupts**  
The Gyro and RTC Interrupts can be used to wake the device up from sleep. With a full custom hardware design, the device could be put into Stop Mode and upon awaking, the time and date could be retrieved from an external source. The current prototype depends on teh RTC keeping its own time. 


**Hardware**  
When choosing hardware, each chip or module will (or should) have a datasheet specifying the electrical characteristics. Within these characteristics there will be a list of the power consumption (usually in mA) along with possible power modes. 

The Dodeca Timer has the following devices. 

Item|Operational|Sleep mode|Standby|Stop mode
-|-|-|-|-
STM32F411CE	|31.1	|12.2	|0.0028	|0.112
-|50% brightness|25%		
Neo Pixel (assume 1 face)|	36.3	|16		
Fuel Gauge	|0.003|	0.001		
MPU6050	|0.014	|0.005		
3v3 Buck Boost regulator	|0.03			

Most of these modules can be configured to consume less power using software configuration at run time. 

The Buck Booster does have an 'Enable' line which would switch off all current to the devices and then consume less then 30uA. However, the module bought has the enable tied high (on) in hardware and is not controllable in its current configuration.


### Operation of the device. 
The device as a whole will spend 99% of its time in a low power mode. It is only when the dice is moved (orientation changed) or a task timeout occurs that normal operational power will be needed. T timed (min by min) change may occur with the addressable LED's, but that will me a few ms of operational power.

The largest consumer of power is the addressable LED's, which as full brightness would consume approximately 60mA. (3 Leds, rgb, per pixel at approx 20mA each).

### Power consumption mitigation

**1. Addressable LED's**

The LED's will, by large, be limited to a single face at 50% brightness in order to reduce the power consumption. On average the true power consumption of a pixel in a mixed colour state is around 20mA.

The device has 12 faces each with 12 LED's for a total of 144 LED's. At full brightness and All lit (white) this could consume :

(The LED ring is sired in parallel and hence the current will multiply with each LED)

60mA X 144 = 8.6A

however at at average of 20mA

20mA X 144 = 2.88A, which is a lot easier to deal with. However the Buck Boost regulator can only supply around 2A which is just short of 2.88A. The brightness could be lowered down to around 25% which would bring this in line.

However, the project does not require all faces to be lit at any given time. At most, we could look at half the faces at 50% brightness which would work out at :

(to be conservative we will assume 30mA per pixel)

72 x (30mA / 2) = 1.08 A

the current design would only require 1 face at any given time

12 X (30mA / 2) = 180mA

Note : the measured current for a single face is curretnly measuring an average of 36mA.

**2. Processor**

The processor is  the STM32F411CE on a Black Pill development board. 
The STM32F4 family has four power modes. 

Item|Run Mode|Sleep mode|Standby|Stop mode
-|-|-|-|-
STM32F411CE	|31.1	|12.2	|0.0028	|0.112

Each mode can further be tuned to switching off different areas or functions of the device. For instance

**In Run Mode:**

Mode|Current
-|-
With all cloaks and peripherals on at 100Mhz    |31.7mA
With HSI, PLL and peripherals off at 1Mhz      |0.9mA
Where as all clocks & peripheral on but at 20Mhz |7.6mA


**In Sleep Mode**
Mode|Current
-|-
With all cloaks and peripherals on at 100Mhz    |12.2mA
With HSI, PLL and peripherals off at 1Mhz      |0.3mA

Where as all clocks & peripheral on but at 20Mhz |2.8mA

**In Stop Mode**
Mode|Current
-|-
Flash in Stop mode, all oscillators OFF, no independent watchdog | 113.7uA
Flash in Deep power down mode, all oscillators OFF, no independent watchdog|10uA

**In Standby mode**
Mode|Current
-|-
Low-speed oscillator (LSE) and RTC ON|2.8uA
RTC and LSE OFF|2.1

For this device , it could operate in Run Mode and Sleep mode as the RTC will be needed in order to keep track of the passing time and wake the processor on an event.

The clock could also be reduced in both Run Mode and sleep to conserve power. In sleep, 20Mhz would be more than sufficient 

In Run Mode the clock could ber reduced to a speed that would still allow the Addressable LED's to be driven at 800Khz and the baud rate for the Serial connection at a minimum of 9600. 

## Spreadsheet calculations##

Please refer to spreadsheet 
![Power Calculations](/week-ten/PowerUsage.xlsx)

