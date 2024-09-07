#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#include <WiFiClientSecure.h> // Use this for secure connections

#define DHTPIN 4
#define PIRPIN 2
#define BUZZERPIN 16
#define SERVOPIN 18
#define SOUNDPIN 34
//#define LDRPIN 15
#define LEDPIN 5
#define I2C_SDA 21  // Replace with your SDA pin
#define I2C_SCL 22  // Replace with your SCL pin

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "cb7241bddd9347ccb8b1403f41271f2b.s1.eu.hivemq.cloud";  // HiveMQ public broker
const char* mqtt_username = "hivemq.webclient.1724159031696";
const char* mqtt_password = "20egJl61p&:T,x.UPXRu";
const int mqtt_port = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHTesp dht;
Servo myservo;

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";
unsigned long lastMsg = 0;
int value = 0;

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int tempThreshold = 37;
int humidityThreshold = 80;
int soundThreshold = 5000;
//int ldrThreshold = 1000;
int motionDetected = 0;

String doorPassword = "1234";
String input = "";
bool doorLocked = false;

// Function declarations
void handleKeypadInput(char key);
void monitorSensors();
void monitorTemperatureHumidity();
void monitorMotion();
void monitorSound();
//void monitorLight();
void buzzAlert();
void buzzPattern(int delayTime, int repeat);

void setup_wifi() {
  delay(10);
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client";
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("garage/door");
      client.publish("ESP32Client","connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  // Secure connection
  espClient.setCACert(root_ca);

  client.setServer(mqtt_server, mqtt_port);

  lcd.init();
  lcd.backlight();
  dht.setup(DHTPIN, DHTesp::DHT22);
  myservo.attach(SERVOPIN);

  pinMode(PIRPIN, INPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(SOUNDPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  //pinMode(LDRPIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  // Calibrate sound sensor
  int calibrationDuration = 5000;
  long soundSum = 0;
  int soundSamples = 0;
  long calibrationStart = millis();

  while (millis() - calibrationStart < calibrationDuration) {
    int soundValue = analogRead(SOUNDPIN);
    soundSum += soundValue;
    soundSamples++;
    delay(100);
  }

  soundThreshold = (soundSum / soundSamples) + 100;
  Serial.print("Sound threshold set to: ");
  Serial.println(soundThreshold);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Monitor sensors
  monitorSensors();

  char key = keypad.getKey();
  if (key) {
    handleKeypadInput(key);
  }

  // Publish readings to MQTT topics
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    TempAndHumidity data = dht.getTempAndHumidity();
    String tempStr = String(data.temperature, 2);
    String humStr = String(data.humidity, 2);

    client.publish("home/temperature", tempStr.c_str());
    client.publish("home/humidity", humStr.c_str());
    //client.publish("home/light", String(analogRead(LDRPIN)).c_str());
    client.publish("home/motion", motionDetected == HIGH ? "Motion Detected" : "No Motion");
    client.publish("home/sound", String(analogRead(SOUNDPIN)).c_str());

    lcd.setCursor(0, 0);
    lcd.print("Temp: " + tempStr + " C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: " + humStr + " %");
  }
}

void handleKeypadInput(char key) {
  if (key == '#') {
    if (input == doorPassword) {
      if (doorLocked) {
        myservo.write(0);  // Unlock the door
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Door Unlocked");
        doorLocked = false;
        buzzPattern(100, 3);
      } else {
        myservo.write(90);  // Lock the door
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Door Locked");
        doorLocked = true;
        buzzPattern(100, 2);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password");
      buzzPattern(500, 1);
    }
    input = "";  // Clear the input after processing
  } else if (key == '*') {
    input = "";  // Clear input
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Input Cleared");
  } else {
    input += key;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Pass: ");
    lcd.setCursor(0, 1);
    lcd.print(input);
  }
}

void monitorSensors() {
  monitorTemperatureHumidity();
  monitorMotion();
  monitorSound();
  //monitorLight();
}

void monitorTemperatureHumidity() {
  TempAndHumidity data = dht.getTempAndHumidity();
  if (data.temperature > tempThreshold || data.humidity > humidityThreshold) {
    buzzAlert();
    lcd.setCursor(0, 1);
    lcd.print("Threshold Exceeded");
  }
}

void monitorMotion() {
  motionDetected = digitalRead(PIRPIN);
  if (motionDetected == HIGH) {
    buzzAlert();
    lcd.setCursor(0, 1);
    lcd.print("Motion Detected");
  }
}

void monitorSound() {
  int soundValue = analogRead(SOUNDPIN);
  if (soundValue > soundThreshold) {
    buzzAlert();
    lcd.setCursor(0, 1);
    lcd.print("Loud Sound Detected");
  }
}

/* void monitorLight() {
  int ldrValue = analogRead(LDRPIN);
  if (ldrValue < ldrThreshold) {
    buzzAlert();
    lcd.setCursor(0, 1);
    lcd.print("Light Threshold");
  }
} */

void buzzAlert() {
  digitalWrite(BUZZERPIN, HIGH);
  delay(500);
  digitalWrite(BUZZERPIN, LOW);
  delay(500);
}

void buzzPattern(int delayTime, int repeat) {
  for (int i = 0; i < repeat; i++) {
    digitalWrite(BUZZERPIN, HIGH);
    delay(delayTime);
    digitalWrite(BUZZERPIN, LOW);
    delay(delayTime);
  }
}
