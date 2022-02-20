
# ELEVATOR CONTROL SYSTEM

## Introduction
   Elevator are the most common system now days .We use elevator every day to move goods or peoples vertically in a building such as shooping centre, Office building, Hospitals and more. Help people to move to desired floor in short time as compare to staircase.
   In this project, microcontroller, sensors, led, motor, buzzer, keypad is used. The user has to select the option to go up or down, press the button in keypad the doors will open user has to enter into the lift select he desired floor. Once the lift reaches to desired floor then it will come back to ground floor.
 
 ## Purpose
     * To move people from one floor to another with proper saftey
    
 ## Inputs 
     * User need to tell the desire floor.
    
 ## Outputs
     * lift car will come to the floor from where it has called.
     * Led indicator will be on 
     * Door opens , Light and fan Switch on inside the car
 ## REQURIMENTS
   
   ### High Level Requriment
   
   | HLR |DESCRIPTION|
   |:----:|:--------------------------------------------------|
   |HLR01|Elevator car must come to floor, where it has called|
   |HLR02|Door must open when car reaches the floor|
   |HLR03|Door must closed before car start moving toward desired floor|
   |HLR04|Should display the position of the car using led light|
   |HLR05|Should give buzzer sound before opening and closing the door|
   
   ### Low Level Requriment
   
   | LLR |DESCRIPTION|
   |:----:|:--------------------------------------------------|
   |LLR01|Door should be open for 10 Seconds|
   |LLR02|Buzzer beep when lift reaches to foor|
   |LLR03|Motor and gear box setup|
   |LLR04|fan, light, switches, buzzer|
   |LLR05|Telephone for emergency contact|
   |LLR06|Fixed speed for the movement of car|
   |LLR0||
   
## Block Diaggram 

![block diagram](https://user-images.githubusercontent.com/57553580/154838648-be8b8d37-4746-46b3-b324-f003355fb619.jpg)

## Flow Chart

![Flow chart](https://user-images.githubusercontent.com/57553580/154838695-c8b8f4a6-16fb-4539-8278-741047e402b6.jpg)

## Design Metrics

   ### Power Dissipation
   * As required by Motors and display and microcontroller unit
   ### Performance 
   * The responce time of a car to come to a floor where it is called must be less than 30 Seconds
   * Door must be open for 10 seconds 
   * Buzzer should beep to 1 second
   ### Process Deadlines
   * Door must be closed after 5 second when user press the desired floor button in keypad.
   * light and fan start with 1 second when the lift door opens and switch off when no one inside the lift car
   * If user not press the desired floor button lift must beep after 10 second 
   ### User Interface
   * There is a LCD to show the position of lift and the input given by hr user
   * There is a keypad inside lift car for the user input.
   * There is LED indicator at every floor.
