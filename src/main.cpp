// V3.0 created by Nikita71 30.01.2022 Arduino Ide 1.8.16 !(v1.0.4(ver Esp32Library))!



// вибро сигнализация может быть включена только со звуковой сигнализацией !
/*
   посмотри на ползунки в приложении фаста, ты не сможешь сделать чтобы
   у тебя была включена только вибро сигнализация, только вместе со звуковой,
   а звуковая может работать сама по себе.
*/
/////////////////////////////////////////////////////////////
bool GetDataThreshold_1;
float DoseThreshold_1 = 10000;
float DoseRateThreshold_1 = 30;
bool DoseRateSoudTh_1;
bool DoseRateVibroTh_1;
bool DoseSoudTh_1;
bool DoseVibroTh_1;
/////////////////////////////////////////////////////////////
bool GetDataThreshold_2;
float DoseThreshold_2 = 1000000;
float DoseRateThreshold_2 = 100;
bool DoseRateSoudTh_2;
bool DoseRateVibroTh_2;
bool DoseSoudTh_2;
bool DoseVibroTh_2;
/////////////////////////////////////////////////////////////
bool GetDataThreshold_3;
float DoseThreshold_3 = 100000000;
float DoseRateThreshold_3 = 100000;
bool DoseRateSoudTh_3;
bool DoseRateVibroTh_3;
bool DoseSoudTh_3;
bool DoseVibroTh_3;
////////////////////////////////////////////////////////////
float dynamic_dose;
float static_dose;
int pulses_total;
int pulses_ps;
float dose_total;



//////////////////////////////////////////////////////////////////////////////////////////////////
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <CG_RadSens.h>
#include <Wire.h>
bool deviceConnected;

CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS);/*Constructor of the class ClimateGuard_RadSens1v2,
                                                           sets the address parameter of I2C sensor.
                                                           Default address: 0x66.*/


