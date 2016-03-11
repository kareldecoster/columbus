#ifndef ROOMBA_CONTROL_H
#define ROOMBA_CONTROL_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>

#define BAUD_RATE B115200	//Default Baud rate for Roomba 620 is 115200
#define SERIAL_INTERFACE "/dev/ttyO1"	//This is the BB-UART1 on the Beaglebone Black

typedef struct roomba roomba_t;

roomba_t* roomba_create();

int roomba_destroy(roomba_t** roomba);

int roomba_start(roomba_t* roomba);

int roomba_control(roomba_t* roomba);

int roomba_safe_mode(roomba_t* roomba);

int roomba_full_mode(roomba_t* roomba);

int roomba_forward(roomba_t* roomba, int16_t speed);

int roomba_reverse(roomba_t* roomba, int16_t speed);

int roomba_clean(roomba_t* roomba);

int roomba_save_song_jingle_bells(roomba_t* roomba);

int roomba_save_song_imperial(roomba_t* roomba);

int roomba_play_song(roomba_t* roomba, char number);

int roomba_turn_cw(roomba_t* roomba);

int roomba_turn_ccw(roomba_t* roomba);

int roomba_stop(roomba_t* roomba);

int roomba_drive_distance(roomba_t* roomba, int mm);

int roomba_play_script(roomba_t* roomba);

double roomba_request_angle(roomba_t* roomba);

int16_t roomba_request_distance(roomba_t* roomba);

#endif
