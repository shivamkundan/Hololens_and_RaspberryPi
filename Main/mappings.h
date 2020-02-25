//Shivam Kundan
//S17-71-HOL1
//Fall 2017

/*Values in braces are what the hololens client sends.*/

//Mapping BCM pins to wiringPi library
#define DESK_FAN_PIN	0  //Desk Fan
#define LAMP_PIN       	2  //Desk Lamp
#define BLENDER_PIN 	3  //Blender
#define inpin   		5  //Trigger
#define outpin  		4  //Echo

//Hardware control mappings
#define DESK_FAN_ID         (1)
#define DESK_LAMP_ID        (2)
#define BLENDER_ID        	(3)

//Music Player mappings
#define PLAY_SONG           (4)
#define NEXT_SONG           (5)
#define STOP_SONG           (6)
#define RANDOM_SONG         (7)

//Sensor control mappings
#define ULTRASONIC_SENSOR_ID (8)
#define TEMP_SENSOR_ID       (9)
#define LUX_SENSOR_ID        (10)

//Others
#define NUM_OF_SONGS 4