#define SERVICE_UUID "63462a4a-c28c-4ffd-87a4-2d23a1c72581"
BLECharacteristic DoseCharacteristics("70bc767e-7a1a-4304-81ed-14b9af54f7bd", BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic ThresholdCharacteristics_1("3f71e820-1d98-46d4-8ed6-324c8428868c", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE );
BLECharacteristic ThresholdCharacteristics_2("2e95d467-4db7-4d7f-9d82-4cd5c102fa05", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE );
BLECharacteristic ThresholdCharacteristics_3("f8de242f-8d84-4c12-9a2f-9c64a31ca7ca", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE );
BLECharacteristic SettingsCharacteristics("ea50cfcd-ac4a-4a48-bf0e-879e548ae157", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE );


float getFloat(uint8_t* pData, uint8_t startIdx) {
  union {
    float val;
    uint8_t b[4];
  } float_bytes_u;
  for (uint8_t i = 0; i < 4; i++) {
    float_bytes_u.b[i] = pData[i + startIdx];
  }
  return float_bytes_u.val;
}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      BLEDevice::startAdvertising();
    }
};
class ThresholdCallbacks_1: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *ThresholdCharacteristics_1) {
      std::string rxValue = ThresholdCharacteristics_1->getValue();
      uint8_t pData [10];
      for (int i = 0; i < rxValue.length(); i++) {
        pData[i] = rxValue[i];
      }
      DoseThreshold_1 = getFloat(pData, 0) * 100000; //доза в мкр
      DoseRateThreshold_1 = getFloat(pData, 4) * 100; // мощось дози в мкр/ч
      switch (pData[9]) {  //костили флагов вибро/звуковой сигнализациї
        case 0:
          DoseRateSoudTh_1 = 0;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 0;
          DoseVibroTh_1 = 0;
          break;
        case 34:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 1;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 1;
          break;
        case 16:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 0;
          DoseVibroTh_1 = 0;
          break;
        case 32:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 1;
          DoseSoudTh_1 = 0;
          DoseVibroTh_1 = 0;
          break;
        case 1:
          DoseRateSoudTh_1 = 0;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 0;
          break;
        case 2:
          DoseRateSoudTh_1 = 0;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 1;
          break;
        case 33:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 1;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 0;
          break;
        case 18:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 1;
          break;
        case 17:
          DoseRateSoudTh_1 = 1;
          DoseRateVibroTh_1 = 0;
          DoseSoudTh_1 = 1;
          DoseVibroTh_1 = 0;
          break;
      }
      GetDataThreshold_1 = 1;
    }
};
class ThresholdCallbacks_2: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *ThresholdCharacteristics_2) {
      std::string rxValue = ThresholdCharacteristics_2->getValue();
      uint8_t pData [10];
      for (int i = 0; i < rxValue.length(); i++) {
        pData[i] = rxValue[i];
      }
      DoseThreshold_2 = getFloat(pData, 0) * 100000; //доза в мкр
      DoseRateThreshold_2 = getFloat(pData, 4) * 100; // мощось дози в мкр/ч
      switch (pData[9]) {  //костили флагов вибро/звуковой сигнализациї
        case 0:
          DoseRateSoudTh_2 = 0;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 0;
          DoseVibroTh_2 = 0;
          break;
        case 34:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 1;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 1;
          break;
        case 16:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 0;
          DoseVibroTh_2 = 0;
          break;
        case 32:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 1;
          DoseSoudTh_2 = 0;
          DoseVibroTh_2 = 0;
          break;
        case 1:
          DoseRateSoudTh_2 = 0;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 0;
          break;
        case 2:
          DoseRateSoudTh_2 = 0;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 1;
          break;
        case 33:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 1;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 0;
          break;
        case 18:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 1;
          break;
        case 17:
          DoseRateSoudTh_2 = 1;
          DoseRateVibroTh_2 = 0;
          DoseSoudTh_2 = 1;
          DoseVibroTh_2 = 0;
          break;
      }
      GetDataThreshold_2 = 1;
    }
};
class ThresholdCallbacks_3: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *ThresholdCharacteristics_3) {
      std::string rxValue = ThresholdCharacteristics_3->getValue();
      uint8_t pData [10];
      for (int i = 0; i < rxValue.length(); i++) {
        pData[i] = rxValue[i];
      }
      DoseThreshold_3 = getFloat(pData, 0) * 100000; //доза в мкр
      DoseRateThreshold_3 = getFloat(pData, 4) * 100; // мощось дози в мкр/ч
      switch (pData[9]) {  //костили флагов вибро/звуковой сигнализациї
        case 0:
          DoseRateSoudTh_3 = 0;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 0;
          DoseVibroTh_3 = 0;
          break;
        case 34:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 1;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 1;
          break;
        case 16:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 0;
          DoseVibroTh_3 = 0;
          break;
        case 32:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 1;
          DoseSoudTh_3 = 0;
          DoseVibroTh_3 = 0;
          break;
        case 1:
          DoseRateSoudTh_3 = 0;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 0;
          break;
        case 2:
          DoseRateSoudTh_3 = 0;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 1;
          break;
        case 33:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 1;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 0;
          break;
        case 18:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 1;
          break;
        case 17:
          DoseRateSoudTh_3 = 1;
          DoseRateVibroTh_3 = 0;
          DoseSoudTh_3 = 1;
          DoseVibroTh_3 = 0;
          break;
      }
      GetDataThreshold_3 = 1;
    }
  };
class SettingsCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *SettingsCharacteristics) {
      std::string rxValue = SettingsCharacteristics->getValue();
      uint8_t pData [8];
      for (int i = 0; i < rxValue.length(); i++) {
        pData[i] = rxValue[i];
      }
      
      if (pData[0] == 222 && pData[1] == 0) {
        Serial.println("RESET DOSE");
      }
      
      if (pData[0] == 224 && pData[1] == 2) {
        Serial.println("slow");
      }      
      if (pData[0] == 224 && pData[1] == 1) {
        Serial.println("medium");
      }   
      if (pData[0] == 224 && pData[1] == 0) {
        Serial.println("fast");
      }   

      if (pData[0] == 226 && pData[1] == 1) {
        Serial.println("accelerometr ON");
      }  
      if (pData[0] == 226 && pData[1] == 0) {
        Serial.println("accelerometr OFF");
      }  

      if (pData[0] == 255 && pData[1] == 172) {
        Serial.println("power OFF");
      }  

      if (pData[0] == 170 && pData[1] == 0) {
        Serial.println("find device");
      }  

      if (pData[0] == 20 && pData[1] == 18) {
        Serial.println("reconnection sounds on");
      }  
      if (pData[0] == 19 && pData[1] == 18) {
        Serial.println("reconnection sounds off");
      } 
      
      if (pData[0] == 8 && pData[1] == 0) {
        Serial.println("No sound");
      } 
      if (pData[0] == 1 && pData[1] == 1) {
        Serial.println("Clicks");
      } 
      if (pData[0] == 1 && pData[1] == 20) {
        Serial.println("Beeps");
      } 

