
#define MQTT_SERVER_IP    "40.85.223.207"
#define MQTT_USERNAME     "MQTTedge"
#define MQTT_PASSWORD     "MagpieCrow77"
//#define MQTT_CLIENT_NAME  "TestClient"
char* MQTT_CLIENT_NAME =  "TestClient";
#define MQTT_SERVER_PORT  1883

//h
#define WIFI_SSID         "TELUS0357"
#define WIFI_PASS         "77kmm7r7hz"

//w
//#define WIFI_SSID         "TELUS6428"
//#define WIFI_PASS         "3mmkgc9gn2"

//int8_t TIME_ZONE = -6;      //set timezone:  MST (use this in summer)
int8_t TIME_ZONE = -7;        //set timezone:  MST (use this in winter)
int16_t DST = 3600;           //To enable Daylight saving time set it to 3600. Otherwise, set it to 0. I am not sure if this actually works. Need to confirm in the fall...

// Configure Radians Wallet for Faucet
const char* FaucetAddress = "TJ4shh6Nkz5tFxqDibC9uZwWqrm8HeeoXP";   //RADIANS testnet address that holds faucet funds
static const auto PASSPHRASE  = "resist once mirror smile witness rent ozone angry retire gym draw kid";

//Configure Bridgechain Relay
//const char* ARK_PEER = "37.34.60.90";  //RADIANS Testnet Forging Node
const char* ARK_PEER = "138.197.165.189";  //RADIANS Testnet Relay - PJ's

int ARK_PORT = 4040;

// Configure Bridgechain Parameters
static const auto BRIDGECHAIN_NETHASH   = "314ccfc8c437e10cccb527ee6726be606da8fbaebe54c5c105df30882511c25a"; // std::string
static const auto BRIDGECHAIN_SLIP44    = 1;          // uint8_t
static const auto BRIDGECHAIN_WIF       = 0x91;       // uint8_t      145
static const auto BRIDGECHAIN_VERSION   = 0x41;       // uint8_t      65
static const auto BRIDGECHAIN_EPOCH     = "2020-05-12T11:34:19.156Z";  // std::string
