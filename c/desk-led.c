/*  File name:    desk-led.c
 *  Description:  C code main file to control a number of LED strips using individual ws2812 programmable LEDs.
 *  Author:       Chris Saiko
 *  Email:        chris.saiko@gmail.com

    This file is part of desk-led.

    desk-led is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    desk-led is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with desk-led.  If not, see <https://www.gnu.org/licenses/>.
    
*/

#include "desk-led.h"
#include "dlserial.h"
#include "dlargs.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char** argv) {

  struct arguments arguments;
  /* Default values. */
  arguments.quiet = 0;
  arguments.verbose = 0;
  arguments.smooth = 0;
  arguments.breathe = 0;
  arguments.zones = "0";
  arguments.seconds = "0";
  arguments.devport = "/dev/ttyUSB0";

  /* Parse our arguments; every option seen by parse_opt will be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf ("RED = %s\nGREEN = %s\nBLUE = %s\n"
          "SMOOTH = %s\nBREATHE = %s\n"
          "MODE_OPTION = %s seconds\nZONES = %s\n"
          "VERBOSE = %s\nQUIET = %s\nPORT = %s\n",
          arguments.args[0], arguments.args[1], arguments.args[2],
          arguments.smooth ? "yes" : "no",
          arguments.breathe ? "yes" : "no",
          arguments.seconds,          
          arguments.zones,
          arguments.verbose ? "yes" : "no",
          arguments.quiet ? "yes" : "no",
          arguments.devport);

  /* Additional checking of the arguments */

  /* Check gradient seconds bounds */
  // double grad_seconds = atof(arguments.grad_seconds);
  // printf ("\nInput gradient seconds %s parsed as %f", arguments.grad_seconds, grad_seconds);

  // if (grad_seconds < 0){  //check for negative seconds
  //   printf ("\n%f gradient seconds entered.\n"
  //           "Seconds must not be negative.\n", grad_seconds);
  //   exit(0);
  // }

  // if (grad_seconds > 120){   //check for over 2 min
  //   printf ("\n%f gradient seconds entered.\n"
  //           "Seconds cannot be greater than 120.\n", grad_seconds);
  //   exit(0);
  // }

  // if ((grad_seconds == 0.0) && arguments.gradient){  //check for zero
  //    printf ("\nInvalid gradient seconds entered.\n");
  //    exit(0);
  // }

  // for (int i = 0; i < strlen(arguments.grad_seconds); i++){
  //   //not digit
  //   if ( !(isdigit(arguments.grad_seconds[i])) ){
  //       //not decimal point
  //       if (arguments.grad_seconds[i] != 46){
  //         //invalid input for gradient length
  //         printf ("\n%s gradient seconds entered.\n"
  //                 "Invalid gradient seconds entered.\n", arguments.grad_seconds);
  //         exit(0);
  //       }

  //   }
  // }

  /* Check RGB color input from user*/

  char *endptr;
  unsigned int rgb_red = (unsigned int)strtol(arguments.args[0], &endptr, 10);
  unsigned int rgb_green = (unsigned int)strtol(arguments.args[1], &endptr, 10);
  unsigned int rgb_blue = (unsigned int)strtol(arguments.args[2], &endptr, 10);

  printf ("\nInput red rgb color    %s parsed as %i", arguments.args[0], rgb_red );
  printf ("\nInput green rgb color  %s parsed as %i", arguments.args[1], rgb_green );
  printf ("\nInput blue rgb color   %s parsed as %i\n", arguments.args[2], rgb_blue );

  if ((rgb_red > 255) || (rgb_red < 0)) {
    printf ("\n%s red rgb color entered.\n"
              "Color must be between 0 and 255.\n", arguments.args[0]);
    exit(0);     
  }

  if ((rgb_green > 255) || (rgb_green < 0)) {
    printf ("\n%s green rgb color entered.\n"
              "Color must be between 0 and 255.\n", arguments.args[1]);
    exit(0);     
  }

  if ((rgb_blue > 255) || (rgb_blue < 0)) {
    printf ("\n%s blue rgb color entered.\n"
              "Color must be between 0 and 255.\n", arguments.args[2]);
    exit(0);     
  }

  for (int i = 0; i < strlen(arguments.args[0]); i++){
    //not digit
    if ( !(isdigit(arguments.args[0][i])) ){
      //invalid input for red
      printf ("\n%s red color entered.\n"
              "Invalid red color entered.\n", arguments.args[0]);
      exit(0);
    }
  }

  for (int i = 0; i < strlen(arguments.args[1]); i++){
    //not digit
    if ( !(isdigit(arguments.args[1][i])) ){
      //invalid input for green
      printf ("\n%s green color entered.\n"
              "Invalid green color entered.\n", arguments.args[1]);
      exit(0);
    }
  }

  for (int i = 0; i < strlen(arguments.args[2]); i++){
    //not digit
    if ( !(isdigit(arguments.args[2][i])) ){
      //invalid input for blue
      printf ("\n%s blue color entered.\n"
              "Invalid blue color entered.\n", arguments.args[2]);
      exit(0);
    }
  }

  /* TODO Check for config file existence */

  /* TODO Parse config file */




  /* Set up serial communication - https://stackoverflow.com/questions/6947413 */
  char *portname = "/dev/ttyUSB0";   //connecting Arduino nano defaults here

  int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0){
          perror ("error opening port");
          exit (0);
  }

  set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  set_blocking (fd, 0);                    // set no blocking


  /* Serial communication testing */

  write (fd, "{}", 2);
  printf ("\nSent %s\nSleep 2", "{}");

  sleep(2); //don't just send data again right away, let the Arduino catch its breath


  char serialCmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  serialCmd[0] = '{';   //start
  serialCmd[1] = 1;     //zone: 1,2  
  serialCmd[2] = 1;     //mode1:  1
  serialCmd[3] = 'R';   //mode2
  serialCmd[4] = rgb_red;   //R
  serialCmd[5] = rgb_green; //G
  serialCmd[6] = rgb_blue;  //B
  serialCmd[7] = '}';       //end

  write (fd, serialCmd, 8);
  printf ("\nSent %s\n", serialCmd);
  printf ("\nStart byte: %c"
          "\nZone byte:  %i"
          "\nMode1 byte: %i"
          "\nMode2 byte: %i"
          "\nRed:        %i"
          "\nGreen:      %i"
          "\nBlue:       %i"
          "\nEnd byte:   %c\n",
           serialCmd[0], (int)serialCmd[1], (int)serialCmd[2], (int)serialCmd[3],
           (unsigned int)serialCmd[4], (int)serialCmd[5], (int)serialCmd[6], serialCmd[7]);
  // write (fd, "!234567}", 8);
  // printf ("\nSent '!234567}'");
  // sleep(4);

  exit (0);
}