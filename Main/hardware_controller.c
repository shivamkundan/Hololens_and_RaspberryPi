//Shivam Kundan
//S17-71-HOL1
//Fall 2017

//Variable to keep track of how many on/off's for each device
int fan_counter=0;  
int lamp_counter=0;
int blender_counter=0;
int song_counter=0;

char *song_list[NUM_OF_SONGS]={"JingleBellRock.wav",
                    "mambo.wav",
                    "ThreeLittleBirds.mp3",
                    "GetLucky.mp3"};
                    
void lcd_client_connected(int i)
/*Display a message when a client connects.*/
{
    char *str_client = (char*)malloc(100);
    char i_str[10];
    strcpy(str_client,"python /home/pi/seniordesign/LCD_Scripts/lcd_client_connected.py ");
    sprintf(i_str, "%d", i+1);
    strcat(str_client,i_str);
    //strcat(str_client,"\"");
    system(str_client);
}

void lcd_client_disconnected(int i)
/*Display a message when a client gets disconnected.*/
{
    char *str_client = (char*)malloc(100);
    char i_str[10];
    strcpy(str_client,"python /home/pi/seniordesign/LCD_Scripts/lcd_client_disconnected.py ");
    sprintf(i_str, "%d", i);
    strcat(str_client,i_str);
    //strcat(str_client,"\"");
    system(str_client);
}

void lcd_received_val(int i)
/*Print received value to LCD. 
  This is helpful for a quick status check/debugging.*/
{
    char *str_client = (char*)malloc(100);
    char i_str[10];
    strcpy(str_client,"python /home/pi/seniordesign/LCD_Scripts/lcd_received_val.py ");
    sprintf(i_str, "%d", i);
    strcat(str_client,i_str);
    system(str_client);
}


void lcd_print(char line2[16])
/*Print to line 2 of LCD.*/
{
    char *str2 = (char*)malloc(100);
    strcpy(str2,"python /home/pi/seniordesign/LCD_Scripts/lcd_print.py ");
    strcat(str2,line2); 
    system(str2);   //Execute in a detached terminal
}

void sig_handler(int signo)
/*Ensures proper termination of all running hardware/threads.*/
{ 
    //Turn off fan, lamp, and blender in case they are on
    system("gpio -g write 17 0");
    system("gpio -g write 27 0");
    system("gpio -g write 22 0");

    //Stop any music that is playing   
    system("killall omxplayer.bin");
    sleep(1);

    //Print/speak messages
    printf("Bye!\n");
    system("screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Speech/system_off.mp3\"");
    system("python /home/pi/seniordesign/LCD_Scripts/lcd_exit.py");
    system("screen -dm bash -c \"python  /home/pi/seniordesign/LCD_Scripts/lcd_clear.py\"");
    
    //Kill all threads associated with this program
    system("killall screen") ;
    
    exit(0);
}

void clear_lcd()
/*Blanks out both lines of LCD.*/
{
    system("screen -dm bash -c \"python /home/pi/seniordesign/LCD_Scripts/clear_lcd.py\"");
}

void play_song()
/*Plays a song from song_list.*/
{
    printf("Playing %s\n",song_list[song_counter]); //Print name of song
    char *str1 = (char*)malloc(100);
    strcpy(str1,"screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Songs/");
    strcat(str1,song_list[song_counter]);
    strcat(str1,"\"");
    system(str1);   //Execute in a detached terminal
    if (song_counter!=NUM_OF_SONGS-1) song_counter++;
    else song_counter=0;
}

void stop_song()
/*Closes omxplayer.*/
{
    system("killall omxplayer.bin");
}

