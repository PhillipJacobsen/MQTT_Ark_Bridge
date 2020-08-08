/********************************************************************************
  This file contains functions used to configure hardware peripherals and various libraries.
********************************************************************************/

/********************************************************************************
  Configure peripherals and system
********************************************************************************/
void setup()
{

  //--------------------------------------------
  // Enable serial port and I/O
  Serial.begin(115200);           // Initialize Serial Connection for debug
  while ( !Serial && millis() < 20 );

  digitalWrite(LED_PIN, HIGH);    // Turn LED on
  pinMode(LED_PIN, OUTPUT);       // initialize on board LED control pin as an output.
  led_status = 0;

  // Optional Features of EspMQTTClient
  WiFiMQTTclient.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  //  WiFiMQTTclient.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overwritten with enableHTTPWebUpdater("user", "password").
  WiFiMQTTclient.enableLastWillMessage("radians_faucet/status", "false");  // You can activate the retain flag by setting the third parameter to true
  WiFiMQTTclient.enableMQTTConnect();
  WiFiMQTTclient.enableMACaddress_for_ClientName();


}


/********************************************************************************

********************************************************************************/
// Handler for receiving MQTT message
void MQTT_Request_Handler (const String & payload) {
  Serial.print("received MQTT topic radians_edge/request: ");
  Serial.println(payload);

  String request = String(payload);

  String receiveaddress = request;
  Serial.print("\nreceiveAddress: ");
  Serial.print(receiveaddress);
  receiveaddress.toCharArray(received_edge_packet, 255 + 1);

  WiFiMQTTclient.publish("radians_edge/response", "received request");

  //--------------------------------------------
  // Retrieve Wallet Nonce from blockchain before sending transaction
  getWallet();
  //--------------------------------------------

  Serial.print("\nreceiveAddress char: ");
  Serial.print(receiveaddress_char);

  //--------------------------------------------
  sendBridgechainTransaction_MQTTrequest();

}




/********************************************************************************
  This function is called once WiFi and MQTT connections are complete
********************************************************************************/
void onConnectionEstablished() {

  if (!initialConnectionEstablished_Flag) {     //execute this the first time we have established a WiFi and MQTT connection after powerup
    initialConnectionEstablished_Flag = true;

    //--------------------------------------------
    //  sync local time to NTP server
    configTime(TIME_ZONE * 3600, DST, "pool.ntp.org", "time.nist.gov");

    //--------------------------------------------
    //  display IP address.
    Serial.println("\nIP address: ");
    Serial.println(WiFi.localIP());


    // Subscribe to "radians_faucet/request" via alternate callback format
    WiFiMQTTclient.subscribe("radians_edge/request", MQTT_Request_Handler);

    WiFiMQTTclient.publish("radians_edge/status", "true");

    Serial.println("");
    Serial.println(WiFiMQTTclient.getMqttClientName());
    Serial.println(WiFiMQTTclient.getMqttServerIp());
    Serial.println(WiFiMQTTclient.getMqttServerPort());
    Serial.println(WiFiMQTTclient.getConnectionEstablishedCount());


    //wait for time to sync from NTP server
    while (time(nullptr) <= 100000) {
      delay(20);
    }

    time_t now = time(nullptr);   //get current time
    struct tm * timeinfo;
    time(&now);
    timeinfo = localtime(&now);

    Serial.print("Unix epoch time is: ");
    Serial.println(now);
    char formattedTime [30];
    strftime (formattedTime, 30, "%r", timeinfo); // http://www.cplusplus.com/reference/ctime/strftime/
    Serial.print("Regular time is: ");
    Serial.println(formattedTime);


  }

  else {

    // Subscribe to "radians_faucet/request" via alternate callback format
    WiFiMQTTclient.subscribe("radians_edge/request", MQTT_Request_Handler);
    WiFiMQTTclient.publish("radians_edge/status", "true", true); //set retain = true
  }

}
