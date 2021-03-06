////////////////////////////////////////
//                                    // 
//    WLM's system　version_1         //
//    Responsible person　maitake     //
//                                    //
////////////////////////////////////////



/////////ライブラリ////////////
#include <Adafruit_Si7021.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
///////////////////////////////////


/////各変数の宣言（温湿度、水温、照度の順）///////////////////////////////
Adafruit_Si7021 sensor = Adafruit_Si7021();

#define ONE_WIRE_BUS 10 // データ(黄)で使用するポート番号
#define SENSER_BIT 12      // 精度の設定bit(9から12までできる)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int BH1750_address = 0x23; // i2c Addresse(OPEN or connecting to GND) 
//int BH1750_address = 0x5c; // i2c Addresse(connecting to Vcc)
byte buff[2];
///////////////////////////////////////////////////////////////////////



void setup() {
  //共通設定////////
  Serial.begin(9600);
  ////
  
  //水温センサの
  sensors.setResolution(SENSER_BIT);
  ////

  //照度センサの/////
  Wire.begin();
  BH1750_Init(BH1750_address);
  pinMode(A4,INPUT);
  pinMode(A5, OUTPUT);
  /////
}

void loop() {
  temp_and_hom();
  water_temp();
  ilmi();
  delay(1000);

}

//気温センサと湿度センサ
void temp_and_hom(){
  Serial.print("Humidity: "); 
  Serial.println(sensor.readHumidity(), 2);//湿度の取得&シリアル通信
  Serial.println("");
  Serial.print("Temperature: "); //気温の取得&シリアル通信
  Serial.println(sensor.readTemperature(), 2);
  Serial.println("");
}

//水温センサ
void water_temp(){
  sensors.requestTemperatures();// 水温取得要求
  Serial.print("water_temp:");
  Serial.println(sensors.getTempCByIndex(0)); //水温の取得&シリアル送信
  Serial.println("");
}

//以下照度センサのコード(現在コードを簡略化を模索中)
void ilmi(){
  float valf=0;
  
  if(BH1750_Read(BH1750_address)==2){
     
    valf=((buff[0]<<8)|buff[1])/1.2;
     
    if(valf<0)Serial.print("> 65535");
    else Serial.print((int)valf,DEC); 
     
    Serial.println(" lx"); 
  }
}


void BH1750_Init(int address){
   
  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] aufloesung
  Wire.endTransmission();
}
 
byte BH1750_Read(int address){
   
  byte i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()){
    buff[i] = Wire.read(); 
    i++;
  }
  Wire.endTransmission();  
  return i;
}
