
byte statusLed = 13;

byte sensorInterrupt = 1; // 0 = digital pin 2
byte sensorPin = 2;       //------chân tín hiệu

float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;
#define cambien 5
#define relay 4
float gioihan = 350; ///đơn vị mL
void setup()
{

    Serial.begin(9600);
    pinMode(cambien, INPUT_PULLUP);
    pinMode(relay,OUTPUT);
    // Set up the status LED line as an output
    pinMode(statusLed, OUTPUT);
    digitalWrite(statusLed, HIGH); // We have an active-low LED attached

    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void loop()
{

    if ((millis() - oldTime) > 1000) // Only process counters once per second
    {
        detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 500) * 1000;
        totalMilliLitres += flowMilliLitres;

        unsigned int frac;

        Serial.print("Dung tích đã bơm: ");
        Serial.print(totalMilliLitres);
        Serial.println("mL");
        pulseCount = 0;

        attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
    if(digitalRead(cambien) == 0 && totalMilliLitres <=0 ){
      digitalWrite(relay, LOW);
    }
    if(totalMilliLitres >= gioihan){
      digitalWrite(relay, HIGH);
    }
    if(digitalRead(cambien) == 1){
      totalMilliLitres = 0;
      pulseCount = 0;
      flowMilliLitres =0;
      digitalWrite(relay, HIGH);
    }
}

void pulseCounter()
{
    pulseCount++;
}
