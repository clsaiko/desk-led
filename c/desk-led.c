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

#include <stdlib.h>
#include <argp.h>
// #include <fcntl.h>
// #include <string.h>
// #include <termios.h>
// #include <unistd.h>
// #include <errno.h>

#include "desk-led.h"

// Using argp example code from http://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html

const char *argp_program_version =
  "desk-led 0.1";
const char *argp_program_bug_address =
  "<chris.saiko@gmail.com>";

/* Program documentation. */
static char doc[] =
  "desk-led -- a program to control WS2812 LED strips attached to an Arduino";

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 ARG2";

/* Argument parser. */
static struct argp argp = { 0, 0, 0, doc };


int main(int argc, char** argv) {

  argp_parse (&argp, argc, argv, 0, 0, 0);
  exit (0);

}