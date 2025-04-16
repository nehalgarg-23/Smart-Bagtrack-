#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// WiFi credentials
const char* ssid = "project";
const char* password = "project007";

// Telegram Bot credentials
const char* botToken = "7750716544:AAGuG8IrpSTv9Q5jcY0Z6XR9DEInsKULmmg";
const char* chat_id = "2016205854";

// Fallback GPS coordinates (default location)
float defaultLat = 28.6109;
float defaultLng = 77.0385;

unsigned long lastTimeBotRan;
const int BOT_INTERVAL = 1000; // Check every 1 second

#define TRIGGER_PIN D1  // GPIO5
#define RX D7           // GPS RX pin
#define TX -1           // TX not used with one-way communication
#define GPS_BAUD 9600

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RX, TX); // Initialize Software Serial
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(GPS_BAUD);

  pinMode(TRIGGER_PIN, INPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  bot.sendMessage(chat_id, "Project started- use /status for bag Navigation", "");


  client.setInsecure(); // Accept self-signed SSL certs
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    Serial.println("Received message: " + text);

  if (text == "/status") 
  {
      unsigned long start = millis();
  float lat = 0.0, lng = 0.0;
  bool validLocation = false;

  // Read GPS data for 5 seconds
  while (millis() - start < 5000) 
  {
    while (gpsSerial.available() > 0) 
    {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isUpdated() && gps.location.isValid()) 
    {
      lat = gps.location.lat();
      lng = gps.location.lng();
      validLocation = true;
      Serial.print("LAT: "); Serial.println(lat, 6);
      Serial.print("LONG: "); Serial.println(lng, 6);
      break; // Exit loop early if valid data found
    }
  }

  // Use default coordinates if GPS not valid
  if (!validLocation) {
    Serial.println("Using default location.");
    lat = defaultLat;
    lng = defaultLng;
  }
    String message = "📍 Location:\nhttps://www.google.com/maps/search/?api=1&query=" + String(defaultLat, 6) + "," + String(defaultLng, 6);
    bot.sendMessage(chat_id, message, "");
    Serial.println("Location sent!");
    } 
    else 
    {
      bot.sendMessage(chat_id, "Unknown command. Use /start, /status, or /stop", "");
    }
  }
}

void loop() {
  unsigned long start = millis();
  float lat = 0.0, lng = 0.0;
  bool validLocation = false;

  // Read GPS data for 5 seconds
  while (millis() - start < 5000) 
  {
    while (gpsSerial.available() > 0) 
    {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isUpdated() && gps.location.isValid()) 
    {
      lat = gps.location.lat();
      lng = gps.location.lng();
      validLocation = true;
      Serial.print("LAT: "); Serial.println(lat, 6);
      Serial.print("LONG: "); Serial.println(lng, 6);
      break; // Exit loop early if valid data found
    }
  }

  // Use default coordinates if GPS not valid
  if (!validLocation) {
    Serial.println("Using default location.");
    lat = defaultLat;
    lng = defaultLng;
  }

  // Check D1 pin status
  if (digitalRead(TRIGGER_PIN) == LOW) 
  {

    Serial.println("Switch Pressed - Sending Telegram Notification");
    // String message = "📍 Location:\nLatitude: " + String(lat, 6) + "\nLongitude: " + String(lng, 6);
    // bot.sendMessage(chat_id, message, "");
    // Serial.println("Location sent!");

    String message = "📍 Location:\nhttps://www.google.com/maps/search/?api=1&query=" + String(lat, 6) + "," + String(lng, 6);
    bot.sendMessage(chat_id, message, "");
    Serial.println("Location sent!");

    // Wait for trigger pin to return to HIGH before continuing
    while (digitalRead(TRIGGER_PIN) == LOW);
  }


  if (millis() - lastTimeBotRan > BOT_INTERVAL) 
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  delay(100); // Slight delay to avoid flooding
}