      if (pData[0] == 225 && pData[1] == 1) {
        Serial.println("1/10");
      } 
      if (pData[0] == 225 && pData[1] == 0) {
        Serial.println("1/1");
      } 
  
    }
};



void setup() {

  Serial.begin(115200);

  Wire.begin(); // This function initializes the Wire library
  Serial.println("-----------------START RADSENS--------------------");
  while(!radSens.init()) /*Initializates function and sensor connection. Returns false if the sensor is not connected to the I2C bus.*/
  {
    Serial.println("Sensor wiring error!");
    delay(1000);
  }

  uint8_t sensorChipId = radSens.getChipId(); /*Returns chip id, default value: 0x7D.*/

  Serial.print("Chip id: 0x");
  Serial.println(sensorChipId, HEX);

  uint8_t firmWareVer = radSens.getFirmwareVersion(); /*Returns firmware version.*/

  Serial.print("Firmware version: ");
  Serial.println(firmWareVer);

  Serial.println("-------------------------------------");
  Serial.println("Set Sensitivity example:\n");

  uint16_t sensitivity = radSens.getSensitivity(); /*Rerutns the value coefficient used for calculating
                                                    the radiation intensity or 0 if sensor isn't connected.*/

  Serial.print("\t getSensitivity(): ");
  Serial.println(sensitivity);
  //Serial.println("\t setSensitivity(55)... ");

  /*radSens.setSensitivity(55); /*Sets the value coefficient used for calculating
                                the radiation intensity*/
  //sensitivity = radSens.getSensitivity();
  //Serial.print("\t getSensitivity(): ");
  //Serial.println(sensitivity);
  Serial.println("\t setSensitivity(2400)... ");

  radSens.setSensitivity(2400);

  Serial.print("\t getSensitivity(): ");
  Serial.println(radSens.getSensitivity());
  Serial.println("-------------------------------------");
  Serial.println("HW generator example:\n");

  bool hvGeneratorState = radSens.getHVGeneratorState(); /*Returns state of high-voltage voltage Converter.
                                                           If return true -> on
                                                           If return false -> off or sensor isn't conneted*/

  Serial.print("\n\t HV generator state: ");
  Serial.println(hvGeneratorState);
  //Serial.println("\t setHVGeneratorState(false)... ");

  /*radSens.setHVGeneratorState(false); /*Set state of high-voltage voltage Converter.
                                        if setHVGeneratorState(true) -> turn on HV generator
                                        if setHVGeneratorState(false) -> turn off HV generator*/

  //hvGeneratorState = radSens.getHVGeneratorState();
  //Serial.print("\t HV generator state: ");
  //Serial.println(hvGeneratorState);
  Serial.println("\t setHVGeneratorState(true)... ");

  radSens.setHVGeneratorState(true);

  hvGeneratorState = radSens.getHVGeneratorState();
  Serial.print("\t HV generator state: ");
  Serial.println(hvGeneratorState);
  Serial.println("-------------------------------------");
  Serial.println("LED indication control example:\n");

  bool ledState = radSens.getLedState(); /*Returns state of LED indicator.
                                                           If return true -> on
                                                           If return false -> off*/

  Serial.print("\n\t LED indication state: ");
  Serial.println(ledState);
  //Serial.println("\t turn off LED indication... ");

  /*radSens.setLedState(false); /*Set state of LED indicator.
                                        if setHVGeneratorState(true) -> turn on LED indicator
                                        if setHVGeneratorState(false) -> turn off LED indicator*/
  //ledState = radSens.getLedState();
  //Serial.print("\t LED indication state: ");
  //Serial.println(ledState);
  Serial.println("\t turn on led indication... ");

  radSens.setLedState(true);

  ledState = radSens.getLedState();
  Serial.print("\t LED indication state: ");
  Serial.print(ledState);
  Serial.println("\n------------------END RADSENS-------------------");

  BLEDevice::init("AtomFast");

  BLEServer *pServer = BLEDevice::createServer();
  
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *BleService = pServer->createService(SERVICE_UUID);

  BleService->addCharacteristic(&DoseCharacteristics);
  DoseCharacteristics.addDescriptor(new BLE2902());


  BleService->addCharacteristic(&ThresholdCharacteristics_1);
  ThresholdCharacteristics_1.setCallbacks(new ThresholdCallbacks_1());

  BleService->addCharacteristic(&ThresholdCharacteristics_2);
  ThresholdCharacteristics_2.setCallbacks(new ThresholdCallbacks_2());

  BleService->addCharacteristic(&ThresholdCharacteristics_3);
  ThresholdCharacteristics_3.setCallbacks(new ThresholdCallbacks_3());

  BleService->addCharacteristic(&SettingsCharacteristics);
  SettingsCharacteristics.setCallbacks(new SettingsCallbacks());

  BleService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
 
}

