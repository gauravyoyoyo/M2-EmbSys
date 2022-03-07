# Test plan and output

## HIGH LEVEL TEST PLAN / Integrated test plan

|Test ID|Description|Input|Expected output|Actual Output|Passed or not|
|:------:|:----------------------|:---------------------|:--------------------|:-------------------|:---|
|HLR01|Level Sensor| 0v or 5v|Shall send lift to desired floor|Shall send lift to desired floor| Passed|
|HLR02|calibration Switch|On|Shall send cabin to floor 1|Shall send cabin to floor 1|Passed|
|HLR03|Keypad switch|press any key (1,2,3,4)|Shall send cabin to the floor which key is pressed|Shall send cabin to the floor which key is pressed|Passed|
|HLR04|Floor Buttons|On|Shall send cabin to floor where button is pressed|Shall send cabin to floor where button is pressed|Passed|
|HLR05|Buzzer|Data from ardunio (1)|Buzzing Sound|Buzzing Sound|Passed|
|HLR06|Door motor|Data from ardunio (1)|door Shall open|door shall open|Passed|
|HLR07|SERVO motor|command from ardunio |shall change position of cabin|shall change position of cabin|Passed|



