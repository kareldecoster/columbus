#ifndef ROOMBA_CONTROL_H
#define ROOMBA_CONTROL_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <math.h>

#define BAUD_RATE B115200	//Default Baud rate for Roomba 620 is 115200
#define SERIAL_INTERFACE "/dev/ttyO1"	//This is the BB-UART1 on the Beaglebone Black
#define ROOMBA_WIDTH	234	//in mm

typedef struct roomba roomba_t;

typedef enum {
	B_LEFT,
	B_RIGHT,
	B_BOTH,
	B_NONE
}bumpstate_t;

typedef enum{
	C_NO_CLIFF,
	C_RIGHT,
	C_LEFT,
	C_FRONT
}cliffstate_t;

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

double roomba_turn_degree_cw(roomba_t* roomba, int16_t speed, double angle);

double roomba_turn_degree_ccw(roomba_t* roomba, int16_t speed, double angle);

double roomba_get_battery(roomba_t* roomba);

int roomba_get_wall(roomba_t* roomba);

bumpstate_t roomba_get_bumpstate(roomba_t* roomba);

cliffstate_t roomba_get_cliffstate(roomba_t* roomba);

int roomba_cover_and_dock(roomba_t* roomba);

#endif
