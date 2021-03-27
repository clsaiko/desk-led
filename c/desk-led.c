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
#include <string.h>

// #include <fcntl.h>
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
  "desk-led -- a program to control WS2812 LED strips attached to an Arduino.\n\n"
  "            The arguments RED, GREEN, and BLUE are integers from 0 to 255,\n"
  "            representing the component brightness of a color.";

/* A description of the arguments we accept. */
static char args_doc[] = "RED GREEN BLUE";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,         0,  "Produce verbose output" },
  {"quiet",    'q', 0,         0,  "Don't produce any output" },
  {"silent",   's', 0,         OPTION_ALIAS },
  {"gradient", 'g', "SECONDS", 0,  "Change color in a gradient over number of SECONDS" },
  {"output",   'o', "FILE",    0,  "Output to FILE instead of standard output" },
  {"config",   'c', "FILE",    0,  "Use the supplied FILE to send instructions" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[2];                /* arg1 & arg2 */
  int silent, verbose,gradient;
  char *grad_seconds;
  char *output_file;
  char *config_file;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'c':
      arguments->config_file = arg;
      break;
    case 'g':
      arguments->gradient = 1;
      arguments->grad_seconds = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 3)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 3)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


int main(int argc, char** argv) {

   struct arguments arguments;
   
   /* Default values. */
   arguments.silent = 0;
   arguments.verbose = 0;
   arguments.gradient = 0;
   arguments.grad_seconds = "0";
   arguments.output_file = "-";
   arguments.config_file = "-";

   /* Parse our arguments; every option seen by parse_opt will be reflected in arguments. */
   argp_parse (&argp, argc, argv, 0, 0, &arguments);

   printf ("RED = %s\nGREEN = %s\nBLUE = %s\n"
           "OUTPUT_FILE = %s\nCONFIG_FILE = %s\nGRADIENT = %s\n"
           "GRADIENT = %s seconds\nVERBOSE = %s\nSILENT = %s\n",
           arguments.args[0], arguments.args[1], arguments.args[2],
           arguments.output_file,
           arguments.config_file,
           arguments.gradient ? "yes" : "no",
           arguments.grad_seconds,
           arguments.verbose ? "yes" : "no",
           arguments.silent ? "yes" : "no");

  exit (0);

}