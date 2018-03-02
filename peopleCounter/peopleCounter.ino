#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManagerMod.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//------------------------PIN SETUP-----------------------------------------------
//reset pin , for reset setting .pull up
#define RESETPIN 14

//-----------------------SYSTEM CONFIG--------------------------------------------
//default values
char mqtt_server[64] = "mqttserver.com";
char mqtt_port[8] = "1883";
char mqtt_user[16] = "user";
char mqtt_pass[16] = "password";
char mqtt_sub_topic[64] = "sub-topic";
char mqtt_pub_topic[64] = "pub-topic";
char mqtt_id[32] = "PeopleCounter_01";
//version
char version[32] = "Ver 1.00b";
//flag for saving data
bool shouldSaveConfig = false;
//time interval to send mqtt
#define INTERVAL 10000
//last send
unsigned long lastSend = millis();

//-------------------------SYSTEM CONTROLLER--------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

//-------------------------JSON DATA----------------------------------------------
// Use arduinojson.org/assistant to compute the capacity.
DynamicJsonBuffer dataJsonBuffer;
JsonObject &dataJson = dataJsonBuffer.createObject();
//data
int doorNumber;
int inCounter;
int outCounter;
int people;
float temperature;
float humidity;
int cabNumber;
char station[64] = "Hemi";

//---------------------------------------------------------------------------------
void initData()
{
  //init data
  doorNumber = 0;
  inCounter = 0;
  outCounter = 0;
  people = 0;
  temperature = 0;
  humidity = 0;
  cabNumber = 0;
  //station[64] = "Hemi";
  lastSend = millis();
}

void updateJson()
{
  dataJson["doorNumber"] = doorNumber;
  dataJson["inCounter"] = inCounter;
  dataJson["outCounter"] = outCounter;
  dataJson["people"] = people;
  dataJson["temperature"] = (int)temperature;
  dataJson["humidity"] = (int)humidity;
  dataJson["cabNumber"] = cabNumber;
  dataJson["station"] = station;
}


int needSendData()
{
  if( millis()-lastSend >=INTERVAL)
  {
    lastSend =millis();
    return 1;
  }
  return 0;
}

void sendData()
{
  char str[270];
  dataJson.printTo(str);
  Serial.println();
  Serial.print(str);
  client.publish(mqtt_pub_topic, str);
}

//read humidity and temperature
void readHumAndTemp()
{
}

//callback notifying us of the need to save config
void saveConfigCallback()
{
  Serial.println("# Set shouldSaveConfig = true");
  shouldSaveConfig = true;
}

//when received data from boker
void callback(char *topic, byte *payload, unsigned int length)
{
  // Conver the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char *)payload;
  unsigned long start;

  Serial.print("Message arrived on topic: [");
  Serial.print(topic);
  Serial.print("], ");
  Serial.println(message);

  if (message == "reset")
  {
    initData();
    Serial.println("Counter data will be reset!.");
    sendData();
  }
}

void setup()
{

  //set mode for reset pin (pull-up)
  pinMode(RESETPIN, INPUT_PULLUP);

  initData();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Startingup....");

  WiFiManager wifiManager;

  //reset ap and SPIFFS
  if (digitalRead(RESETPIN) == LOW)
  {
    Serial.println("# Reset Ap setting ...");
    wifiManager.resetSettings();
    SPIFFS.format();
  }

  //parse version infor to portal
  wifiManager.setVersion(version);

  //read configuration from FS json
  Serial.println("# Mounting FS...");

  if (SPIFFS.begin())
  {
    Serial.println("# Mounted file system");
    if (SPIFFS.exists("/config.json"))
    {
      //file exists, reading and loading
      Serial.println("# Reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("# Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success())
        {
          Serial.println("\n# parsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
          strcpy(mqtt_sub_topic, json["mqtt_sub_topic"]);
          strcpy(mqtt_pub_topic, json["mqtt_pub_topic"]);
          strcpy(mqtt_id, json["mqtt_id"]);
        }
        else
        {
          Serial.println("# Failed to load json config");
        }
      }
    }
  }
  else
  {
    Serial.println("# Failed to mount FS");
  }
  //end read

  // The extra parameters
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 63);
  WiFiManagerParameter custom_mqtt_port("port", "port", "1883", 7, " type='number'");
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", "", 15);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", "", 15, " type='password'");
  WiFiManagerParameter custom_mqtt_sub_topic("sub", "subscribe topic", "", 63);
  WiFiManagerParameter custom_mqtt_pub_topic("pub", "publish topic", "", 63);
  WiFiManagerParameter custom_mqtt_id("id", "mqtt id", mqtt_id, 31);

  //Custom text parameters
  WiFiManagerParameter custom_text_mqtt("<h2>MQTT Setting</h2>");

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //add parameters to wifiManager
  wifiManager.addParameter(&custom_text_mqtt);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);
  wifiManager.addParameter(&custom_mqtt_sub_topic);
  wifiManager.addParameter(&custom_mqtt_pub_topic);
  wifiManager.addParameter(&custom_mqtt_id);

  if (!wifiManager.autoConnect("ESP8266", "password"))
  {
    Serial.println("# Failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("# Connected !");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_user, custom_mqtt_user.getValue());
  strcpy(mqtt_pass, custom_mqtt_pass.getValue());
  strcpy(mqtt_sub_topic, custom_mqtt_sub_topic.getValue());
  strcpy(mqtt_pub_topic, custom_mqtt_pub_topic.getValue());
  strcpy(mqtt_id, custom_mqtt_id.getValue());

  //display debug parameters
  Serial.println("#DEBUG : Updated parameters");
  Serial.print("\tMQTT Server :");
  Serial.println(mqtt_server);
  Serial.print("\tMQTT Port :");
  Serial.println(mqtt_port);
  Serial.print("\tMQTT User :");
  Serial.println(mqtt_user);
  Serial.print("\tMQTT Password :");
  Serial.println(mqtt_pass);
  Serial.print("\tMQTT Subscribe topic :");
  Serial.println(mqtt_sub_topic);
  Serial.print("\tMQTT Publish topic :");
  Serial.println(mqtt_pub_topic);
  Serial.print("\tMQTT Id :");
  Serial.println(mqtt_id);

  //save the custom parameters to FS
  if (shouldSaveConfig)
  {
    Serial.println("# Saving config...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;
    json["mqtt_sub_topic"] = mqtt_sub_topic;
    json["mqtt_pub_topic"] = mqtt_pub_topic;
    json["mqtt_id"] = mqtt_id;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("# Failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
    Serial.println("# Save successed!");
  }

  Serial.print("# Local ip : ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("# Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(mqtt_id, mqtt_user, mqtt_pass))
    {
      Serial.println("# Connected to MQTT server");
      //subscribe
      client.subscribe(mqtt_sub_topic, 1);
    }
    else
    {
      Serial.print("\tFailed, rc=");
      Serial.print(client.state());
      Serial.println("\tTry again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
  //MQTT connect
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  //counter sensor code goes here 


  //publish code goes here
  if(needSendData())
  {
    //read lastest hum and temp data
    readHumAndTemp();
    //update sjon data
    updateJson();
    //send it
    sendData();
  }

}
