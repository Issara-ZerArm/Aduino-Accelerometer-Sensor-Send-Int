

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> 


float xSensor = 0;
float ySensor = 0;
float zSensor = 0;

int xx100;
int yx100;
int zx100;

int powx;
int powy;
int powz;

int sumxyz;

int xBLE;
int yBLE;
int zBLE;

int sumsqrt;

BLEService SensorService("1101");
BLEUnsignedIntCharacteristic XChar("2101", BLERead | BLENotify);
BLEUnsignedIntCharacteristic YChar("2102", BLERead | BLENotify);
BLEUnsignedIntCharacteristic ZChar("2103", BLERead | BLENotify);

 
void setup() {
  IMU.begin();  
  Serial.begin(115200); 
  //while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);


  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1);
  }

  BLE.setLocalName("Accelerometer BLE");
  BLE.setAdvertisedService(SensorService);
  SensorService.addCharacteristic(XChar);
  SensorService.addCharacteristic(YChar);
  SensorService.addCharacteristic(ZChar);
  
  BLE.addService(SensorService);

  XChar.writeValue(xBLE);
  YChar.writeValue(yBLE);
  ZChar.writeValue(zBLE);

  BLE.advertise();

  Serial.println("Accelerometer BLE peripheral device is now active, waiting for connections...");
}

void loop() {

  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    digitalWrite(LED_BUILTIN, 1);
    
       
    while (central.connected()) {
    //while(1){

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(xSensor, ySensor, zSensor);
      }

      xx100 = xSensor ;
      yx100 = ySensor ;
      zx100 = zSensor ;
      


      powx = pow(xx100,2);
      powy = pow(yx100,2);
      powz = pow(zx100,2);

      sumxyz = (powx + powy + powz); 

      sumsqrt = sqrt(sumxyz);



      Serial.println(sumsqrt);

 
      XChar.writeValue(xBLE);
      YChar.writeValue(yBLE);
      ZChar.writeValue(zBLE);
      
      delay(50);
    }    
  }       
   
  else {
    central.disconnect();
    digitalWrite(LED_BUILTIN, 0);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    delay(1000);
  }
  
  digitalWrite(LED_BUILTIN, 0);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
  BLE.advertise();

}
