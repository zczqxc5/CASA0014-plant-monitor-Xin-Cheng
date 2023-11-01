# CASA0014-PlantMonitor-Xin-Cheng

## Overview
This is a sensor system built for monitoring and store soil moisture and environment conditions, reminds users to water plant when it needs. I also wanted to make the plant feel like a Christmas tree and add some interaction functions for users to communicate with collected data.<br>
Main codes are in the “Plant-Monitor.ino” file

## Functions
### 1. Real-time sensor data monitoring
Get Moisture, Temperature(/C),Temperature(#F), Humidity data via DHT22, GET Distance data from ultrasonic rangefinder sensor.
### 2. Live data presentation and transmission
Present live data(Distance,Temperature(/C),Temperature(#F), Humidity) via a webserver and send data via wifi to an MQTT server so that historic data could be stored in a data base.
### 3. Remind user to water plant 
When plant needs water, get close to it, the LED light Fast flashing <br>
When plant don't need water, get close to it, it plays music “jingle bell”

## Prerequisites
· Raspberry Pi<br>
· MQTT protocol<br>
· pre-designed Huzzah(with WIFI module)<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/a4bf1737a54f534ed13b4dd59454edd.png" width="400px"><br>
· Sensors: DHT22, ultrasonic rangefinder <br>
· libraries(see in code)

## Multiple Physical Prototypes methods
In this project, the prototype was designed in three phases. In the first stage, I completed the welding of the pre-designed Huzzah, reading Moisture and DHT library data Present live data via a webserver and send data via wifi to an MQTT server are realized<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/8bdd4a0682ba0da0b75f7b0ef9ff61a.jpg" width="500px"><br>

The second step, I realized in the Arduino UNO R3 by monitoring the data of the ultrasonic sensor, to achieve the song "jingle bell" play or not. This is because the pre-designed Huzzah has no bare pins and it was difficult for me to add features with it and test the functionality. <br>
The code for this prototype are shown in file "Buzzer-function(UNO R3).ino" <br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/598b7b9bd06c72aa865ef5839df9fe1.jpg" width="500px"><br>

In the final prototype, I combined the functions on the two boards into one. I welded Huzzah's unused pins "2,14,15,16" and "power supply" and ground with "wires", to connect the functions I wrote on the second prototype<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/7a7cc263fc17feafa366a0c61c7ca0d.jpg" width="500px"><br>
Different of two boards <br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/37b0e000830f1fdc959db474eea3fc8.png" width="350px"> <img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/b0fc1a147b849f3fdf48bb5e58b6b69.png" width="550px"><br>

## Visualise data 
### Uses the DHT22 to create a simple environment reporting webpage
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/dcebaa04c15a3a787cdc4599d9b4675.png" width="800px"><br>
### On MQTT
![image](https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/assets/146037962/3d7736a8-ee52-4ba6-bf05-9feec57b154a)

### On influxDB
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/46dbe8c824dab441607ebfe56df6686.png" width="800px"><br>
### On Grafana and compare data with other students
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/de3df8b5d42cc037416de4bca255481.png" width="800px"><br>



## Limitation
1. Welding method between Huzzah and wires is not strong (prototype only)<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/195559164985fe32466b3c4b619cec1.jpg" width="400px"><br>
2. Moderate data changes drastically with the depth and distance of the nail insertion, so prototype cannot move the nail when it is actually used<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/115a93d8ce7e74c5d6df9ab7d53ee1e.jpg" width="400px"><br>
3. Ultrasonic rangefinder will not output data to MQTT when there is no object near it for a long time, and it will take a certain time to restore the display, even if there is an object near it<br>
<img src="https://github.com/zczqxc5/CASA0014-plant-monitor-Xin-Cheng/blob/main/pictures/2eb7e256de489f96565be438970ae47.png" width="800px"><br>