void BleSentDoseData (float RAD, float DOSERAD, float PULSE2, uint8_t battery, int8_t temperature) {
  uint8_t date[13];
  if (battery > 100) {                      // индикатор зарядки акб
    *(uint8_t*)(&date[0]) = 253;
  } else {
    *(uint8_t*)(&date[0]) = 0;
  }
  *(float*)(&date[1]) = DOSERAD / 100000.0; // доза в млР/ч
  *(float*)(&date[5]) = RAD / 100.0;        // мощность дозы в мкР/ч
  uint16_t PULSE = PULSE2 * 4;              // костили
  *(uint16_t*)(&date[9]) = PULSE / 2;       // CPS
  *(uint8_t*)(&date[11]) = battery;         // процент заряда батареи (0..100)
  *(int8_t*)(&date[12]) = temperature;      // температура

  DoseCharacteristics.setValue(date, 13);
  DoseCharacteristics.notify();
}

void SentDataThreshold_1() {
  uint8_t date[10];
  *(float*)(&date[0]) = DoseThreshold_1 / 100000;   //  доза в мкр
  *(float*)(&date[4]) = DoseRateThreshold_1 / 100;  // мощность дози в мкр/ч
  if (DoseRateSoudTh_1 == 0 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 0 && DoseVibroTh_1 == 0) { //костили флагов вибро/звуковой сигнализациї
    date[9] = 0;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 1 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 1) {
    date[9] = 34;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 0 && DoseVibroTh_1 == 0) {
    date[9] = 16;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 1 && DoseSoudTh_1 == 0 && DoseVibroTh_1 == 0) {
    date[9] = 32;
  }
  if (DoseRateSoudTh_1 == 0 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 0) {
    date[9] = 1;
  }
  if (DoseRateSoudTh_1 == 0 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 1) {
    date[9] = 2;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 1 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 0) {
    date[9] = 33;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 1) {
    date[9] = 18;
  }
  if (DoseRateSoudTh_1 == 1 && DoseRateVibroTh_1 == 0 && DoseSoudTh_1 == 1 && DoseVibroTh_1 == 0) {
    date[9] = 17;
  }

  ThresholdCharacteristics_1.setValue(date, 10);
  ThresholdCharacteristics_1.notify();
}
void SentDataThreshold_2() {
  uint8_t date[10];
  *(float*)(&date[0]) = DoseThreshold_2 / 100000;   //  доза в мкр
  *(float*)(&date[4]) = DoseRateThreshold_2 / 100;  // мощность дози в мкр/ч
  if (DoseRateSoudTh_2 == 0 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 0 && DoseVibroTh_2 == 0) { //костили флагов вибро/звуковой сигнализациї
    date[9] = 0;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 1 && DoseSoudTh_1 == 2 && DoseVibroTh_2 == 1) {
    date[9] = 34;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 0 && DoseVibroTh_2 == 0) {
    date[9] = 16;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 1 && DoseSoudTh_2 == 0 && DoseVibroTh_2 == 0) {
    date[9] = 32;
  }
  if (DoseRateSoudTh_2 == 0 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 1 && DoseVibroTh_2 == 0) {
    date[9] = 1;
  }
  if (DoseRateSoudTh_2 == 0 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 1 && DoseVibroTh_2 == 1) {
    date[9] = 2;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 1 && DoseSoudTh_2 == 1 && DoseVibroTh_2 == 0) {
    date[9] = 33;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 1 && DoseVibroTh_2 == 1) {
    date[9] = 18;
  }
  if (DoseRateSoudTh_2 == 1 && DoseRateVibroTh_2 == 0 && DoseSoudTh_2 == 1 && DoseVibroTh_2 == 0) {
    date[9] = 17;
  }

  ThresholdCharacteristics_2.setValue(date, 10);
  ThresholdCharacteristics_2.notify();
}
void SentDataThreshold_3() {
  uint8_t date[10];
  *(float*)(&date[0]) = DoseThreshold_3 / 100000;   //  доза в мкр
  *(float*)(&date[4]) = DoseRateThreshold_3 / 100;  // мощность дози в мкр/ч
  if (DoseRateSoudTh_3 == 0 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 0 && DoseVibroTh_3 == 0) { //костили флагов вибро/звуковой сигнализациї
    date[9] = 0;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 1 && DoseSoudTh_3 == 2 && DoseVibroTh_3 == 1) {
    date[9] = 34;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 0 && DoseVibroTh_3 == 0) {
    date[9] = 16;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 1 && DoseSoudTh_3 == 0 && DoseVibroTh_3 == 0) {
    date[9] = 32;
  }
  if (DoseRateSoudTh_3 == 0 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 1 && DoseVibroTh_3 == 0) {
    date[9] = 1;
  }
  if (DoseRateSoudTh_3 == 0 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 1 && DoseVibroTh_3 == 1) {
    date[9] = 2;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 1 && DoseSoudTh_3 == 1 && DoseVibroTh_3 == 0) {
    date[9] = 33;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 1 && DoseVibroTh_3 == 1) {
    date[9] = 18;
  }
  if (DoseRateSoudTh_3 == 1 && DoseRateVibroTh_3 == 0 && DoseSoudTh_3 == 1 && DoseVibroTh_3 == 0) {
    date[9] = 17;
  }

  ThresholdCharacteristics_3.setValue(date, 10);
  ThresholdCharacteristics_3.notify();
  }

