# GRBL Online controller with BT gateway
*For Russian click here: [Russian](README_ru.md)*

I think many people are familiar with the terrible GRBL Controller, which comes bundled with the CNC 3018 Pro, popular on many Chinese sites.
![71bmuXZ9FVL](https://github.com/beeline09/grblGateway/assets/2519627/da9a46f9-5c30-4f36-9362-bae5de05f194)
After a lot of experience with 3D printers, this simply stunned me! This prompted me to solve two problems:
1. Make a convenient and more informative interface for managing and displaying basic information
2. Get rid of wires and be able to connect to the machine via Bluetooth connection.

The **ESP32 doit devkit v1** module is perfect for these purposes.
После нескольких вечеров кропотливой работы, я довел устройство до более или менее рабочего состояния и готов представить его общественности!

So here is the list of functions:
1. Control of axes and spindle.
2. Select acceleration and step size for each axis. The selected settings are saved after reboot.
3. Settings. Here we can set the parameters for connecting to the WiFi network, the name of the built-in access point, the name of the Bluetooth device, as well as the minimum and maximum spindle speeds.
4. View information about the history and firmware version.
5. Possibility of OTA firmware update.

When the device is turned on for the first time, it creates a built-in access point named **GrblGatewaySoftAP** and is available at the IP address *192.168.4.1*. After connecting to the built-in access point, you need to go to the settings and specify the settings of your WiFi network, but no one is stopping you from working on the built-in access point, you just won’t have Internet.

This is what the interface looks like on a mobile device:
![ezgif-2-b8d8cff7c9](https://github.com/beeline09/grblGateway/assets/2519627/157c6f34-40fc-4c3b-8f0c-5773a1ebf5de)

And here it is on the desktop:
![ezgif-3-53a8149111](https://github.com/beeline09/grblGateway/assets/2519627/ac92c6d5-580e-45a8-a4a9-414e81f53e1a)

Demonstration of my modified CNC 3018 Pro with its original red control board:
[![](https://markdown-videos-api.jorgenkh.no/youtube/AT5URx6y_3I)](https://youtu.be/AT5URx6y_3I)

In the future, you can update the firmware by clicking on the **OTA Update** button in the side menu.


Work in progress!!!
