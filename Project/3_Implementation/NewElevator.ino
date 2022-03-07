#include<Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<Servo.h>
#include <stdlib.h >
#include <SPI.h>
/////INIT hardware part////
#define CALL_BUTTONS A1    //--------Here is connection buttons of colling elevator
#define FLOOR_SENSORS A0   //--------Here is connection sensors which are localed on floors
#define CALIBRATION_SENSOR 2 //------Here is connected button for instalation the elevator on first floor
///////////////////////////
#define DEFAULT_SPEED 300 // Default speed of elevator
#define SLOW_SPEED 150  // Default speed of elevator when it is landing
#define N 4 //Number of floor

////////Flags of moving the elevator//////////
#define MOVING_UP 1 //This flag means that the elevator is moving up
#define MOVING_DOWN -1 //This flag means that the elevator is moving down
#define NOT_MOVING 0
/////////////////////////////////////////////

#define NOT -1

///////COMANDS FOR PANEL_CONTROL ///////
#define ELEVATOR_IS_ERIVED 1


#define SIGNAL_PIN 12
#define BUTTON_SIGNAL A2
#define DOOR_PIN 3

#define DOOR_IS_CLOSED 1
#define DOOR_IS_OPENED 2
#define CALL_ON_FLOOR 3


char command[2]={'0','0'};
boolean flagCallFloor = true;

unsigned long timeDelay = 0;
boolean isElevatorErrived = true;

Servo door;


////////////////Init motor///////////////////////
unsigned short stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 8,10,9,11); // Conected Step motor
/////////////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27,16,2);//Connected LDC display

short registeredFloor;
short arrayFloor[N] = {-1,-1,-1,-1};
short ledFloor[N] = {7,6,5,4};
short executableFloor = NOT; // -1 it means not called floors
boolean canWork = false;
boolean landed = false;
boolean waitForNext = true;
short flagMoving = NOT_MOVING;
short calledFloorFromCabine = NOT;


boolean flagPressSameBtn = true;

void setup() {
   Serial.begin(9600);
  pinMode(CALL_BUTTONS,INPUT);
  pinMode(FLOOR_SENSORS,INPUT);
  pinMode(CALIBRATION_SENSOR, INPUT);
  
  for(byte i = 0; i < N; i++)
    pinMode(ledFloor[i],OUTPUT);
  
  myStepper.setSpeed(DEFAULT_SPEED);

  lcd.begin(16,2);                            
  lcd.backlight();                     
  printInf();
  calibration();

  pinMode(SIGNAL_PIN,OUTPUT);
  pinMode(BUTTON_SIGNAL,INPUT);
  
  door.attach(DOOR_PIN);
  
  Serial.begin(9600);
  closeDoor();

  for(byte i = 0; i < 5; i++)
  {
    digitalWrite(SIGNAL_PIN,HIGH);
    delay(100);
    digitalWrite(SIGNAL_PIN,LOW);
    delay(100);
  }
}
void loop() 
{
  readComFromPanelControl();   
  
  registerFloor();
  writeTurn();

  if(waitForNext && canWork)
    nextFloor(); 
 
  if(executableFloor != NOT && canWork)
    moveElevator();
    
     readSerial(command);
   decodeCommand(command);

   short calledFloor = whatFloorCall();

   if(calledFloor != 0 && flagCallFloor)
   {
    sendCommand(CALL_ON_FLOOR);
    calledFloor = 0;
    flagCallFloor = false;
   }

   if( isElevatorErrived && millis() - timeDelay >= 5000)
   {
    closeDoor();
    isElevatorErrived = false;
   }
   
  delay(10);
}

void nextFloor()
{ 
  
  if(calledFloorFromCabine != NOT && calledFloorFromCabine != registeredFloor )
  {
    executableFloor = calledFloorFromCabine;
    landed = false;
    waitForNext = false;
    return;
  }

  if(arrayFloor[0] == NOT)
  {
    for(byte i = 0; i < N; i++)
    {
      swipeArray(arrayFloor);
      if(arrayFloor[0] != NOT)
        break;
    }
  }
    executableFloor = arrayFloor[0];

    if(executableFloor != NOT)
    {
      swipeArray(arrayFloor);
      landed = false;
      waitForNext = false;
    }

}

