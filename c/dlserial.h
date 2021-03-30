/*  File name:    dlserial.h
 *  Description:  C code header file to provide declarations for serial communications with an Arduino.
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

#ifndef DLSERIAL_H
#define DLSERIAL_H

#include <termios.h>

/* Serial communication - https://stackoverflow.com/questions/6947413 */
int set_interface_attribs (int fd, int speed, int parity);

/* Serial communication - https://stackoverflow.com/questions/6947413 */
void set_blocking (int fd, int should_block);

#endif