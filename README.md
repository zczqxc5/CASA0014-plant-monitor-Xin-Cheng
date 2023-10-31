# CASA0014-PlantMonitor-Xin-Cheng

## Overview
This is a sensor system built for monitoring and store soil moisture and environment conditions, reminds users to water plant when it needs. 

## Functions
### 1. Real-time sensor data monitoring
Get Moisture, Temperature(/C),Temperature(#F), Humidity data via DHT22, GET Distance data from ultrasonic rangefinder sensor.
### 2. Live data presentation and transmission
Present live data via a webserver and send data via wifi to an MQTT server so that historic data could be stored in a data base.
### 3. Remind user to water plant 
When plant needs water, get close to it, the LED light Fast flashing <br>
When plant don't need water, get close to it, it plays music “jingle bell”

## Prerequisites
· Raspberry Pi<br>
· MQTT protocol<br>
· pre-designed Huzzah(with WIFI module)<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/a4bf1737a54f534ed13b4dd59454edd.png" width="400px"><br>
· Sensors: DHT22, ultrasonic rangefinder <br>

## Multiple Physical Prototypes
In this project, the prototype was designed in three phases. In the first stage, I completed the welding of the pre-designed Huzzah, Reading Moisture and DHT library data Present live data via a webserver and send data via wifi to an MQTT server are realized
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/8bdd4a0682ba0da0b75f7b0ef9ff61a.jpg" width="350px">


## Visualise data & Rasberry Pi connected

## Arduino Circuit
1. Expand function(with arduino UNO R3): Ultrasonic sensors control the active buzzer to play music and the LED flash，When the object is close to the ultrasonic sensor, the buzzer and LED start to work<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/598b7b9bd06c72aa865ef5839df9fe1.jpg" width="500px"><br>

2. Add expand function with Plant Monitor: <br>
a. Different of two Arduino board <br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/37b0e000830f1fdc959db474eea3fc8.png" width="350px"> <img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/b0fc1a147b849f3fdf48bb5e58b6b69.png" width="550px"><br>
b. Welding method (prototype only)<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/195559164985fe32466b3c4b619cec1.jpg" width="400px"><br>
c. Whole connection<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/7a7cc263fc17feafa366a0c61c7ca0d.jpg" width="500px"><br>


## Reminds
1. If you need to use a function inside the main loop, you need to have a return in that loop, with staro of “float”<br>
   <img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/image.png" width="500px">


