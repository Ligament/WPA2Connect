#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}

static const char* ssid = "@SSID";
static const char* username = "username";
static const char* password = "password";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH); 
  if (WiFi.status() != WL_CONNECTED)
  {    
    wifi_set_opmode(STATION_MODE);
    
    struct station_config wifi_config;
    
    memset(&wifi_config, 0, sizeof(wifi_config));
    strcpy((char*)wifi_config.ssid, ssid);
    
    wifi_station_set_config(&wifi_config);
    
    wifi_station_clear_cert_key();
    wifi_station_clear_enterprise_ca_cert();
    
    wifi_station_set_wpa2_enterprise_auth(1);
    wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
    wifi_station_set_enterprise_username((uint8*)username, strlen(username));
    wifi_station_set_enterprise_password((uint8*)password, strlen(password));
    
    wifi_station_connect(); 
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.write('*');
      delay(250);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(250);
    }

    if (WiFi.status() != WL_CONNECTED)
      ESP.restart();

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
