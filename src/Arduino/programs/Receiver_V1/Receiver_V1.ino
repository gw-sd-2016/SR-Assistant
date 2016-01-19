/*****************************************************************
  This program is designed to read information from
  field devices and send that information to the to the computer.
  Should it receive information from the computer it will send that
  information out to other devices
*****************************************************************/

// Leonardo Serial  is the USB port
// Leonardo Serial1 is pins 0 and 1

const int ID_CONST = 0;//the receiver ID will aways be 0
/**
   code to run at startup
*/
void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  Serial1.begin(9600);  //XBee/UART1/pins 0 and 1
  Serial.begin(9600);   //USB
  pinMode(13, OUTPUT);

}
/**
   Main loop arduino runs
*/
void loop()
{

  if (Serial1.available())   //XBee/UART1/pins 0 and 1
  { // If data comes in from XBee, send it out to serial monitor
    process();

  }
}
/**
 * Makes the LED blink. Used for giving visual feedback when the arduino is doing something
*/
void ledBlink()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(100);  
}
/**
   Used to process the incoming information and do something useful with it
*/
int process()
{
  //* is used as the delimiter between sections of information
  char currentSym;//this will store the current symbol being read
  String messageBuffer; // used for storing mutliple characters to process
  //first we need to check and see if the message is for the receiver
  currentSym = Serial1.read();
  if (currentSym != '@') //if we dont get this first somethings wrong
  {
    return -1;
  }
  //if we got here then it is a message and we can read the ID
  messageBuffer = readSection();
  if (messageBuffer != "0") //if its not for us
  {
    return 0;
  }

  //if it is for us then lets read the message and send to the computer

  //grab the lat
  messageBuffer = readSection();
  Serial.println(messageBuffer);
  //grab the long
  messageBuffer = readSection();
  Serial.println(messageBuffer);
  //grab the message
  messageBuffer = readSection();
  Serial.println(messageBuffer);
  //we succeeded so return a good value
  return 1;
}
/**
   This will read a section of the message and return the characters
   in a string
*/
String readSection()
{
  char currentSym = '!';//if we send back this there was a problem
  String messageBuffer;
  while (currentSym != '*')
  {
    currentSym = Serial1.read();//read the input
    messageBuffer = messageBuffer + currentSym;//store it here
  }

  return messageBuffer;
}
/**
   Used for testing out the program and making sure everything works alright
*/
void testRun()
{
  Serial.println("Testing");
  ledBlink();
  delay(1000);
}

