/**
 * This program gets the GPS coordinates from a button press
 */
/*
 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 Currently free pins: 7, 10
 */
 #include <SoftwareSerial.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec, String infront);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
static float return_float(float val, float invalid, int len, int prec);

TinyGPS gps;
//{"$PMTK161,0*28\x0D\x0A"}; GPS command to set to standby mode. Any byte sent after will wake it up
//

// software serial for xbee: RX = digital pin 8, TX = digital pin 9
//Space for globals
SoftwareSerial portGPS(8, 9);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//int GPSPowerPin = --;
// RadPowerPin = 10;
float flat, flon;
unsigned long age, date, time, chars = 0;

//These are constants that will be used with the timer to say when it should set things off
unsigned long lastCheckTm = 0;//used to keep track of the last tine we did a full check in
unsigned long currCheckTm = 0;
unsigned long lastFindTm = 0;//used to keep track of the last tine we just got GPS to give to user
unsigned long currFindTm = 0;
unsigned long lastInterval = 0;
int checkInTotal = 0; //used to keep track of the total number of times we check in
int findTotal = 0;//used to keep track of the total times we just got a signal
boolean isGPSOn = true;//is the GPS on
boolean isLCDOn = true;//is the LCD on
boolean isRadOn = true;//is the radio on
//boolean flags that keep track of which functions are running 
boolean enProt1F = false;
boolean enProt2F = false; 
int buttonPin = 6;// pin for the button
//These are the globals for protocal 1
int p1Counter = 0;
String ID = "000001";
int buttonState = 0;
int LEDPin = 13;
int xbsp = 7;

void setup() 
{
  Serial.begin(9600);//Communication for the 
  Serial1.begin(9600);//this is the communication port for the XBee
  portGPS.begin(57600);
  lcd.begin(16, 2);
 // portGPS.print("$PMTK104*37"); //code for resetting the GPS
 pinMode(buttonPin, INPUT);
 pinMode(LEDPin, OUTPUT);
 pinMode(xbsp, OUTPUT);
 digitalWrite(xbsp, LOW);
}

void loop() 
{
   buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) 
  {
    enProtocal1(0);
    digitalWrite(LEDPin, HIGH);
  } 
    checkIn();
    checkPower();//check out power consumption

    /**
    * Commands:
    * 1. Get currCheckTm
    * 2. get lastCheckTm 
    * 3. Activate protocol 1. if its on, it will turn off
    */
    int compInput = readASCII();
    if(compInput == 1) 
    {
      Serial.println(currCheckTm);
    }
    else if(compInput == 2)
    {
      Serial.println(lastCheckTm);
    }
    else if(compInput == 3)
    {
      Serial.println("Protocl 1");
      enProtocal1(0);
    }
    
   

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
}
/**
*This will read information from the computer
*and returnt that information as an ASCII character
*/
int readASCII()
{
  int incomingByte = 0;
  int testByte = 0;
          // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                testByte = incomingByte - 48;
                // say what you got:
                Serial.print("I received: ");
                Serial.println(testByte, DEC);
        }
        return testByte;
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
 * Similar to print_float except that it will simply return the value
 * read
 */
static float return_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
    {
  //    Serial.print('*');
    }
  //  Serial.print(' ');
    return -1;
  }
  else
  {
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
    {
      //Serial.print(' ');
    }
    return val;
  }
  smartdelay(0);
  return -1;
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
  portGPS.listen();
  do 
  {
    while (portGPS.available())
      gps.encode(portGPS.read());
  } while (millis() - start < ms);
}
/**
 * This will display coordinate information onto the screen
 */
void disCoor()
{
  gps.f_get_position(&flat, &flon, &age);//get gps coordinates
  lcd.clear();
  lcd.setCursor(0, 0);//start at the top
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6, "Lat:");//print to USB and screen
  lcd.setCursor(0,1);//start at the bottom
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6, "Lon:");//print again
  smartdelay(1000);
}
/**
 * Sends the information through the xbee
 */
