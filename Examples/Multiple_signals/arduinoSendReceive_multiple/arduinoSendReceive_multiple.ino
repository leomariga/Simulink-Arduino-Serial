// Create a union to easily convert float to byte
typedef union{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

// Create the variable you want to send
FLOATUNION_t myValue1;
FLOATUNION_t myValue2;
FLOATUNION_t myValue3;
FLOATUNION_t myValue4;
FLOATUNION_t myValue5;
FLOATUNION_t myValue6;

FLOATUNION_t send1;
FLOATUNION_t send2;
FLOATUNION_t send3;

void setup() {
  // initialize serial, use the same boudrate in the Simulink Config block
  Serial.begin(115200);

}
void loop(){
  myValue1.number = getFloat(); // Give your float a value
  myValue2.number = getFloat(); // Give your float a value
  myValue3.number = getFloat(); // Give your float a value
  myValue4.number = getFloat(); // Give your float a value
  myValue5.number = getFloat(); // Give your float a value
  myValue6.number = getFloat(); // Give your float a value

  send1.number = myValue1.number+myValue2.number;
  send2.number = myValue3.number+myValue4.number;
  send3.number = myValue5.number+myValue6.number;
  
  // Print header: Important to avoid sync errors!
  Serial.write('A'); 
  
  // Print float data
  for (int i=0; i<4; i++){
    Serial.write(send1.bytes[i]); 
  }
  for (int i=0; i<4; i++){
    Serial.write(send2.bytes[i]); 
  }
  for (int i=0; i<4; i++){
    Serial.write(send3.bytes[i]); 
  }
  // Print terminator
  Serial.print('\n');
  
  // Use the same delay in the Serial Receive block
  delay(50);
}


float getFloat(){
    int cont = 0;
    FLOATUNION_t f;
    while (cont < 4 ){
        f.bytes[cont] = Serial.read() ;
        cont = cont +1;
    }
    return f.number;
}
