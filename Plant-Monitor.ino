/*
    get date and time - uses the ezTime library 
    show data from a DHT22 on a web page served by the Huzzah 
    push data to an MQTT server
    control active buzzer by moisture data and ultrasonic rangefinder

    Xin Cheng
    Oct 2023
*/

//include library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ezTime.h> //date and time library
#include <PubSubClient.h> //connected MQTT library

#include <DHT.h>
#include <DHT_U.h>//nail sensor & humidity & temperature library

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


// Sensors - DHT22 and Nails
uint8_t DHTPin = 12;        //Pin 12 on Huzzah
uint8_t soilPin = 0;      // A0 pin on Huzzah for nail
float TemperatureC;
float Humidity;
float TemperatureF;
float temp;  
int Moisture = 1; // initial value just in case web page is loaded before readMoisture called
int sensorVCC = 13;
int blueLED = 2;
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.


// Set Wifi and MQTT
#include "arduino_secrets.h" 

const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

ESP8266WebServer server(80);
const char* mqtt_server = "mqtt.cetools.org";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// London timezone
Timezone GB;

//extend function with buzzer and LED
//define medium note value
#define NTF0 -1
#define NTF1 350
#define NTF2 393	
#define NTF3 441
#define NTF4 495
#define NTF5 556
#define NTF6 624
#define NTF7 661

//define high note value
#define NTFH1 700
#define NTFH2 786
#define NTFH3 882
#define NTFH4 935
#define NTFH5 965
#define NTFH6 996
#define NTFH7 1023

//define low note value
#define NTFL1 175
#define NTFL2 196
#define NTFL3 221
#define NTFL4 234
#define NTFL5 262
#define NTFL6 294
#define NTFL7 330

//Note frequency array
int tune[]=
{
	NTF3,NTF3,NTF3,NTF3,NTF3,NTF3,
	NTF3,NTF5,NTF1,NTF2,NTF3,NTF0,
	NTF4,NTF4,NTF4,NTF4,NTF4,NTF3,NTF3,NTF3,NTF3,
	NTF5,NTF5,NTF4,NTF2,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,NTFL5,NTFL5,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,
	NTFL6,NTF4,NTF3,NTF2,NTFL7,NTF0,
	NTF5,NTF5,NTF4,NTF2,NTF3,NTF1,NTF0,

	NTFL5,NTF3,NTF2,NTF1,NTFL5,NTF0,
	NTFL5,NTF3,NTF2,NTF1,NTFL6,NTF0,NTFL6,
	NTFL6,NTF4,NTF3,NTF2,NTF5,NTF5,NTF5,NTF5,
	NTF6,NTF5,NTF4,NTF2,NTF1,NTF0
};

//Note beat array
float durt[]=
{
	0.5,0.5,1,0.5,0.5,1,
	0.5,0.5,0.75,0.25,1.5,0.5,
	0.5,0.5,1,0.5,0.5,0.5,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1.5,0.5,

	0.5,0.5,0.5,0.5,1,0.5,0.25,0.25,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.75,0.25,

	0.5,0.5,0.5,0.5,1,1,
	0.5,0.5,0.5,0.5,1,0.5,0.5,
	0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
	0.5,0.5,0.5,0.5,0.75,0.25
};


//semsors -- buzzer and ultrasonic rangefinder
const int TrigPin = 15;
const int EchoPin = 16; 
int cm=1; //initial distance value 
int buzzer_pin = 14; 
int length;
int ledp=2; //led pin



void setup() {
  // Set up LED to be controllable via broker
  // Initialize the BUILTIN_LED pin as an output
  // Turn the LED off by making the voltage HIGH
  pinMode(BUILTIN_LED, OUTPUT);     
  digitalWrite(BUILTIN_LED, HIGH);  

  // Set up the outputs to control the soil sensor
  // switch and the blue LED for status indicator
  pinMode(sensorVCC, OUTPUT); 
  digitalWrite(sensorVCC, LOW);
  pinMode(blueLED, OUTPUT); 
  digitalWrite(blueLED, HIGH);

  //set up LED & buzzer & ultrasonic rangefinder
  pinMode(buzzer_pin, OUTPUT);
  length = sizeof(tune)/sizeof(tune[0]);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(ledp, OUTPUT);

  // open serial connection for debug info
  Serial.begin(115200);
  delay(100);

  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  // run initialisation functions
  startWifi();
  startWebserver();
  syncDate();

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

}


