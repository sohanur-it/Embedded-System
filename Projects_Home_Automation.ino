#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include<dht.h>


/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Sohan-Router"
#define WLAN_PASS       "sohan12345"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "team360"
#define AIO_KEY         "13dc66e3db4346d98bbd0387106e45c6"


/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

Adafruit_MQTT_Publish air_quality = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/air_quality");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/sohan_led1");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe sohan_led2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/sohan_led2");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe maingate = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/maingate");

Adafruit_MQTT_Subscribe bedroom_fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/bedroom_fan");

Adafruit_MQTT_Subscribe reset = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/reset");





/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
dht DHT;
//
#define ledpin D2
#define ledpin1 D3
#define maindoor D4
#define bedfan D1
#define DHT11_PIN D0
#define anInput A0 
#define co2Zero 55
uint32_t x=0;




void setup() {
  Serial.begin(115200);
  
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
   mqtt.subscribe(&sohan_led2);
   mqtt.subscribe(&maingate);
   mqtt.subscribe(&bedroom_fan);
   mqtt.subscribe(&reset);

  pinMode(ledpin, OUTPUT);
  pinMode(ledpin1, OUTPUT);
  pinMode(maindoor, OUTPUT);
  pinMode(bedfan, OUTPUT);
  pinMode(anInput,INPUT);





}



void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)onoffbutton.lastread);
      
      if (strcmp((char *)onoffbutton.lastread, "ON") == 0) {
        digitalWrite(ledpin, HIGH);
      }
      if (strcmp((char *)onoffbutton.lastread, "OFF") == 0) {
        digitalWrite(ledpin, LOW); 
      }
    }

    if (subscription == &sohan_led2) {
      Serial.print(F("sohan_led2: "));
      Serial.println((char *)sohan_led2.lastread);
      
      if (strcmp((char *)sohan_led2.lastread, "ON") == 0) {
        digitalWrite(ledpin1, HIGH);
      }
      if (strcmp((char *)sohan_led2.lastread, "OFF") == 0) {
        digitalWrite(ledpin1, LOW); 
      }
    }

    if (subscription == &bedroom_fan) {
      Serial.print(F("Bedroom-fan: "));
      Serial.println((char *)bedroom_fan.lastread);
      
      if (strcmp((char *)bedroom_fan.lastread, "ON") == 0) {
        digitalWrite(bedfan, LOW);
      }
      if (strcmp((char *)bedroom_fan.lastread, "OFF") == 0) {
        digitalWrite(bedfan, HIGH); 
      }
    }

    if (subscription == &maingate) {
      Serial.print(F("maingate: "));
      Serial.println((char *)maingate.lastread);
      
      if (strcmp((char *)maingate.lastread, "ON") == 0) {
        digitalWrite(maindoor, LOW);
        delay(100);
        digitalWrite(maindoor, HIGH);
        
      
        
      }
      if (strcmp((char *)maingate.lastread, "OFF") == 0) {
        
        
        digitalWrite(maindoor, LOW);
        delay(100);
        digitalWrite(maindoor, HIGH);
      
      }
    }


       if (subscription == &reset) {
      Serial.print(F("reset: "));
      Serial.println((char *)reset.lastread);
      
      if (strcmp((char *)reset.lastread, "ON") == 0) {
        Serial.println("reset activate");
        
int chk = DHT.read11(DHT11_PIN);
delay(100);

String temp = String(DHT.temperature);
String temp_in_cel =  String(temp + "°C");

String humidit = String(DHT.humidity);
String humidity_percent =  String(humidit +"%");


Serial.println(" Humidity " );
Serial.println(humidity_percent);
Serial.println(" Temparature ");
Serial.println(temp_in_cel);

int co2now[10];                               //int array for co2 readings
int co2raw = 0;                               //int for raw value of co2
int co2comp = 0;                              //int for compensated co2 
int co2ppm = 0;                               //int for calculated ppm
int zzz = 0; //int for averaging

for (int x = 0;x<10;x++){                     //samplpe co2 10x over 2 seconds
    co2now[x]=analogRead(A0);
    delay(10);
  }
for (int x = 0;x<10;x++){                     //add samples together
    zzz=zzz + co2now[x];
  }
  co2raw = zzz/10;                            //divide samples by 10
  //Serial.println(co2raw);
  
  co2comp = co2raw - co2Zero;                 //get compensated value
  co2ppm = map(co2comp,0,1023,300,2000);      //map value for atmospheric levels
  //Serial.println(co2comp);
  //Serial.print("Measurement of CO2 : ");
  Serial.println(co2ppm);

  temperature.publish(DHT.temperature);
  humidity.publish(DHT.humidity);
  air_quality.publish(co2ppm);
      }
    
    }


  
  }

    




  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
