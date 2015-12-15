/**
 * This program gets the GPS coordinates from a button press
 */

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>
 const int buttonPin = 3;
 TinyGPS gps;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
 // pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial1.begin(57600);
  lcd.begin(16, 2);
//  lcd.print("Lat: 38.8");
//  lcd.setCursor(0, 1);
//  lcd.print("Lon: -77.6");
}

void loop() 
{
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  String latTP;
  String lonTP;
//  if(buttonPin == HIGH)
//  {
    gps.f_get_position(&flat, &flon, &age);//get gps coordinates
    Serial.println();
    lcd.setCursor(0, 0);//start at the top
    print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);//print to USB and screen
    lcd.setCursor(0,1);//start at the bottom
    print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);//print again
    smartdelay(1000);
//  }
  
  
}
/*
 * Prints out the float value. Performs some checks including
 * if the input was valid or not
 */
static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
    {
      Serial.print('*');
      lcd.print("No signal");
    }
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}
/**
 * Prints out gps information in int form
 */
static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

/**
 * Proints out the GPS information in string form
 */
static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
/**
 * A better delay system more talored 
 * for the GPS.
 */
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
