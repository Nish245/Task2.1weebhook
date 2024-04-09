#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"

char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
int number2 = random(0,100);
int number3 = random(0,100);
int number4 = random(0,100);
String myStatus = "";

const int buzzerPin = 2; // Piezo buzzer pin
float temperature = 0;

void setup() {
  Serial.begin(115200);      
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin to output mode
  ThingSpeak.begin(client);  
  // Assuming setup for DHT11 sensor would go here
}

void loop() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Read temperature from the DHT11 sensor
  // For demonstration, this is where you'd call a function to read from the DHT11
  // temperature = readTemperature();
  temperature = 21.0; // Simulated temperature value

  // Check temperature and control buzzer
  if(temperature > 20){
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);

  // figure out the status message
  if(number1 > number2){
    myStatus = String("field1 is greater than field2"); 
  }
  else if(number1 < number2){
    myStatus = String("field1 is less than field2");
  }
  else{
    myStatus = String("field1 equals field2");
  }
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // change the values
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  number2 = random(0,100);
  number3 = random(0,100);
  number4 = random(0,100);
  
  delay(20000); // Adjusted delay to 20 seconds as per your initial requirement
}
