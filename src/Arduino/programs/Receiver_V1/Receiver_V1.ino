/*****************************************************************
This program is designed to read information from
field devices and send that information to the to the computer.
Should it receive information from the computer it will send that
information out to other devices
*****************************************************************/

// Leonardo Serial  is the USB port
// Leonardo Serial1 is pins 0 and 1

const int ID_CONST = 0;//the receiver ID will aways be 0

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  Serial1.begin(9600);  //XBee/UART1/pins 0 and 1
  Serial.begin(9600);   //USB
   pinMode(13, OUTPUT);
  
}

void loop()
{

  if (Serial1.available())   //XBee/UART1/pins 0 and 1
  { // If data comes in from XBee, send it out to serial monitor
    process();
    
  }
}
/**
 * Used to process the incoming information and do something useful with it
 */
int process()
{
  //* is used as the delimiter between sections of information
  char currentSym;//this will store the current symbol being read
  String messageBuffer; // used for storing mutliple characters to process
  //first we need to check and see if the message is for the receiver
  currentSym = Serial1.read();
  if(currentSym != '@')//if we dont get this first somethings wrong
  {
    return -1;
  }
  //if we got here then it is a message and we can read the ID
  messageBuffer = readSection();
  if(messageBuffer != "0")//if its not for us
  {
      return 0;
  }

  //if it is for us then lets read the message
  messageBuffer = readSection();
}
/**
 * This will read a section of the message and return the characters
 * in a string
 */
String readSection()
{
  char currentSym = '!';//if we send back this there was a problem
  String messageBuffer;
  while(currentSym != '*')
  {
    currentSym = Serial1.read();//read the input
    messageBuffer = messageBuffer + currentSym;//store it here
  }

  return messageBuffer;
}

