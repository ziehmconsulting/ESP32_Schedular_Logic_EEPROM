#include <WiFi.h>
#include <Arduino_Mqtt_Client.h>  //Version @^0.1.8
#include <ThingsBoard.h>  //Version @0.12.2
#include <ArduinoOTA.h> //(Uncomment if you are using PlatformIO)
// include library to read and write from flash memory
#include <EEPROM.h>

//-------------------------EEPROM-Settings-------------------------
// define the number of bytes you want to access
#define EEPROM_SIZE 1

//-----------------------------------------------------------------

// Wi-Fi and ThingsBoard configuration
constexpr char WIFI_SSID[] = "Vodafone-6D9D";
constexpr char WIFI_PASSWORD[] = "euGBvU4DApy6NvLC";
constexpr char TOKEN[] = "kTFbPCc8obSDWqiGPvye";
constexpr char THINGSBOARD_SERVER[] = "142.132.168.158";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

// Pin definition for the LED
constexpr uint8_t LED_PIN = 14;
int BAUD = 9600;

// Global variables
// Variables will change:
bool led_actual_State ;         // LED state
bool eeprom_LED_State ;
bool subscribed = false;        // Indicates if RPC subscription is done
bool state_changed = false;
bool first_loop = true;

// Initialize WiFi and MQTT clients
WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

// Function declarations
void initWiFi();
bool reconnect();
RPC_Response processSetLedStatus(const RPC_Data &data);
void processTime(const JsonVariantConst& data);

// Define the array of RPC callbacks
const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ "setLedStatus", processSetLedStatus }
};

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD);
  delay(100);

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);

  // Set the LED pin as output
  pinMode(LED_PIN, OUTPUT);



  // read the last LED state from flash memory
  delay(100);
  // set the LED to the last stored state
  digitalWrite(LED_PIN, EEPROM.read(0));

  // Small delay to ensure the serial monitor is ready
  delay(100);

  // Initialize Wi-Fi connection
  initWiFi();
}

void loop() {
  // Small delay to avoid overwhelming the loop
  delay(10);

  // Attempt to reconnect if the Wi-Fi connection is lost
  if (!reconnect()) {
    return;
  }

  // Check if we are connected to ThingsBoard
  if (!tb.connected()) {
    // Attempt to connect to ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }

    // Subscribe to RPC callbacks
    Serial.println("Subscribing for RPC...");
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }
  }

  if (first_loop == true ){
    digitalWrite(LED_PIN, EEPROM.read(0));
    first_loop = false;
    Serial.println("First Loop Done");
  }


  if (led_actual_State != eeprom_LED_State && first_loop == false){
    eeprom_LED_State = led_actual_State;
    EEPROM.write(0, eeprom_LED_State); //store at eeprom
    EEPROM.commit();
    digitalWrite(LED_PIN, EEPROM.read(0));
    Serial.printf("changed State: %d",eeprom_LED_State);
    Serial.println("");
  }
  delay(50);


  // Control the LED state
  //digitalWrite(LED_PIN, led_actual_State ? true : false);



  delay(50);

  // Request the current time if not already subscribed
  if (!subscribed) {
    Serial.println("Requesting RPC...");
    RPC_Request_Callback timeRequestCallback("getCurrentTime", processTime);
    if (!tb.RPC_Request(timeRequestCallback)) {
      Serial.println("Failed to request for RPC");
      return;
    }
    Serial.println("Request done");
    subscribed = true;
  }

  // Maintain the connection and process incoming messages
  tb.loop();

}

void initWiFi() {
  // Start the connection to the Wi-Fi network
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

bool reconnect() {
  // Check the Wi-Fi connection status and reconnect if necessary
  if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
  }
  return WiFi.status() == WL_CONNECTED;
}

RPC_Response processSetLedStatus(const RPC_Data &data) {
  // Process the RPC request to change the LED state
  int dataInt = data;
  led_actual_State = dataInt == 1;  // Update the LED state based on the received data
  Serial.println(led_actual_State ? "LED ON" : "LED OFF");
  return RPC_Response("newStatus", dataInt);  // Respond with the new status
}

void processTime(const JsonVariantConst& data) {
  // Process the RPC response containing the current time
  Serial.print("Received time from ThingsBoard: ");
  Serial.println(data["time"].as<String>());
}