void measure_distance()
/*Use ultrasonic sensor to measure distance. 
  Use values to tell when door is open.
  For E127 lab, door closed is 150cm and
  door open is 152cm or more.*/
{
    long unsigned  int start,stop,total,dist;
    struct timespec gettime_now;
    digitalWrite(outpin, LOW);
    sleep(0.01);
    digitalWrite(outpin, HIGH);
    sleep(0.01);
    digitalWrite(outpin, LOW);

    //Trigger
    while (digitalRead(inpin)==LOW);
    clock_gettime(CLOCK_REALTIME,&gettime_now);
    start=gettime_now.tv_nsec;
    //printf("start: %ld\n",start);
          
    //Echo
    while (digitalRead(inpin)==HIGH);
    clock_gettime(CLOCK_REALTIME,&gettime_now);
    stop=gettime_now.tv_nsec;
    //printf("stop: %ld\n",stop);    

    total=(stop-start);
    //printf("time elapsed: %lu\n",total);
    dist=(3.43*pow(10,-5)*total)/2;
    printf("distance: %lu cm\n",dist);
}

void control_hardware(int data)
/*Receives input integers and controls processes/threads accordingly.*/
{
    //Fan
    if (data == DESK_FAN_ID) {        
        fan_counter++;
        printf("fan counter: %d\n",fan_counter);
   
        if (fan_counter%2==1) {
    	  lcd_print("Fan On");
    	  printf("Fan on\n");
    	  digitalWrite (DESK_FAN_PIN, HIGH) ;
	}
        else {
    	  printf("Fan off\n");
    	  digitalWrite (DESK_FAN_PIN, LOW);
    	  lcd_print("Fan Off");
	}   
    }
    //Lamp
    else if (data == DESK_LAMP_ID) {
        lamp_counter++;
        printf("desk lamp counter: %d\n",lamp_counter);
        if (lamp_counter%2==1) {
    	  lcd_print("Lamp on");
    	  printf("Lamp on\n");
    	  digitalWrite (DESK_LAMP_ID, HIGH);
	}
          else {
    	    lcd_print("Lamp off");
    	    printf("Lamp off\n");
    	    digitalWrite (DESK_LAMP_ID, LOW);
        }  
    }
    //Blender
    else if (data == BLENDER_ID) {
        blender_counter++;
        lcd_print("Blender");
        printf("Blender\n");
   
        if (blender_counter%2==1) {
          lcd_print("Blender On");
          printf("Blender on\n");
          digitalWrite (BLENDER_PIN, HIGH) ;
    }
        else {
          printf("Blender off\n");
          digitalWrite (BLENDER_PIN, LOW);
          lcd_print("Blender Off");
    }
    }
    //Play Song
    else if (data == PLAY_SONG) {
        lcd_print("Play Song");
        stop_song();
        printf("Play song\n");
        play_song();       
    }
    //Next Song
    else if (data == NEXT_SONG) {
        lcd_print("Next Song");
        printf("Next song\n");
        stop_song();
        play_song();
    }
    //Stop Song (kill omxplayer)
    else if (data == STOP_SONG) {
        lcd_print("Stop Song");
        printf("Stop song\n");
        stop_song();
    }
    //Random Song
    else if (data == RANDOM_SONG) {
        lcd_print("Random song");
        printf("Random song\n");
        int random_num =rand()%NUM_OF_SONGS;
        char *str1 = (char*)malloc(100);
        strcpy(str1,"screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Songs/");
        strcat(str1,song_list[random_num]);
        strcat(str1,"\"");
        system(str1);   //Execute in a detached terminal
    }
    //Measure distance to door
    else if (data == ULTRASONIC_SENSOR_ID) {
        lcd_print("Door Sensor");
        printf("Ultrasonic distance sensor\n");
    //system("screen bash -c \"python sensor.py\"");
    //measure_distance();
    }
    //Temperature and Humidity
    else if (data == TEMP_SENSOR_ID) {
        lcd_print("Temp / Humidity");
        printf("Temperature and humidity\n");
	    system("python /home/pi/seniordesign/Sensor_Scripts/temperature.py");
    }
    //Room brightness
    else if (data == LUX_SENSOR_ID) {
        lcd_print("Room Brightness");
        printf("Lux sensor\n");
        system("python /home/pi/seniordesign/Sensor_Scripts/light_sensor.py");
    }
    else {
      //Display unmapped value on LCD
      lcd_received_val(data);
      printf("Number not mapped to any functions\n");
    }
}
