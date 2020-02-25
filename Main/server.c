//Shivam Kundan
//S17-71-HOL1
//Fall 2017

/*This program -
  Handles multiple HoloLens socket connections.
  Sends message to hardware controller.
  Sends message back to client.
  Socket programming code modified from: goo.gl/mPWPbP
  */

#include <stdio.h> 
#include <string.h>                 //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>               //FD_SET, FD_ISSET, FD_ZERO macros 
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>
#include "mappings.h"               //For misc values used in the code  
#include "hardware_controller.c"    //Contains all hardware control functions

#define PORT 51717

void begin_server();               //Maintains communication with hololens(s)
void control_hardware(int data);   //Sends received integers to hardware control functions

int main(int argc , char *argv[])  
{  
    //Initialize GPIO pins using wiringPi mapping
    wiringPiSetup();
    pinMode (DESK_FAN_PIN, OUTPUT);
    pinMode (LAMP_PIN, OUTPUT);
    pinMode(BLENDER_PIN,OUTPUT);
    pinMode (outpin, OUTPUT);
    pinMode (inpin, INPUT);
    
    //Signal handling
    signal(SIGINT,sig_handler);

    //Will be used later for random song function
    srand(time(NULL));   

    //Display and play welcome message
    system("screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Speech/system_on.mp3\"");
    system("python  /home/pi/seniordesign/LCD_Scripts/lcd_clear.py");
    system("python  /home/pi/seniordesign/LCD_Scripts/lcd_systemon.py");
    
    begin_server();

    return 0;  
}

void begin_server()
/*Maintains communication with one or more hololens. Sends received data to
  hardware controlling functions*/
{
    int opt = 1;  
    int master_socket , addrlen , new_socket , client_socket[30] , 
          max_clients = 30 , activity, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;     
    char buffer[1025];  //data buffer of 1K 
        
    //set of socket descriptors 
    fd_set readfds;  
         
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++) {  
        client_socket[i] = 0;  
    }  
        
    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //set master socket to allow multiple connections , 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 ) {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
        
    //bind the socket to localhost port PORT 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
        
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0) {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");
     
        
    while(1)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
    
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
            
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++) {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
            
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)              {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  

            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++) {  
                //if position is empty 
                if( client_socket[i] == 0 ) {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);

                    //Play a welcome message 
                    system("screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Speech/client_connected.mp3\""); 
                    
                    //Display number of clients connected on LCD display
                    lcd_client_connected(i);
                    break;  
                }  
            }  
        }  
            
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++) {  
            sd = client_socket[i];           
            if (FD_ISSET( sd , &readfds)) {  
                //Check if it was for closing , and also read the incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                  //Stop the currently playing song (if there is one)
                  stop_song();  
                  sleep(1);

                  //Play a goodbye message
                  system("screen -dm bash -c \"omxplayer -o local /home/pi/seniordesign/Audio_Files/Speech/client_disconnected.mp3\"");
                  
                  //Make sure fan is off
                  digitalWrite (DESK_FAN_PIN, LOW);
                  
                  //Display message on LCD
                  lcd_client_disconnected(i);
                
                  //Somebody disconnected , get his details and print 
                  getpeername(sd , (struct sockaddr*)&address , \
                    (socklen_t*)&addrlen);  
                  printf("Host disconnected , ip %s , port %d \n" , 
                      inet_ntoa(address.sin_addr) , ntohs(address.sin_port));    
                    //Close the socket and mark as 0 in list for reuse 
                  close( sd );  
                  client_socket[i] = 0;  
                }  
                   
                //Control GPIO pins
                else
                {  
                    int data = atoi(buffer);
                    printf("\nreceived: %d\n", data);
                    
                    control_hardware(data);

                    //Send back messages
                    //buffer[valread] = '\0';  
                    //send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
    }  
}