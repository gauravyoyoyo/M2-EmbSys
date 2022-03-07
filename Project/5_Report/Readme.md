# __Report__

# ELEVATOR CONTROL SYSTEM

## Tabel of Content
  1. [Introduction](#introduction)
   
  2. [Purpose](#purpose)
   
  3. [Requriments](#requriments)
   
     * [High Level Requriment](#high-level-requriment)
   
     * [Low Level Requriment](#low-level-requriment)
    
  4. [Block Diagram](#block-diagram)
   
  5. [Design Metrics](#design-metrics)
   
     * [Power Dissipation](#power-dissipation)
   
     * [Performance](#performance)
    
     * [Process Deadlines](#process-deadlines)
    
     * [User Interface](#user-interface)
   
   6. [Components](#components)
   
       * [Sensors](#sensors)
       
       * [Actuators](#actuators)
       
       * [User Interaction](#user-interaction)
   
   7. [Circuit Diagram](#circuit-diagram)
  
   8. [Description of schematic](#description-of-schematic)
    
   9. [System Testing](#system-testing)
   
   10. [Test Plan and Output](#test-plan-and-output)
   
## Introduction
   Elevator are the most common system now days .We use elevator every day to move goods or peoples vertically in a building such as shooping centre, Office building, Hospitals and more. Help people to move to desired floor in short time as compare to staircase.
   In this project, microcontroller, sensors, led, motor, buzzer, keypad, light and fan is used. The user has to select the option to go up or down, press the button in keypad the doors will open user has to enter into the lift select he desired floor. Once the lift reaches to desired floor then it will come back to ground floor.
 
 
 
## Purpose
 * Elevator is avertical transportation system commonly use in buildings to move people between multiple floors with proper saftey



## Requriments
   
   #### High Level Requriment
   
   | HLR |               DESCRIPTION|
   |:----:|:--------------------------------------------------|
   |HLR01|Elevator car must come to floor, where it has called|
   |HLR02|Door must open when car reaches the floor|
   |HLR03|Door must closed before car start moving toward desired floor|
   |HLR04|It shall display the position of the car using led light|
   |HLR05|It shall give buzzer sound before opening and closing the door|
   
   #### Low Level Requriment
   
   | LLR |          DESCRIPTION|
   |:----:|:--------------------------------------------------|
   |LLR01|Door must be open for 10 Seconds|
   |LLR02|The buzzer must beep when lift reaches to foor|
   |LLR03|It shall have fan, light, switches, buzzer|
   |LLR04|It must have Telephone for emergency contact|
   |LLR05|It shall has Fixed speed for the movement of car|



## Block Diagram 

![block diagram](https://user-images.githubusercontent.com/57553580/154838648-be8b8d37-4746-46b3-b324-f003355fb619.jpg)

## Flow Chart

![Flow chart](https://user-images.githubusercontent.com/57553580/154838695-c8b8f4a6-16fb-4539-8278-741047e402b6.jpg)



## Design Metrics

   #### Power Dissipation
   * As required by Motors and display and microcontroller unit
   #### Performance 
   * The responce time of a car to come to a floor where it is called must be less than 30 Seconds
   * Door must be open for 10 seconds 
   * Buzzer should beep to 1 second
   #### Process Deadlines
   * Door must be closed after 5 second when user press the desired floor button in keypad.
   * light and fan start with 1 second when the lift door opens and switch off when no one inside the lift car
   * If user not press the desired floor button lift must beep after 10 second 
   #### User Interface
   * There is a LCD to show the position of lift and the input given by hr user
   * There is a keypad inside lift car for the user input.
   * There is LED indicator at every floor.



## Components

   #### Sensors
   * __Level Sensor__ : It is use to calculate the alignment of elevator car between the two floor.
   * __Door Sensor__: Ti is device that detects a passenger or an object on the doorway which prevents the doors from closing.
  
   #### Actuators
   * __Motor__ : A motor at the top of the shaft turns a sheave—essentially a pulley—that raises and lowers cables attached to the cab and a counterweight.
   * __Limit Switch__ : They are used in elevators to notify the system that the elevaaor car has reached the desired floor.
   
   #### User Interaction
   * __Keypad__ : It is used for user to input the desired floor number, Call in Emergency service, Alram Switch.
   * __LCD Display__: It is used to display the current status of elevator car, on which floor it is. 
   
   
## Circuit Diagram
![Circuit Diagram](https://user-images.githubusercontent.com/57553580/156881542-4c79c26b-d43d-44bf-98b9-1c29abeccb05.png)

## Description of schematic

   * Fn (n = 1,2,3,4) - There are sensors Level switch which connected on every floor. Each sensor is connected by a resistor and link up with analog input A0 on Arduino Uno. Each sensor when is active has own signal from interval (0;1024). Level of signal depends on the resisting of resistor.

   * Cn (n = 1,2,3,4) - There are buttons of calling the elevator (Call bar). They are connected by resistors with different nominals and link up to analog input A1 on Arduino Uno. The call bar has two buttons (only on 2,3 floors).These buttons are used to call Elevator, the dependency wherever you want to go (up/down) - It's not implemented at the moment! Also each floor has one led which light up when this floor is called.

   * CS (Calibration Switch) is a calibration switch located on the first floor. When the Elevator starts to work, first and foremost, it is calibrating (down to the first floor, while CS is not pressed).

   * CBn ( n = 1,2,3,4) - There are the Elevator call buttons which are located in the cabin. They also are connected by resistors with different nominals and link up to analog input A0 on Arduino nano(In the Cabin).

   * Lift Motor -  a step motor with connected by driver module ULN2003.

   * Display - I connected the Lcd display 16x2 by module I2C. The display shows the floor at the moment.

   * Door Servor Motor - The servo drive is used to open and close the door.

   * Buzzer - It is used to signal that the Elevator has arrived.

## System Testing

| TEST ID |                DESCRIPTION|
|:----:|:--------------------------------------------------|
|TC01|All user Command Must function correctly|
|TC02|All the content of LCD should appear as per the program|
|TC03|Check the functionality of Keypad|
|TC04|Check Buzzer at each floor |
|TC05|Check the timing of car to arrival and departure|
|TC06|Check the time of door opening and closing|
|TC07|Check the infrared sensor |
|TC08|Check the funcationality of lift at time of power failure|
|TC09|Check telephone|
|TC08|Check the emergency alaram|
|TC09|Check the working of fan and light inside the car|
|TC10|Check the Led indiactor at each floor showing correct indiaction or not|

## Test plan and output

### HIGH LEVEL TEST PLAN / Integrated test plan

|Test ID|Description|Input|Expected output|Actual Output|Passed or not|
|:------:|:----------------------|:---------------------|:--------------------|:-------------------|:---|
|HLR01|Level Sensor| 0v or 5v|Shall send lift to desired floor|Shall send lift to desired floor| Passed|
|HLR02|calibration Switch|On|Shall send cabin to floor 1|Shall send cabin to floor 1|Passed|
|HLR03|Keypad switch|press any key (1,2,3,4)|Shall send cabin to the floor which key is pressed|Shall send cabin to the floor which key is pressed|Passed|
|HLR04|Floor Buttons|On|Shall send cabin to floor where button is pressed|Shall send cabin to floor where button is pressed|Passed|
|HLR05|Buzzer|Data from ardunio (1)|Buzzing Sound|Buzzing Sound|Passed|
|HLR06|Door motor|Data from ardunio (1)|door Shall open|door shall open|Passed|
|HLR07|SERVO motor|command from ardunio |shall change position of cabin|shall change position of cabin|Passed|


