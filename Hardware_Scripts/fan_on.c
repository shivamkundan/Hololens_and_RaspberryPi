
#include <stdio.h>
#include <wiringPi.h>

#define	FAN	1

int main (void)
{
  printf ("Desk fan on\n") ;

  wiringPiSetup () ;
  pinMode (FAN, OUTPUT) ;
  digitalWrite (FAN, HIGH) ;	// On

  return 0 ;
}