void swipeArray(short * arrayFloor){
  for(byte i = 0; i < N - 1; i++)
    arrayFloor[i] = arrayFloor[i+1];
  arrayFloor[N-1] = NOT;  
}

void moveElevator()
{

   if(executableFloor == registeredFloor && !landed)
  {
    if(flagMoving == MOVING_UP)
      moveUpMiddle();
    else if(flagMoving == MOVING_DOWN)
       moveDownMiddle();
       
    ElevatorErrived(executableFloor);
    return;    
  }
    
  if(executableFloor > registeredFloor)
  {
    moveUp();
    flagMoving = MOVING_UP;
  }else if(executableFloor < registeredFloor)
  {
    moveDown();
    flagMoving = MOVING_DOWN;
  }
}

void ElevatorErrived(short errivedOnFloor)
{

  for(byte i = 0; i < N; i++)
    if(arrayFloor[i] == errivedOnFloor)
        arrayFloor[i] = NOT;

  executableFloor = NOT;
  turnOffLedOnFloor(errivedOnFloor);
  sendCommandToPanelControl(ELEVATOR_IS_ERIVED);
  flagMoving = NOT_MOVING;
  waitForNext = true;
  
  if(calledFloorFromCabine == registeredFloor)
    calledFloorFromCabine = NOT;
    
  landed = true;
  canWork = false;
}

void turnOffLedOnFloor(short errivedOnFloor)
{
  digitalWrite(ledFloor[errivedOnFloor-1],LOW);
}

void writeTurn()
{ 
  short localCalledFloorExpected = calledFloor();
 
  ///////if we call the elevator on floor wheme we are, elevator will just send command to the cabin for opening door
  if( (localCalledFloorExpected == registeredFloor && flagPressSameBtn && executableFloor == NOT) || 
      (calledFloorFromCabine == registeredFloor && executableFloor == NOT && flagPressSameBtn ))
  {
    digitalWrite(ledFloor[localCalledFloorExpected - 1],HIGH);
    delay(10);
    digitalWrite(ledFloor[localCalledFloorExpected - 1],LOW);
    sendCommandToPanelControl(ELEVATOR_IS_ERIVED);
    calledFloorFromCabine = NOT;
    flagPressSameBtn = false;
    return;
  }

  ///here we create queue calling the elevator
  if(localCalledFloorExpected != NOT)
  {
    if(executableFloor == NOT && localCalledFloorExpected == registeredFloor)
      return;

    //if called floor is in queue, it will not add to queue again
    for (byte i = 0; i < 4;i++)
      if (localCalledFloorExpected == arrayFloor[i])
          return;
          
      for (byte i = 0; i < 4; i++)
        if (arrayFloor[i] == -1)
        {
          arrayFloor[i] = localCalledFloorExpected;
          digitalWrite(ledFloor[abs(localCalledFloorExpected)-1],HIGH);// Turn on led on floor which called
          return;
        }
  }
}
void calibration()
{
  for (byte i = 0; i < N; i++)
    digitalWrite(ledFloor[i], HIGH);
  lcd.setCursor(0,0); 
  lcd.print("  CALIBRATION...");
  
  while(!digitalRead(CALIBRATION_SENSOR))
    myStepper.step(-10);
    
  for (byte i = 0; i < N; i++)
    digitalWrite(ledFloor[i], LOW);
    
  registeredFloor = 1;  
  lcd.clear();
  lcd.print("Floor:1");
}

void moveUp()
{
  myStepper.step(50);
}

void moveUpMiddle()
{
  myStepper.setSpeed(SLOW_SPEED);

  switch(registeredFloor)
  {
    case 2:
      myStepper.step(950);
      break;
    case 3:
      myStepper.step(900);
      break;
    case 4:
      myStepper.step(900);
      break;
  }
  myStepper.setSpeed(DEFAULT_SPEED);
}

void moveDownMiddle()
{
  myStepper.setSpeed(SLOW_SPEED);
  switch(registeredFloor)
  {
    case 1:
      myStepper.step(-700);
      break;
    case 2:
      myStepper.step(-950);
      break;
    case 3:
      myStepper.step(-950);
      break;
    case 4:
      myStepper.step(-650);
      break;
  }
  myStepper.setSpeed(DEFAULT_SPEED);
}

