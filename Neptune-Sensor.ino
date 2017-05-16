#include <WiFi.h>

const char* ssid = "Carlin";
const char* password = "ayylamoo";

const char* host = "34.207.150.80";//"auxo-server-test.herokuapp.com";
//int sensorValue = 0;

int NumDigits(int x)  
{  
    x = abs(x);  
    return (x < 10 ? 1 :   
        (x < 100 ? 2 :   
        (x < 1000 ? 3 :   
        (x < 10000 ? 4 :   
        (x < 100000 ? 5 :   
        (x < 1000000 ? 6 :   
        (x < 10000000 ? 7 :  
        (x < 100000000 ? 8 :  
        (x < 1000000000 ? 9 :  
        10)))))))));  
}

void sendPOSTRequest(int sv) {
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.println();

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
  }

  // We now create a URI for the request
  String url = "/sensor";

  Serial.print("Requesting URL: ");
  Serial.print(url);
  Serial.println();

  Serial.print("Sensor Value: ");
  Serial.print(sv);
  Serial.println();
  int contentLength = NumDigits(sv);
  contentLength += 9;

  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "Content-Length: " + contentLength + "\r\n\r\n" +
               "moisture=" + sv + "\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 15000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  
  //sensorValue = analogRead(14);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(32);
  Serial.println(sensorValue);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(sensorValue > 0) {
    sensorValue = rand() % 1095 + 3001;
  }
  sendPOSTRequest(analogRead(32));
  WiFi.disconnect(false);
  Serial.println("WiFi Disconnected");
  delay(1000);
}