void loop () {
  delay(1000);
  pulses_ps = radSens.getNumberOfPulses() - pulses_total;
  pulses_total = radSens.getNumberOfPulses();
  if (deviceConnected) {
    dynamic_dose = radSens.getRadIntensyDynamic();
    static_dose = radSens.getRadIntensyStatic();
    BleSentDoseData(dynamic_dose, static_dose, pulses_ps, 100, 22); // Мощность дозы в мкР/ч, доза в мкР/ч, cps, процент заряда батареи, температура.
    SentDataThreshold_1();
    SentDataThreshold_2();
    SentDataThreshold_3();
  }

  Serial.print("Rad intensy dyanmic: ");

  Serial.println(radSens.getRadIntensyDynamic()); /*Returns dynamic radiation intensity (recommended if measurement period T < 123 sec).*/

  Serial.print("Rad intensy static: ");

  Serial.println(radSens.getRadIntensyStatic()); /*Returns static radiation intensity (recommended if measurement period T = 500 sec).*/

  Serial.print("Number of pulses: ");

  Serial.println(radSens.getNumberOfPulses()); /*Returns the accumulated number of pulses registered by the 
                                                 module since the last I2C data reading.*/
  Serial.print("CPS: ");

  Serial.println(pulses_ps);

  if (GetDataThreshold_1 == 1) {
    GetDataThreshold_1 = 0;
    Serial.println();
    Serial.println();
    Serial.print("DoseRateThreshold_1 = ");
    Serial.print(DoseRateThreshold_1);
    Serial.print("    ");
    Serial.print(DoseRateSoudTh_1);
    Serial.print("    ");
    Serial.print(DoseRateVibroTh_1);
    Serial.println();
    Serial.print("DoseThreshold_1 = ");
    Serial.print(DoseThreshold_1);
    Serial.print("    ");
    Serial.print(DoseSoudTh_1);
    Serial.print("    ");
    Serial.print(DoseVibroTh_1);
  }
  if (GetDataThreshold_2 == 1) {
    GetDataThreshold_2 = 0;
    Serial.println();
    Serial.println();
    Serial.print("DoseRateThreshold_2 = ");
    Serial.print(DoseRateThreshold_2);
    Serial.print("    ");
    Serial.print(DoseRateSoudTh_2);
    Serial.print("    ");
    Serial.print(DoseRateVibroTh_2);
    Serial.println();
    Serial.print("DoseThreshold_2 = ");
    Serial.print(DoseThreshold_2);
    Serial.print("    ");
    Serial.print(DoseSoudTh_2);
    Serial.print("    ");
    Serial.print(DoseVibroTh_2);
  }
  if (GetDataThreshold_3 == 1) {
      GetDataThreshold_3 = 0;
      Serial.println();
      Serial.println();
      Serial.print("DoseRateThreshold_3 = ");
      Serial.print(DoseRateThreshold_3);
      Serial.print("    ");
      Serial.print(DoseRateSoudTh_3);
      Serial.print("    ");
      Serial.print(DoseRateVibroTh_3);
      Serial.println();
      Serial.print("DoseThreshold_3 = ");
      Serial.print(DoseThreshold_3);
      Serial.print("    ");
      Serial.print(DoseSoudTh_3);
      Serial.print("    ");
      Serial.print(DoseVibroTh_3);
    }
  

}