/*
 *	Author: Karel De Coster - k.decoster94@gmail.com
 *	Date:	2016-02-29
 *	Description:	roomba_control.c and roomba_control.h is a simple interface to communicate with Roomba (620) over a serial port. 
 *	It was designed to work on the TI,Beaglebone Black. Parameters such as BAUD_RATE are hard coded and can be found as macros in roomba_control.h.
 */
 
 #include <roomba_control.h>
 
struct roomba{
 	int fd;
};
 
roomba_t* roomba_create(){
	roomba_t* roomba = malloc(sizeof(roomba_t));
	if(roomba == NULL){
		return NULL;
	}
	char port[20] = SERIAL_INTERFACE; /* port to connect to */
	speed_t baud = BAUD_RATE; /* baud rate */
	roomba->fd = open(port, O_RDWR); /* connect to port */

	/* set the other settings (in this case, 115200 8N1) */
	struct termios settings;
	tcgetattr(roomba->fd, &settings);

	cfsetospeed(&settings, baud); /* baud rate */
	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = ICANON; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */

	tcsetattr(roomba->fd, TCSANOW, &settings); /* apply the settings */
	tcflush(roomba->fd, TCOFLUSH);
	
	return roomba;
}

int roomba_destroy(roomba_t** roomba){
	int i = close((*roomba)->fd);
	free(*roomba);
	*roomba = NULL;
	return i;
}

int roomba_start(roomba_t* roomba){
	char msg[2]={0x80, '\n'};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_control(roomba_t* roomba){
	char msg[2]={0x82, '\n'};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_safe_mode(roomba_t* roomba){
	char msg[2]={0x83, '\n'};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_full_mode(roomba_t* roomba){
	char msg[2]={0x84, '\n'};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_forward(roomba_t* roomba){
	//char msg[6]={0x89, 0, 0x64, 0x80, 0, '\n'};
	char msg[6]={0x91, 0x00, 0x64, 0x00, 0x64, '\n'};
	int i = write(roomba->fd, msg,6);
	return i;
}

int roomba_reverse(roomba_t* roomba){
	char msg[6]={0x89, 0xff, 0x9c, 0x80, 0x00, '\n'};
	int i = write(roomba->fd, msg, 6);
	return i;
}

int roomba_clean(roomba_t* roomba){
	char msg[2]={0x88, '\n'};
	int i = write(roomba->fd, msg,2);
	return i;
}

int roomba_save_song_jingle_bells(roomba_t* roomba){
	char msg[26] = {0x8c, 0x00, 0x0B, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4f, 0x10, 0x48, 0x18, 0x4A, 0x08, 0x4c, 0x40, '\n'};
	int i = write(roomba->fd, msg,26);
	return i;
}

int roomba_save_song_imperial(roomba_t* roomba){
	char msg[22] = {0x8c, 0x00, 0x09, 0x43, 0x20, 0x43, 0x20, 0x43, 0x20, 0x40, 0x18, 0x46, 0x08, 0x43, 0x20, 0x40, 0x18, 0x46, 0x08, 0x43, 0x40, '\n'};
	int i = write(roomba->fd, msg, 22);
	return i;
}

int roomba_play_song(roomba_t* roomba){
	char msg[3] = {0x8d, 0x00, '\n'};
	int i = write(roomba->fd, msg,3);
	return i;
}

int roomba_turn_cw(roomba_t* roomba){
	char msg[6]={0x89, 0, 0x64, 0xff, 0xff,'\n'};
	int i = write(roomba->fd, msg, 6);
	return i;
}

int roomba_turn_ccw(roomba_t* roomba){
	char msg[6]={0x89, 0, 0x64, 0x00, 0x01, '\n'};
	int i = write(roomba->fd, msg, 6);
	//char msg[16] = {0x98, 0x0D , 0x89, 0x00, 0x64, 0x00, 0x01,  0x9D, 0x00, 0x5A, 0x89, 0x00, 0x00, 0x80, 0x00, '\n'}; SCRIPT CODE
	//int i = write(roomba->fd, msg, 16);
	//i += roomba_play_script(roomba);
	return i;
}

int roomba_stop(roomba_t* roomba){
	char msg[6]={0x89, 0x00, 0x00, 0x80, 0x00, '\n'};
	int i = write(roomba->fd, msg, 6);
	return i;
}

int roomba_drive_distance(roomba_t* roomba){
	char msg[16]={0x98, 0x0D, 0x89, 0x01, 0x2c, 0x80, 0x00, 0x9B, 0x0A, 0x89, 0x00, 0x00, 0x80, 0x00, '\n'};	//10 cm
	int i = write(roomba->fd, msg, 16);
	i += roomba_play_script(roomba);
	return i;
}

int roomba_play_script(roomba_t* roomba){
	char msg[2] = {0x99, '\n'};
	int i = write(roomba->fd, msg,2);
	return i;
}
