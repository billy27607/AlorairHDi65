**AlorAir HDi65**

I have this AlorAir HDi65 Dehumidifier that I use to help reduce the humidity in a home that we have near the water. After purchasing it I found it had a CAN bus that is used to communicate with a remote control panel.

In our family we use Apple Home to control lots of stuff in our homes and I figured it might be cool to add the dehumidifer to the list. These are the results.

Thus project utiulizes a Seeed Studio Xiao ESP32C3 with their CAN bus breakout. I picked it because it has a low enough power requirement to use the power provided by the CAN bus too run the appliance.

There are a couple of examples provided which use the library. The first is a simple cli application that I used to work out the CAN bus protocol used to talk to the Dehumidifier. The second is the homekit appliance itself.

The appliance is develeloped using the excellent HomeSpan library for creating homekit appliances on ESP32 microcontollers. It is easy to use, stable and works great. I highly recommend it.

All of the reverse engineering that I did is noted in the file AlorAir HDi65.md There are more functions there that I needed for the homekit appliance.

The code is as is, so have fun.

