#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <arduino.h>+

const uint16_t kIrLed = D5;
IRsend irsend(kIrLed);  

  const char* ssid = "realme7";   
const char* password = "krishna@";

const char* mqtt_server = "broker.hivemq.com";

unsigned long millisNow = 0; //for delay purposes
unsigned int sendDelay = 2000; //delay before sending sensor info via MQTT

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // put your setup code here, to run once:
Serial.println("Connecting to network....");
  //searching the network & connecting if available
WiFi.mode(WIFI_STA);
  //connection with wifi station mode
WiFi.begin(ssid,password);
  //obtaining the ssid & password
while(WiFi.status() != WL_CONNECTED)
{
  Serial.print("..");
  delay(1000);
  }
Serial.println("WiFi Connected Successfully");
Serial.println("IP Address :");//will pint IP Address of the connected network
IPAddress myIP = WiFi.localIP();
//saving the ip address of connected network in myIP variable
Serial.println(myIP);//print saved IP in "myip" variable in serial monitor
}
void subscriber(){
  client.subscribe("MYdata");
//  client.subscribe("topic2");
//  client.subscribe("topic3");
}
void callback(char* topic, byte* message, unsigned int length) //function created for fetching data
{
  Serial.print("Data arrived [");//print ("Data arrived [") in serial monitor
  Serial.print(topic);//print topic of the publisher
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    //print data into serial monitor
    irsend.sendNEC((char)message[i]);
  }
  Serial.println();
}

void reconnect()//function created for recreation to MQTT Broker
{
  // Loop until we're reconnected
  int counter = 0;
  while (!client.connected()) //when client is not connected then will enter in the loop
  {
    if (counter==5)//if counter value equals to 5 then it will enter the loop
    {
      ESP.restart();
    }//it will do s/w restart of ESP8266
    counter+=1;//increment value of counter variable
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
   
    if (client.connect("b7d60435-ec68-4d43-9014-e2d093abe432","engineer","Rushil@1810")) 
    //connecting with the client id,user name & password is declared pf MQTT Broker
    //if NODEMCU is connected then it will enter the loop
    {
      Serial.println("connected");
      subscriber(); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());//print status of connection to client
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);//delay generated to 2000ms which is 2s
    }
  }
}

void setup(){
  Serial.begin(9600);//initiate serial pin & set baud rate of 9600
  setup_wifi();
  client.setServer(mqtt_server, 1883);//function named "mqtt_server" called to connect to MQTT server
  client.setCallback(callback);//call the function named "callback" which is created above
  irsend.begin();
  }

void loop() {
if (!client.connected())//if client not connected then enter to this loop
{
    reconnect();//try to reconnect to client and try until client is connected
  }
  client.loop();
}
