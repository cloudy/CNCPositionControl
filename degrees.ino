int CW_PIN = 8;
int CCW_PIN = 9;
int SENSR_HOME = 2;
int SENSR_BACK = 3;
int CUR_POS = 0;
int MOT_SPEED = 2800/2;

bool doTests = false;

void setup() {

  pinMode(CW_PIN, OUTPUT);
  pinMode(CCW_PIN, OUTPUT);
  pinMode(SENSR_HOME, INPUT);
  pinMode(SENSR_BACK, INPUT);
  Serial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(SENSR_HOME), SensorTriggered, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSR_BACK), SensorTriggered, FALLING);

  while (digitalRead(SENSR_HOME) == HIGH)
  {
    digitalWrite(CW_PIN, HIGH);
    delayMicroseconds(MOT_SPEED);
    digitalWrite(CW_PIN, LOW);
    delayMicroseconds(MOT_SPEED);
  }
  
  Serial.println("Found home.");

  if (doTests)
  {
    tests();
  }
}

void tests()
{
   Serial.println("Now doing measure test.");
  
  int steps = 0;
  
  while (digitalRead(SENSR_BACK) == HIGH)
  {
    digitalWrite(CCW_PIN, HIGH);
    delayMicroseconds(MOT_SPEED);
    digitalWrite(CCW_PIN, LOW);
    delayMicroseconds(MOT_SPEED);
    steps++;
  }
  Serial.println("Reached end num steps: ");
  Serial.println(steps);
}


void loop() {
  if (Serial.available() > 0) {
    //delay(5);
    int desmm = Serial.parseInt();

    int selectedpos = desmm - CUR_POS;//abs(round(desmm/0.72/2.0));
    
    for (int i=0; i< abs(round(selectedpos/0.112)); i++) {
      if (selectedpos < 0)
      {
        if (digitalRead(SENSR_HOME) == HIGH)
        {
          digitalWrite(CW_PIN, HIGH);
          delayMicroseconds(MOT_SPEED);
          digitalWrite(CW_PIN, LOW);
          delayMicroseconds(MOT_SPEED);
        }
      }
      else
      {
        if (digitalRead(SENSR_BACK) == HIGH)
        {
          digitalWrite(CCW_PIN, HIGH);
          delayMicroseconds(MOT_SPEED);
          digitalWrite(CCW_PIN, LOW);
          delayMicroseconds(MOT_SPEED);        
        }
      }

      CUR_POS = desmm;
      Serial.println("Current pos");
      Serial.println(CUR_POS);
      
    }
  }
}

void SensorTriggered()
{
  Serial.println("Sensor triggered: is home'd");
  digitalWrite(CW_PIN, LOW);
  digitalWrite(CCW_PIN, LOW);
  
  if (digitalRead(SENSR_HOME) == LOW)
  {
    CUR_POS = 0;
  }
  else
  {
   CUR_POS = round(230/0.112);
  }
}

/* Sensor pinout:
 * Black = output
 * Brown = positive
 * Blue = negative
 * 
 * 
 * 
 * Track length: 23.00 cm 
 * Steps along track: 2805
 * 
 * BUGS:
 * if you enter 0 as a position it starts looping like crazy
 * Consistently off by 0.2 mm 
 * 
 * 
 */
