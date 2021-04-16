#include <HX711.h>
#include <DHT.h>
#include <DHT_U.h>


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// You should get Auth Token in the Blynk App.
char auth[] = "*******";

// Your WiFi credentials.
char ssid[] = "*****";
char pass[] = "*****";

float beer_coef = 1.04; //1литр пива это 1.04кг

int keg1_calibration_factor = -21020;   // Калибровочная значение, индивидуальное для каждой итоговой площадки
int keg1_offset_factor = -64200;        // 0 вес для площадки (сюда вставляем значение, который прилетает в keg1_raw при своем пустом весе)
float keg1_keg_scale = 0.00;            // Вес пустого кега, размещенного на площадке
bool keg1_isNeedSetKegScale = false;
// Датчик веса
const int keg1_LOADCELL_DOUT_PIN = 13; 
const int keg1_LOADCELL_SCK_PIN = 12;
HX711 keg1_scale;

// датчик DHT
uint8_t DHTPin = D5; 
DHT dht(DHTPin, DHT22);
float temperature;
float humidity;

BlynkTimer timer;


BLYNK_WRITE(V42) { //получаем вес кега 1 с сервера
  float f_scale = param.asFloat();
  if (f_scale > 0){
    keg1_keg_scale = f_scale;
  }else{
    keg1_keg_scale = 0;
  }
}

BLYNK_WRITE(V43){ //получаем состояние кнопки 1 с сервера
  int intButtonPressed = param.asInt();
  if (intButtonPressed == 0) { //нажатие кнопки "сбросить вес" кег1
    Serial.println("Button is pressed.");
    Blynk.setProperty(V43, "label", "Ожидайте");
    keg1_isNeedSetKegScale = true;
  }
}


void myTimerEvent(){

  keg1_scale.power_up();
  float keg1_reading;                       // Значение прочитанное
  float keg1_raw;                           // Сырое значение
  keg1_scale.wait_ready();                  
  keg1_scale.set_scale(keg1_calibration_factor);
  keg1_scale.set_offset(keg1_offset_factor);    

  Serial.print("keg1_Reading: ");
  keg1_scale.wait_ready();
  keg1_reading = keg1_scale.get_units(10); //Read scale in g/Kg
  keg1_raw = keg1_scale.read_average(5);   //Read raw value from scale too
  Serial.print(keg1_reading, 2); Serial.println(" KG");
  Serial.print("keg1_Raw: "); Serial.println(keg1_raw);
  Serial.print("keg1_Calibration factor: "); Serial.println(keg1_calibration_factor);

  float keg1_result;
  if (keg1_isNeedSetKegScale == true){ // если нужно записать вес пустого кега
    keg1_isNeedSetKegScale = false;
    keg1_keg_scale = keg1_reading;
    Blynk.setProperty(V43, "label", "Калибровка кег #1");
  }
  
  keg1_result = (keg1_reading - keg1_keg_scale)/beer_coef;  //(вес итоговый - вес_пустого_кега)/коэф_веса_пива
  Serial.print("keg1_kegScale: ");Serial.print(keg1_keg_scale, 2); Serial.println(" KG");
  Serial.print("keg1_result: ");Serial.print(keg1_result, 2); Serial.println(" L");
  
  if (keg1_result < 0){
     keg1_result = 0.00; //чтобы небыло минусового значения
  }

  Blynk.virtualWrite(V40, keg1_result);
  Blynk.virtualWrite(V41, keg1_raw);
  Blynk.virtualWrite(V42, keg1_keg_scale);
  keg1_scale.power_down();

  

  temperature = dht.readTemperature(); 
  if(!isnan(temperature)){
    Blynk.virtualWrite(V3, temperature);
//    Serial.print("Temperature: ");  Serial.println(temperature);
  }
  
  humidity = dht.readHumidity();
  if(!isnan(humidity)){
    Blynk.virtualWrite(V4, humidity);
//    Serial.print("Humidity: ");  Serial.println(humidity);
  }


}

void setup(){
  Serial.begin(9600);


  keg1_scale.begin(keg1_LOADCELL_DOUT_PIN, keg1_LOADCELL_SCK_PIN); // Start scale on specified pins
  keg1_scale.wait_ready();                               //Ensure scale is ready, this is a blocking function
  keg1_scale.set_scale();
  Serial.println("Scale Set keg1_");
  keg1_scale.wait_ready();
  keg1_scale.tare(); // Tare scale on startup
  keg1_scale.wait_ready();
  Serial.println("Scale Zeroed keg1_");

  
  pinMode(DHTPin, INPUT);
  dht.begin();


  Blynk.begin(auth, ssid, pass); //родной blynk

  
}

BLYNK_CONNECTED() { //как только подключились - запрашиваем значения переменных
    Blynk.syncVirtual(V42);
    Blynk.syncVirtual(V43);
    timer.setInterval(1000L, myTimerEvent);
}

void loop(){
  Blynk.run();
  timer.run();

}