void moveDown()
{
  myStepper.step(-50);
}

void registerFloor()
{
  unsigned short value = analogRead(FLOOR_SENSORS);
  short bufRegisteredFloor = 0;
  if(value >= 221 && value <= 231) bufRegisteredFloor = 4;
  else if(value >= 300 && value <= 310) bufRegisteredFloor = 3;
  else if(value >= 395 && value <= 405) bufRegisteredFloor = 2;
  else if(value >= 691 && value <= 701) bufRegisteredFloor = 1;
  
  if(bufRegisteredFloor != 0 && bufRegisteredFloor != registeredFloor)
  {
    registeredFloor = bufRegisteredFloor;
    printFloor();
  }
}

short calledFloor()
{
  short value = analogRead(CALL_BUTTONS);

  if(value >= 85 && value <= 95) return 4;
  else if(value >= 333 && value <= 345) return 3;
  else if(value >= 175 && value <= 185) return 3;
  else if(value >= 505 && value <= 515) return 2;
  else if(value >= 402 && value <= 412) return 2;  
  else if(value >= 670 && value <= 705) return 1;
  else return NOT;
  
}

void printFloor()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Floor:");
  lcd.print(registeredFloor);
}

void printInf()
{
  lcd.print("    Elevalor");
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  delay(3000);
  lcd.setCursor(0,1);
  lcd.println("LCD DISPLAY");
  delay(2000);
  lcd.clear();
}

void sendCommandToPanelControl(short command)
{
  char str[2] = {'0','0'};
  switch(command)
  {
    case ELEVATOR_IS_ERIVED:
      str[0] = 'e';
      str[1] = 'f';
        break;    
  }

  Serial.write(str,2);
}

void readComFromPanelControl()
{
  if(!Serial.available())
    return;

    byte i = 0;
    char str[2] = {'0','0'};
    delay(100);
    while(Serial.available())
      str[i++] = Serial.read();  

   if(str[0] == 'd' && str[1] == 'o')
    canWork = false;
   else if(str[0] == 'd' && str[1] == 'c')
   {
    canWork = true;
    flagPressSameBtn = true;
   }else if(str[0] == 'c')
   {
      calledFloorFromCabine = str[1] - '0';
   }
}

void decodeCommand(char str[2])
{
  if(str[0] == '0' && str[1] == '0')
    return;
  
  if(str[0] == 'e' && str[1] == 'f')
  {
    elevatorIsArrived();
        str[0] = '0';
        str[1] = '0';
  }
  
}

void elevatorIsArrived()
{ isElevatorErrived = true;
  digitalWrite(SIGNAL_PIN,HIGH);
  delay(500);
  digitalWrite(SIGNAL_PIN,LOW);
  openDoor();
  flagCallFloor = true;
  timeDelay = millis();
}

void readSerial(char str[2])
{
  str[0] = '0';
  str[1] = '0';

  if(Serial.available())
  {
    delay(100);
    byte i = 0;
    while(Serial.available() && i<2)
      str[i++] = Serial.read();
  }   
}

void sendCommand(short codeOfCommand)
{
  char com[2] = {'0','0'};
  switch(codeOfCommand)
  {
    case DOOR_IS_CLOSED:
      com[0] = 'd';
      com[1] = 'c';
    break;

    case DOOR_IS_OPENED:
      com[0] = 'd';
      com[1] = 'o';
      break;

    case CALL_ON_FLOOR:
      com[0] = 'c';
      itoa(calledFloor,&com[1],10);
     com[1] = (char)calledFloor;   
      break;
  }
   Serial.write(com,2);
}

void openDoor()
{
  door.write(180);
  sendCommand(DOOR_IS_OPENED);

}

void closeDoor()
{
  door.write(0);
  sendCommand(DOOR_IS_CLOSED);
}

short whatFloorCall()
{
  short value = analogRead(BUTTON_SIGNAL);
  
  if(925 <= value && value <= 935) return 1;
  else if(885 <= value && value <= 895) return 2;
  else if(759 <= value && value <= 769) return 3;
  else if(696 <= value && value <= 706) return 4;
  else return 0;
}
