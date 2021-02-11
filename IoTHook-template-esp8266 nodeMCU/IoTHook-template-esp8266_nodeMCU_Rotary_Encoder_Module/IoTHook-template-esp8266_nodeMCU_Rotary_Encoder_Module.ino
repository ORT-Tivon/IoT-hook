#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "iothook.com"

#ifndef STASSID
#define STASSID "home"
#define STAPSK  "10203040"
#endif

#define outputA D4
#define outputB D3
#define outputC D5

int counter = 0; 
int aState;
int aLastState;  
int pressed; 
String pres = "";
unsigned long lastButtonPress = 0;

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

   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   pinMode (outputC,INPUT_PULLUP);
}

void loop() {
  /* אם חיבור הוויפיי עובד */
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // הגדר את קריאת הhttp לשרת
    http.begin(client, "http://" SERVER_IP "/api/update/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    // קרא את המקודד המעגלי
    aState = digitalRead(outputA); 
    if (aState != aLastState){     
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
    aLastState = aState;
    }
    pressed = digitalRead(outputC);
    if (pressed == LOW) {
    if (millis() - lastButtonPress > 50) {
      pres = "bob";
    }
    pres = "";
    lastButtonPress = millis();
   }
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("{\"api_key\":\"1bccf7170a87abc6aef4f71a\",\"field_1\":\"" + String(counter) + "\"}");
    //int httpCode = http.POST("{\"api_key\":\"1bccf7170a87abc6aef4f71a\",\"field_2\":\"" + pres + "\"}");
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
  delay(100000); // עשר שניות בין שליחה לשליחה 
}
