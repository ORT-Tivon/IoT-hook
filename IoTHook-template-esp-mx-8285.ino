#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "iothook.com"

#ifndef STASSID
#define STASSID "home"
#define STAPSK  "10203040"
#endif

int durum = 32;

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

 /* חיבור לוויפי עם סיסמא ושם רשת */
  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/api/update/"); //HTTP
    http.addHeader("Content-Type", "application/json");
     
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // turn the LED on (HIGH is the voltage level)  
    
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("{\"api_key\":\"a47c1002d88c7d4739804c3a\",\"field_1\":\"" + String(durum) + "\"}");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(60000);

} 
