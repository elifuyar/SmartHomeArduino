#include <Servo.h>
#include <SoftwareSerial.h>
#include <dht11.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 8, 7, 6, 5);
#define DHT11PIN 2 // DHT11PIN olarak Dijital 2'yi belirliyor
dht11 DHT11;
int led = 9; //isik icin 9. pini sectik
int pirPin = 3; // hareket sensoru icin
int deger = 0; 
float sicaklik=0;
float nem=0;
Servo myservo; // servo tanimlama
int pos = 0; //servo pos
SoftwareSerial Genotronex(10, 11); // RX, TX
int BluetoothData; // bluetooth değişkeni oluşturduk

void setup() 
{
  lcd.begin(16,2);
  lcd.print("S.A");
  myservo.attach(4); // Servonun sinyal alacagi pin numarasini belirledik
  pinMode(led, OUTPUT); //led pinini cikis olarak ayarladik
  pinMode(pirPin, INPUT); //hareket pinini giris yaptik
  Serial.begin(9600);//Seri iletisimi baslattik
  Genotronex.begin(9600);
}


void loop() 
{
  int chk = DHT11.read(DHT11PIN);
  deger = digitalRead(pirPin);  // Dijital pin okunuyor
  
 //BLUETOOTH KONTROL KISMI
  
  if (Genotronex.available()) //Veri var mı yok mu kontrolü yaptık
  {
    BluetoothData=Genotronex.read();//bluetooth degerini okuyoruz
    
    if (BluetoothData=='0') //led kismina tikladiysa
    {
      digitalWrite(led, HIGH);   // ledi yaktık
      delay(1000);               // 1 saniye bekleme
      Genotronex.print("Yandi! ");  
    }
    else if(BluetoothData=='h')
    {
      digitalWrite(led, LOW);    // ledi kapattık
      delay(1000);
    }
    if (BluetoothData=='5')//hareket kismina tiklandiysa
    {
      deger = digitalRead(pirPin);
      if (deger == HIGH) 
      {         
        for(int i = 0; i<5; i++)//hareket varsa led 5 kere yanip sonsun
        {
          digitalWrite(led, HIGH);   // ledi yaktık
          delay(500);               // 1 saniye bekleme
          digitalWrite(led, LOW);    // ledi kapattık
          delay(500);
        }
        digitalWrite(led, HIGH);//ledi yaktık
      }
    }


    if(BluetoothData=='1') //sicaklik kismina tikladiysa
    {
      lcd.clear();
      Genotronex.print("Nem (%): ");
      Genotronex.print((float)DHT11.humidity, 2);
      nem = (float)DHT11.humidity;
      Genotronex.println();

      Genotronex.print("Sicaklik (Celcius): ");
      Genotronex.print((float)DHT11.temperature, 2); 
      sicaklik = (float)DHT11.temperature;
      Genotronex.println();
 
      delay(1000);
      lcd.print("Sicaklik: ");
      lcd.print(sicaklik);
      lcd.setCursor(0,1);
      lcd.print("Nem: ");
      lcd.print(nem);
    }


    if (BluetoothData=='4')//Kapi ac a tiklandiysa
    {
      for(pos = 90; pos>=0; pos-=1)// Servo 1'den 0 pozisyonuna 90 derece dönecek.
      {
        myservo.write(pos); // Belirlenen pozisyona gitmesi isteniyor. 
        delay(5); // Pozisyona 5 ms de ulasiyor
      } 
    }

    if (BluetoothData=='3') //kapi kapaya tiklandiysa
    {
      for(pos = 0; pos < 90; pos += 1) 
      {
        myservo.write(pos); // Belirlenen pozisyona gitmesi isteniyor. 
        delay(5); // Pozisyona 5 ms de ulasiyor. 
      } 
    }
  }
  delay(100);

}

