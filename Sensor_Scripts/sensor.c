#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LED	0
#define inpin   4  //Trigger bcm_pin=23
#define outpin  1  //Echo    bcm_pin=24
//power bcm_in=

int main (void)
{
  printf ("Ultrasonic sensor\n") ;
  long unsigned  int start,stop,total,dist;
  struct timespec gettime_now;
  

  wiringPiSetup () ;
  pinMode (LED, OUTPUT) ;
  pinMode (outpin, OUTPUT) ;
  pinMode (inpin, INPUT) ;

  while(1)
  {
    digitalWrite(outpin, LOW);
    sleep(0.01);
    digitalWrite(outpin, HIGH);
    sleep(0.01);
    digitalWrite(outpin, LOW);
    

    while (digitalRead(inpin)==LOW);
    clock_gettime(CLOCK_REALTIME,&gettime_now);
    start=gettime_now.tv_nsec;
    //printf("start: %ld\n",start);
		  
    while (digitalRead(inpin)==HIGH);
    clock_gettime(CLOCK_REALTIME,&gettime_now);
    stop=gettime_now.tv_nsec;
    //printf("stop: %ld\n",stop);    

    total=(stop-start);
    printf("time elapsed: %lu\n",total);
    dist=(3.43*pow(10,-5)*total)/2;
    printf("distance: %lu cm\n",dist);
    sleep(1);
    
    
  }
  return 0 ;
}