void sendCoor()
{
  String delim = "*";
  String message = "NO MESSAGE IS HERE AT THIS TIME";
  float tlat = return_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);//get the lon
  float tlon = return_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);//get the lat
  //lets test and see if we got back actual inforamtion or not
  if((tlat != -1) && (tlon != -1))
  {
    message = "lat:"+ (String)tlat + delim + "lon:" + (String)tlon; 
   // Serial.println(message);
  }
  //no matter what lets add the id number to the front of the message
  message = ID + delim + message + "&";
  Serial.println(message);
  Serial1.print(message);
}

/**
 * This will display the information to the LCD screen
 * and send tht information through the radio
 */
 void checkIn()
 {
    disCoor();
    sendCoor();
    currCheckTm = millis();
    Serial.println(currCheckTm);//DEB
 }
/**
 * This will only get the GPS location and
 * display that information to the user
 */
void simpleFind()
{
  disCoor();
  currFindTm = millis();
}
/**
 * If we received a signal then lets do something with it
 */
void recSig()
{
  String sigRaw;
  
  //are we getting a signal?
//  if(comSerial.available())
//  {
//    sigRaw = comSerial.read();
//    //now process the information
//    //lets check and see who sent this
//    if(sigRaw.substring(0, 2) == "FD")
//    {
//      //field device sent this
//    }
//    else if(sigRaw.substring(0, 2) == "HB")
//    {
//      //The home base sent this out, so might be important!!
//    }
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
//  }
}
/**
 * This is called at the loop and checks the power consumption to see if
 * we need to take power saving actions
 */
void checkPower()
{
  //These are times when we will activate specific power saving functions
  checkpwCT();

  //lets check when we last only found the GPS signal
  checkpwFT();
}
/**
 * Does all the logic to determine which protocal would be
 * the best protocal
 */
void checkpwCT()
{
  unsigned long interval = 0;
  unsigned long pwRateB = 0;
  unsigned long pwRateC = 0;
  unsigned long varianceP1 = 30;


  //lets check when we last checked in
  
  interval = currCheckTm - lastCheckTm;//how long has it been since we last checked in?
  /**
  *We have to add the last time we checked in and the current time together then see
  *if they are within a threshold of say 5 minutes. This means that if the last and current
  *check in both happened x minutes apart, +/- 5 extra minutes, then we will count it
  *towards using protocal 1
  */
  if((lastInterval + interval) > 30 && //floor
    (lastInterval + interval) < 60) //cieling
  {
    p1Counter = p1Counter + 1;
    if(p1Counter == 3)//if we checked in 3 times within threshold
    {
      enProtocal1(interval);//activate the protocal and go with the floor
    }
  }
  else
  {
    p1Counter = p1Counter -1;
    if(p1Counter < 0)
    {
      p1Counter = 0;
    }
  }
  lastInterval = interval;
}
/**
 * Only does logic for when we only got the GPS
 * for the user
 */
void checkpwFT()
{
  
}
/**
 * This will set all the flags for the
 * energy protocals to false
 */
void defFlags()
{
  
}

/**
 * Protocal 1 will deal with regular check ins.
 * It will shut down all parts, and then turn them on
 * 5 minutes before we want to use them
 */
int enProtocal1(unsigned long interval)
{
  //is this already on?
  if(enProt1F)
  {
    enProt1F = false;
    lcd.display();//turn lcd on
    portGPS.print("w");
   // digitalWrite(xbsp, LOW);
    return 1;
  }
  else
  {
    //we need to shut off the GPS, radio, and LCD screen.
    //turn off LCD
    lcd.noDisplay();
    //set GPS to low power mode
    portGPS.print("{\"$PMTK161,0*28\x0D\x0A\"}");
    //set the radio to sleep
    //digitalWrite(xbsp, HIGH);

    //Set GPS to periodic standby
    enProt1F = true;
    return 0;
  }
}
/**
 * 
 */
int enProtocal2()
{
  
}
