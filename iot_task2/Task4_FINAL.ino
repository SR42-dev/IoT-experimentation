#include <WiFi.h>
#include <WebServer.h>

// Defining motor pins on ESP32
#define ena 25
#define enb 26
#define lb 17
#define lf 18
#define rb 19
#define rf 20

#define del 100

//Initializing all motor statuses as LOW (at rest)
bool motor_F = LOW;
bool motor_B = LOW;
bool motor_L = LOW;
bool motor_R = LOW;

const char* ssid = "<wifi_name>";  // Enter SSID here
const char* password = "<password>";  //Enter Password here

//Initiating WebServer
WebServer server(80);          

// Functions to control the direction of the car
void Stop() {
  digitalWrite(lf, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, LOW);
}

void Forward()  {
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  //Setting Direction (FORWARD in this case) status as HIGH and sending data to the HTML page using server.send
  motor_F = HIGH;
  Serial.println("Motor Status: FORWARD");
  server.send(200, "text/html", Home(motor_L, motor_R, true, motor_B));
}

void Left() {
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, HIGH);
  digitalWrite(rb, LOW);
  motor_L = HIGH;
  Serial.println("Motor Status: LEFT");
  server.send(200, "text/html", Home(true, motor_R, motor_F, motor_B));
}

void Right()  {
  digitalWrite(lf, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
  motor_R = HIGH;
  Serial.println("Motor Status: RIGHT");
  server.send(200, "text/html", Home(motor_L, true, motor_F, motor_B));
}

void Backward() {
  digitalWrite(lf, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(rf, LOW);
  digitalWrite(rb, HIGH);
  motor_B = HIGH;
  Serial.println("Motor Status: BACKWARD");
  server.send(200, "text/html", Home(motor_L, motor_R, motor_F, true));
}

// Functions of the webserver
void handle_OnConnect() {
  motor_F = LOW;
  motor_B = LOW;
  motor_L = LOW;
  motor_R = LOW;
  //Motor STOP position
  server.send(200, "text/html", Home(motor_F,motor_B,motor_L,motor_R)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  // Call the respective functions when the link is active.
  server.on("/", handle_OnConnect);
  server.on("/forward", Forward);
  server.on("/backward", Backward);
  server.on("/left", Left);
  server.on("/right", Right);
  server.on("/stop", Stop);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  // Setting Motor Pins up for OUTPUT
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(lf, OUTPUT);
  pinMode(lb, OUTPUT);
  pinMode(rf, OUTPUT);
  pinMode(rb, OUTPUT);

  digitalWrite(ena, HIGH);
  digitalWrite(enb, HIGH);
  
}

// Making the HTML webpage and integrating it with the WebServer
String Home(uint8_t motor_l, uint8_t motor_r, uint8_t motor_f, uint8_t motor_b)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Car control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Car Control</h1>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;

//Depending on the motor direction statuses input by the user, webpage redirection takes place
if(motor_f)
  {ptr +="<p>MOTOR Status: ON</p><a class=\"button button-off\" href=\"/forward\">OFF</a>\n";}
else
  {ptr +="<p>MOTOR Status: OFF</p><a class=\"button button-on\" href=\"/stop\">ON</a>\n";}

if(motor_b)
  {ptr +="<p>MOTOR Status: ON</p><a class=\"button button-off\" href=\"/backward\">OFF</a>\n";}
else
  {ptr +="<p>MOTOR Status: OFF</p><a class=\"button button-on\" href=\"/stop\">ON</a>\n";}

if(motor_l)
  {ptr +="<p>MOTOR Status: ON</p><a class=\"button button-off\" href=\"/left\">OFF</a>\n";}
else
  {ptr +="<p>MOTOR Status: OFF</p><a class=\"button button-on\" href=\"/stop\">ON</a>\n";}

if(motor_r)
  {ptr +="<p>MOTOR Status: ON</p><a class=\"button button-off\" href=\"/right\">OFF</a>\n";}
else
  {ptr +="<p>MOTOR Status: OFF</p><a class=\"button button-on\" href=\"/stop\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
void loop() {
  // Does the needful http server stuff and calls the right callback function (handle_OnConnect()).
    server.handleClient();
}
