// Create a union to easily convert float to byte
typedef union{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

// Create the variable you want to send
FLOATUNION_t myValue;

void setup() {
  // initialize serial, use the same boudrate in the Simulink Config block
  Serial.begin(115200);

}
void loop(){
  myValue.number = getFloat(); // Give your float a value
  
  // Print header: Important to avoid sync errors!
  Serial.write('A'); 
  
  // Print float data
  for (int i=0; i<4; i++){
    Serial.write(myValue.bytes[i]); 
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
