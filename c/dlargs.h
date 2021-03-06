/*  File name:    dlargs.h
 *  Description:  C code header file to provide declarations for program arguments.
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

// Using argp example code from http://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html


#ifndef DLARGS_H
#define DLARGS_H

#include <argp.h>

const char *argp_program_version =
  "desk-led 0.1";
const char *argp_program_bug_address =
  "<chris.saiko@gmail.com>";

/* Program documentation. - argp */
static char doc[] =
  "desk-led -- a program to control WS2812 LED strips attached to an Arduino.\n\n"
  " The arguments RED, GREEN, and BLUE are integers from 0 to 255, representing\n"
  " the component brightness of a color.\
  \vSource available at https://github.com/clsaiko/desk-led";

/* A description of the arguments we accept. - argp */
static char args_doc[] = "RED GREEN BLUE";

/* The options we understand. - argp */
static struct argp_option options[] = {
  {"verbose",  'v', 0,         0,  "Produce verbose output" },
  {"quiet",    'q', 0,         0,  "Don't produce any output" },
  {"cylon",    'c', 0,         0,  "Cylon animation effect. One zone only" },
  {"zones",    'z', "ZONES",   0,  "Zones to apply, comma separated (ex: 1,2,3)"},
  {"smooth",   's', "SECONDS", 0,  "Smooth transition in 1 to 120 SECONDS"},
  {"breathe",  'b', "SECONDS", 0,  "Breathe on and off, over 2 to 10 SECONDS"},
  {"port",     'p', "PORT",    0,  "PORT at which the Arduino may be accessed"},
  { 0 }
};

/* Used by main to communicate with parse_opt. - argp */
struct arguments
{
  char *args[3];        /* RED GREEN BLUE */
  int quiet, verbose;   // output
  int smooth, breathe, cylon;   // modes
  char *zones;
  char *seconds;
  char *devport;
};

/* Parse a single option. - argp */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {

  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'q':
      arguments->quiet = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'z':
      arguments->zones = arg;
      break;
    case 'c':
      arguments->cylon = 1;
      break;
    case 's':
      arguments->smooth = 1;
      arguments->seconds = arg;
      break;
    case 'p':
      arguments->devport = arg;
      break;
    case 'b':
      arguments->breathe = 1;
      arguments->seconds = arg;
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

/* Our argp parser. - argp */
static struct argp argp = { options, parse_opt, args_doc, doc };

#endif