void loop() {
  //set up ultrasonic rangefinder function
  digitalWrite(14, LOW);

  digitalWrite(TrigPin, LOW); //send a short pulse to TrigPin
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  //call readDistance function to get distance value "cm"
  cm = readDistance();
  

  //Distance in 2-10cm humidity greater than 100 ,then play "jingle bell"
  if (cm>=2 && cm<=10 && readMoisture()>100){
    //The for loop plays the tune
    for(int x=0;x<length;x++)
    {
      tone(buzzer_pin, tune[x]);
      digitalWrite(ledp,HIGH);
      delay(500*durt[x]);	  // this 500 controls the duration of each note to set the tempo of the tune
      digitalWrite(ledp,LOW);	
      delay(100*durt[x]);				
      noTone(buzzer_pin);
    }
    delay(500);							// interval to start the next loop
  }
  else if (cm>=2 && cm<=10 && readMoisture()<=100){
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
    digitalWrite(ledp,HIGH);
    delay(100);
    digitalWrite(ledp,LOW);
    delay(100);
  }
  // handler for receiving requests to webserver
  server.handleClient();

  //call function
  delay(1000); //time interval to refresh MQTT data
  readMoisture();
  readDistance();
  sendMQTT();
  Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
  
  
  client.loop();
}


//Get distance value by ultrasonic rangefinder
float readDistance(){   //need return
  digitalWrite(TrigPin, LOW); //Send low to the TrigPin
  delayMicroseconds(2); //wait 2 microsecond
  digitalWrite(TrigPin,HIGH); //Send high to the TrigPin
  delayMicroseconds(10); //wait 10 microsecond
  digitalWrite(TrigPin, LOW); //Send low to the TrigPin
    
  //Store the echo time
  temp = float(pulseIn(EchoPin, HIGH)); 

  //convert echo time to distance(cm)
  cm = (temp * 17 )/1000; //The speed of sound is :340m/1s, Distance (cm) = (echo time * (34/1000)) / 2

  //Print data in Monitor
  Serial.print("Echo =");
  Serial.print(temp);  //echo time
  Serial.print(" | | Distance = ");
  Serial.print(cm);  //Actural distance
  Serial.println("cm");
  delay(100);
  return(cm); //return distance value
}

//Get Moisture value by DHT22
float readMoisture(){  //need return
  
  // power the sensor
  digitalWrite(sensorVCC, HIGH);
  digitalWrite(blueLED, LOW);
  delay(100);
  
  // read the value from the sensor:
  Moisture = analogRead(soilPin);         
  digitalWrite(sensorVCC, LOW);  
  digitalWrite(blueLED, HIGH);
  delay(100);
  Serial.print("Wet ");
  Serial.println(Moisture);   // read the value from the nails
  return Moisture;  //get the value of mositure
}


void startWifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void syncDate() {
  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());
}


void sendMQTT() {

  //check connect
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //send Temperature(unit:C)
  TemperatureC = dht.readTemperature(); // Gets the values of the temperature
  snprintf (msg, 50, "%.1f", TemperatureC);
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/zczqxc5/temperatureC", msg);

  //send Temperature(unit:F)
  TemperatureF = dht.readTemperature(true); // Gets the values of the temperature
  snprintf (msg, 50, "%.1f", TemperatureF);
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/zczqxc5/temperatureF", msg);

  //send humidity
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  snprintf (msg, 50, "%.0f", Humidity);
  Serial.print("Publish message for h: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/zczqxc5/humidity", msg);

  //send Moisture = analogRead(soilPin);   
  //moisture read by readMoisture function
  snprintf (msg, 50, "%.0i", Moisture);
  Serial.print("Publish message for m: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/zczqxc5/moisture", msg);

  //send Distance
  //Distance read by readDistance function
  snprintf (msg, 50, "%.0i", cm);
  Serial.print("Publish message for D: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/zczqxc5/Distance", msg);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect with clientID, username and password
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("student/CASA0014/plant/zczqxc5/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void startWebserver() {
  // when connected and IP address obtained start HTTP server  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
}


void handle_OnConnect() {
  TemperatureC = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  server.send(200, "text/html", SendHTML(TemperatureC, Humidity, Moisture));
}


void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";
  ptr += "<p>Sampled on: ";
  ptr += GB.dateTime("l,");
  ptr += "<br>";
  ptr += GB.dateTime("d-M-y H:i:s T");
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
