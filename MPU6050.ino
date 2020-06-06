/**
 * MPU6050 setup
 */
void initializeMPU6050(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

/*
 * Reads and store the current XYZ angles in the given array.
 * @param anglesArray Array to store the read angles (angles[] or armedAngles[])
 */
void readAngles(double anglesArray[]){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  
  anglesArray[0] = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  anglesArray[1] = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  anglesArray[2] = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


  // When MPU6050 has some power problem or resets or bricks, it gives 225 for all the angles.
  // We detect if it has bricked, and re-initialize it.
  if(round(anglesArray[0]) == 225 && round(anglesArray[1]) == 225 && round(anglesArray[2]) == 225){
    initializeMPU6050();
    delay(1000);  // We wait 1 second before call this same function to read the angles once MPU has re-initialized
    readAngles(anglesArray);  // We call this function again to read the real angles, otherwise alarm would blow with the fake 225 angles read.
  }

  if(false){
    printArmedAngles();
    printAngles();
  }
  
}

/*
 * Prints stored angles
 */
void printAngles(){
  Serial.println("----------- ANGLES ------------");
  Serial.print("AngleX: ");
  Serial.println(angles[0]);
   
  Serial.print("AngleY: ");
  Serial.println(angles[1]);
   
  Serial.print("AngleZ: ");
  Serial.println(angles[2]);
  Serial.println("-------------------------------");
}

/*
 * Prints stored armedAngles
 */
void printArmedAngles(){
  Serial.println("-------- ARMED ANGLES ---------");
  Serial.print("ArmedAngleX: ");
  Serial.println(armedAngles[0]);
   
  Serial.print("ArmedAngleY: ");
  Serial.println(armedAngles[1]);
   
  Serial.print("ArmedAngleZ: ");
  Serial.println(armedAngles[2]);
  Serial.println("-------------------------------");
}


/*
 * Compares angles[] and armedAngles[]
 */
boolean compareStoredAngles(){
  readAngles(angles); // Update current angles information from gyroscope

  return ((diffBetweenTwoAngles(armedAngles[0], angles[0]) > xThreshold) || 
          (diffBetweenTwoAngles(armedAngles[1], angles[1]) > yThreshold));
}

/*
 * Returns the angle difference in decimal degrees between two given angles.
 * Formula used: min(abs(A-B), min(A + abs(360-B), abs(360-A) + B))
 * @param angle1 First angle to compare
 * @param angle2 Second angle to compare
 */
float diffBetweenTwoAngles(float angle1, float angle2){
  return min(abs(angle1 - angle2), min(angle1 + abs(360-angle2), abs(360-angle1) + angle2));
}
