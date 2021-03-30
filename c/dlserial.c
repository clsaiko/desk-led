/*  File name:    dl-serial.c
 *  Description:  C code file to control serial communications with an Arduino.
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

#include "dlserial.h"

#include <string.h>
#include <stdio.h>

/* Serial communication - https://stackoverflow.com/questions/6947413 */
int set_interface_attribs (int fd, int speed, int parity) {

        struct termios tty;
        if (tcgetattr (fd, &tty) != 0)
        {
                perror("error from tcgetattr");
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
        
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        /* https://forum.arduino.cc/index.php?topic=28167.msg208341#msg208341 */
        tty.c_cflag &= ~HUPCL;          // disable hang-up-on-close to avoid reset

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
        tty.c_cflag |= (CLOCAL | CREAD);        // ignore modem controls, enable reading
        
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;        
        tty.c_cflag &= ~CSTOPB;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                perror("error from tcsetattr");
                return -1;
        }
        return 0;
}

/* Serial communication - https://stackoverflow.com/questions/6947413
   Will probably never really use this to set blocking since we never read from the Arduino,
   but the StackOverflow answer has it. */

void set_blocking (int fd, int should_block) {

        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                perror("error from tcgetattr");
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                perror("error from tcsetattr");
}