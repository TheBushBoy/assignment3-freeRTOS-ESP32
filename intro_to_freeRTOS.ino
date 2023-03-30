// Pin definition
const byte T1_Pin = 0;
const byte T2_Pin = 19;
const byte T3_Pin = 18;
const byte T4_Pin = 3;
const byte btn_Pin = 6;
const byte led_Pin = 7;

// Maximum values
const unsigned int maxValue = 3300;
const unsigned int freq1Min = 333;
const unsigned int freq2Min = 500;
const unsigned int freqMax = 1000;

// Values measured
float freq1 = 0;
float freq2 = 0;
unsigned int avg = 0;

// Store 4 values for the task 4
unsigned int values[4];

// Button parameters for task6
bool state = 1;
unsigned short btn = LOW;
bool oldBtn = 0;
bool pressed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(T1_Pin, OUTPUT);
  pinMode(T2_Pin, INPUT);
  pinMode(T3_Pin, INPUT);
  pinMode(T4_Pin, INPUT);
  pinMode(btn_Pin, INPUT_PULLUP);
  pinMode(led_Pin, OUTPUT);
}

void task1() {
  // High for 200us
  digitalWrite(T1_Pin, HIGH);
  delayMicroseconds(200);
  
  // Low for 50us
  digitalWrite(T1_Pin, LOW);
  delayMicroseconds(50);
  
  // High for 30us
  digitalWrite(T1_Pin, HIGH);
  delayMicroseconds(30);

  digitalWrite(T1_Pin, LOW);
}

void task2() {
  // Getting the duration of a pulse, multiplied by 2 to get the period
  unsigned int pulseTime = pulseIn(T2_Pin, HIGH, 2500);
  freq1 = (pulseTime == 0) ? 0 : 1e6/(2*pulseTime);
}

void task3() {
  // Getting the duration of a pulse, multiplied by 2 to get the period
  unsigned int pulseTime = pulseIn(T3_Pin, HIGH, 2200);
  freq2 = (pulseTime == 0) ? 0 : 1e6/(2*pulseTime);
}

void task4() {
  unsigned short i;

  // Shifting the former values the storing the new one
  values[3] = values[2];
  values[2] = values[1];
  values[1] = values[0];
  values[0] = analogRead(T4_Pin);

  // Calculating the average of the last 4 values
  for(i = 0; i < 4; i++) {
    avg += values[i];
  }
  avg /= i+1;

  // Turn on the led if the average is greater than the half of the maximum value
  if(avg >= maxValue /2) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void task5() {
  // Scaling the values beetween 0 and 99
  int freq1Scaled = (freq1 - freq1Min) / (freqMax - freq1Min) * 99;
  int freq2Scaled = (freq2 - freq2Min) / (freqMax - freq2Min) * 99;

  freq1Scaled = (freq1Scaled < 0) ? 0 : freq1Scaled;
  freq1Scaled = (freq1Scaled > 99) ? 99 : freq1Scaled;
  freq2Scaled = (freq2Scaled < 0) ? 0 : freq2Scaled;
  freq2Scaled = (freq2Scaled > 99) ? 99 : freq2Scaled;

  Serial.printf("%d,%d\n", freq1Scaled, freq2Scaled);
}

void task6() {
  btn = digitalRead(btn_Pin);
  
  // oldBtn is used to prevent the button from bouncing
  if(btn && !oldBtn) { 
    oldBtn = 1;
    pressed = 1;
  }
  // When button is pressed, we change the state
  if(pressed) {
    state = (state + 1) % 2;
    pressed = 0;
  }
  else if(!btn && oldBtn) {
    oldBtn = 0;
  }
}

void task7() {
  if(state) {
    digitalWrite(led_Pin, HIGH);
  }
  else {
    digitalWrite(led_Pin, LOW);
  }
}

void loop() {
  task6();
  task7();
}
