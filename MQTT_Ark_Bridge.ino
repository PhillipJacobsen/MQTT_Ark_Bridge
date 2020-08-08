/********************************************************************************
    Ark.io BridgeChain IoT Edge Device
    This projects runs on an ESP32 microcontroller 

    Ark_IoT_Edge.ino
    2020 @phillipjacobsen

    Program Features:


********************************************************************************/

/********************************************************************************
                              Private Data & Configuration
  IMPORTANT - Modify the secrets.h file with your network connection and configuration details
********************************************************************************/
#include "secrets.h"

/********************************************************************************
                            Misc I/O Definitions
********************************************************************************/
const int LED_PIN = 13;     //LED integrated on Adafruit HUZZAH32 module
const int BAT_PIN = 35;     //ADC connected to Battery input pin (A13 = 35;)

/********************************************************************************
                              Various Global Variables
********************************************************************************/
bool initialConnectionEstablished_Flag = false;   //used to detect first run after power up


/********************************************************************************
    utlgbotlib Telegram bot Library by JRios Version 1.0.0
    https://github.com/J-Rios/uTLGBotLib-arduino

********************************************************************************/
#include <utlgbotlib.h>

// Create Bot object
//uTLGBot Bot(TLG_TOKEN);



/********************************************************************************
  Time Library
  required for internal clock to syncronize with NTP server.
********************************************************************************/
#include "time.h"


/********************************************************************************
  Update Intervals for various algorithms
********************************************************************************/
uint32_t UpdateInterval_TelegramBot = 3000;           // 3000ms
uint32_t previousUpdateTime_TelegramBot = millis();

/********************************************************************************
   Arduino Json Libary - Tested with version 6.13

********************************************************************************/
//#include <ArduinoJson.h>


/********************************************************************************
  Library for reading/writing to the ESP32 flash memory.
  ESP32 Arduino libraries emulate EEPROM using a sector (4 kilobytes) of flash memory.
  The total flash memory size is ???
  The entire space is split between bootloader, application, OTA data, NVS, SPIFFS, and EEPROM.
  EEPROM library on the ESP32 allows using at most 1 sector (4kB) of flash.
********************************************************************************/
#include <EEPROM.h>

/********************************************************************************
    EspMQTTClient Library by @plapointe6 Version 1.8.0
    WiFi and MQTT connection handler for ESP32
    This library does a nice job of encapsulating the handling of WiFi and MQTT connections.
    https://github.com/plapointe6/EspMQTTClient
    You just need to provide your credentials and it will manage the connection and reconnections to the Wifi and MQTT networks.
    EspMQTTClient is a wrapper around the MQTT PubSubClient Library Version 2.7 by @knolleary
********************************************************************************/
// The MQTT packets are larger then the allowed for the default setting of the libary.
// You need to update this line in PubSubClient.h. Setting it here does nothing.
// If you update this library you will need to update this setting as it will be overwritten.
// #define MQTT_MAX_PACKET_SIZE 512  // the maximum message size, including header, is 128 bytes by default. Configurable in \Arduino\libraries\PubSubClient\src\PubSubClient.h.

#include "EspMQTTClient.h"


EspMQTTClient WiFiMQTTclient(
  WIFI_SSID,
  WIFI_PASS,
  MQTT_SERVER_IP,   // MQTT Broker server ip
  MQTT_USERNAME,    // Can be omitted if not needed
  MQTT_PASSWORD,    // Can be omitted if not needed
  MQTT_CLIENT_NAME, // Client name that uniquely identify your device
  MQTT_SERVER_PORT  // The MQTT port, default to 1883. this line can be omitted
);



/********************************************************************************
  State Machine
********************************************************************************/
enum State_enum {STATE_0, STATE_1, STATE_2, STATE_3, STATE_4, STATE_5, STATE_6};    //The possible states of the state machine
State_enum state = STATE_0;     //initialize the starting state.

/********************************************************************************
  Update Intervals for various algorithms
********************************************************************************/
//Frequency at which the MQTT packets are published
uint32_t UpdateInterval_MQTT_Publish = 10000;           // 10 seconds
uint32_t previousUpdateTime_MQTT_Publish = millis();


/********************************************************************************
  Function prototypes
  Arduino IDE normally does its automagic here and creates all the function prototypes for you.
  We have put functions in other files so we need to manually add some prototypes as the automagic doesn't work correctly
********************************************************************************/



// LED status
uint8_t led_status;



/**************************************************************************************************/

#include <arkCrypto.h>
#include "arkCrypto_esp32.h"  // This is a helper header that includes all the Misc ARK C++ Crypto headers required for this sketch
#include "transactions/builders/radians/radians.hpp"

// Namespaces
using namespace Ark::Crypto;
using namespace Ark::Crypto::identities;
using namespace Ark::Crypto::transactions;


// BridgeChain Network Structure Model.
// These are defined in secrets.h
const Network BridgechainNetwork = {
  BRIDGECHAIN_NETHASH,
  BRIDGECHAIN_SLIP44,
  BRIDGECHAIN_WIF,
  BRIDGECHAIN_VERSION,
  BRIDGECHAIN_EPOCH
};

// Load the Custom Network Configuration
const Configuration cfg(BridgechainNetwork);


/********************************************************************************
  Ark Client Library (version 1.4.1)
********************************************************************************/
#include <arkClient.h>
Ark::Client::Connection<Ark::Client::Api> connection(ARK_PEER, ARK_PORT);   // create ARK blockchain connection



/********************************************************************************
  This structure is used to store details of the bridgechain wallet
********************************************************************************/
struct wallet {
  char walletBalance[64 + 1];             //current balance
  uint64_t walletBalance_Uint64 = 0ULL;   //current balance
  char walletNonce[64 + 1];               //current nonce
  uint64_t walletNonce_Uint64 = 1ULL;     //current nonce
};
struct wallet bridgechainWallet;

char receiveaddress_char[34 + 1];

char mybalanceAddress_char[34 + 1];
char mywalletBalance[64 + 1];             //current balance
uint64_t mywalletBalance_Uint64 = 0ULL;   //current balance


char received_edge_packet[255 + 1];


/********************************************************************************
  Function prototypes
  Arduino IDE normally does its automagic here and creates all the function prototypes for you.
  We have put functions in other files so we need to manually add some prototypes as the automagic doesn't work correctly
********************************************************************************/
void StateMachine();
//void send_MQTTpacket();


/********************************************************************************
  MAIN LOOP
********************************************************************************/
void loop() {

  //--------------------------------------------
  // Process state machine
  StateMachine();

  //--------------------------------------------
  // Handle the WiFi and MQTT connections
  WiFiMQTTclient.loop();

  //--------------------------------------------
  // Publish MQTT data every UpdateInterval_MQTT_Publish (10 seconds)
  //  send_MQTTpacket();


  //--------------------------------------------
  // Check for Telegram Bot received messages
//  if (millis() - previousUpdateTime_TelegramBot > UpdateInterval_TelegramBot)  {
//    previousUpdateTime_TelegramBot += UpdateInterval_TelegramBot;
//    telegramBotHandler();
//  }



}
