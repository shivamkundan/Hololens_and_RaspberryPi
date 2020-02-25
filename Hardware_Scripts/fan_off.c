#include <stdio.h>
#include <wiringPi.h>

#define	FAN	1

int main (void)
{
  printf ("Desk fan off\n") ;

  wiringPiSetup () ;
  pinMode (FAN, OUTPUT) ;

  digitalWrite (FAN, LOW); 	//Off

  return 0 ;
}
