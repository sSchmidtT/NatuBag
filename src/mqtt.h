#include <PubSubClient.h>
#include <ArduinoJson.h>
// MQTTClient client;
PubSubClient client(net);

void messageReceivedPubSub(char *topic, byte *payload, unsigned int length)
{
  String recebido = "";
  Serial.print("Recebido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    recebido +=( char)payload[i];
  }
  Serial.println(recebido);
  Serial.println();
  if(recebido == "\"INATTENDANCE\""){
    Serial.println("Entrou no If");
    if(inQueue){
      onSuccess();
      ledState = 1;
      inQueue = false;
      inAttendance = true;
    }
    
  }else if (recebido == "\"FINALIZED\"")
  {
    inQueue = false;
    inAttendance = false;
    ledState = 0;
    onInitial();
  }
  

}

void pubSubErr(int8_t MQTTErr)
{
  if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
    Serial.print("Connection tiemout");
  else if (MQTTErr == MQTT_CONNECTION_LOST)
    Serial.print("Connection lost");
  else if (MQTTErr == MQTT_CONNECT_FAILED)
    Serial.print("Connect failed");
  else if (MQTTErr == MQTT_DISCONNECTED)
    Serial.print("Disconnected");
  else if (MQTTErr == MQTT_CONNECTED)
    Serial.print("Connected");
  else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
    Serial.print("Connect bad protocol");
  else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
    Serial.print("Connect bad Client-ID");
  else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
    Serial.print("Connect unavailable");
  else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
    Serial.print("Connect bad credentials");
  else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
    Serial.print("Connect unauthorized");
}

void connectToMqtt(bool nonBlocking = false)
{
  Serial.print("MQTT conectando");
  while (!client.connected())
  {
    Serial.print(".");
    delay(1000);

    if (client.connect(THINGNAME, MQTT_USER, NULL))
    {
      Serial.print("");
      Serial.println("conectado!");
      if (!client.subscribe(MQTT_SUB_TOPIC))
        pubSubErr(client.state());
    }
    else
    {
      Serial.print("SSL Error Code: ");
      Serial.println(net.getLastSSLError());
      Serial.print("failed, reason -> ");
      pubSubErr(client.state());
      if (!nonBlocking)
      {
        Serial.println(" < tentando novamente em 5 segundos");
        delay(5000);
      }
      else
      {
        Serial.println(" <");
      }
    }
    if (nonBlocking)
      break;
  }
}

void sendDataAddProduct(String rfid)
{
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 100);
  JsonObject root = jsonBuffer.to<JsonObject>();
  JsonObject payload = root.createNestedObject("payload");

  root["schema"] = "natubag_product";
  payload["id_device"] = id_device;
  payload["product_rfid"] = rfid;
  payload["at_incl"] = ctime(&now);
  payload["status"] = "INCL";


  Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
  serializeJson(root, Serial);
  Serial.println();
  char shadow[measureJson(root) + 1];
  serializeJson(root, shadow, sizeof(shadow));
  if (!client.publish(MQTT_PUB_TOPIC, shadow, false))
    pubSubErr(client.state());
}

void sendDataAddQueue(bool status)
{
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 100);
  JsonObject root = jsonBuffer.to<JsonObject>();
  JsonObject payload = root.createNestedObject("payload");

  root["schema"] = "natubag_queue";
  payload["id_device"] = id_device;
  payload["at_incl"] = ctime(&now);
  payload["status"] = status? "INCL" : "REMO";


  Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
  serializeJson(root, Serial);
  Serial.println();
  char shadow[measureJson(root) + 1];
  serializeJson(root, shadow, sizeof(shadow));
  if (!client.publish(MQTT_PUB_TOPIC, shadow, false))
    pubSubErr(client.state());
}

void sendDataInitialBag(bool active)
{
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 100);
  JsonObject root = jsonBuffer.to<JsonObject>();
  JsonObject payload = root.createNestedObject("payload");

  root["schema"] = "natubag_dispo";
  payload["id_device"] = id_device;
  payload["active"] = active;
  payload["at_event"] = ctime(&now);

  Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
  serializeJson(root, Serial);
  Serial.println();
  char shadow[measureJson(root) + 1];
  serializeJson(root, shadow, sizeof(shadow));
  if (!client.publish(MQTT_PUB_TOPIC, shadow, false))
    pubSubErr(client.state());
}

void setupMQTT() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(messageReceivedPubSub);
}