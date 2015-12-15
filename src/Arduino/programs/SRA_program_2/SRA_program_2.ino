/**
 * This program gets the GPS coordinates from a button press
 */

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
 const int buttonPin = 3;
 TinyGPS gps;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec, String infront);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
//static void disCoor();
//static void sendCoor();
//Space for globals
float flat, flon;
unsigned long age, date, time, chars = 0;
SimpleTimer powerTimer;//this is the timer used to count for power functions
//These are constants that will be used with the timer to say when it should set things off
int comTime = 1000;
int gpsTime = 1000;
SoftwareSerial comSerial(9, 10); // RX, TX
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(57600);
  lcd.begin(16, 2);
}

void loop() 
{
  //we pressed the button
  if(buttonPin == HIGH)
  {
    disCoor();//Get the GPS coordinate and print it out to the screen
    sendCoor();//send the message out
  }
  else
  {
    
  }
  
  
  
}
/*
 * Prints out the float value to a connected
 * computer or the LCD screen. Performs some checks including
 * if the input was valid or not
 */
static void print_float(float val, float invalid, int len, int prec, String infront)
{
  lcd.print(infront + " ");
  if (val == invalid)
  {
    while (len-- > 1)
    {
      Serial.print('*');
    }
    lcd.print("No signal");
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    lcd.print(val);
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
/**
 * This will display coordinate information onto the screen
 */
void disCoor()
{
  gps.f_get_position(&flat, &flon, &age);//get gps coordinates
  Serial.println();
  lcd.setCursor(0, 0);//start at the top
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6, "Lat:");//print to USB and screen
  lcd.setCursor(0,1);//start at the bottom
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6, "Lon:");//print again
  smartdelay(1000);
}
/**
 * Sends the information 
 */
void sendCoor()
{
//  if()
//  {
//    
//  }
}
/**
 * If we received a signal then lets do something with it
 */
void recSig()
{
  String sigRaw;
  
  //are we getting a signal?
  if(comSerial.available())
  {
    sigRaw = comSerial.read();
    //now process the information
    //lets check and see who sent this
    if(sigRaw.substring(0, 2) == "FD")
    {
      //field device sent this
    }
    else if(sigRaw.substring(0, 2) == "HB")
    {
      //The home base sent this out, so might be important!!
    }
    /*
     * Signal code:
     *    HB - 001 - 344 - 'found it' - $
     * What type - ID of who sent - ID of who its for - message - End code
     */
    /**
     * Steps: 
     * 1. get who sent it by taking the first two letters and see what we got
     * 2. After we then need to check the last variable and see if its an end code
     * 2A. if there is no end code than we didnt get the full message
     * 2Ai. If home base send back message saying we didnt get it
     * 3. If there is an end code we got the full message! so read it
     * 4. The 6th letter is the start of who its for so read
     * 4A. if not for us then ignore
     *  
     */
  }
}
/**
 * This contains all the code for monitoring power consumption
 * it is called every time we do something by the function that did it
 */
void powerMonitor(int who)
{
  //These are used to keep track of how many times we have used something
  int numSend;
  int numRec;
  int numGPS;
}

