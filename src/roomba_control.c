/*
 *	Author: Karel De Coster - k.decoster94@gmail.com
 *	Date:	2016-02-29
 *	Description:	roomba_control.c and roomba_control.h is a simple interface to communicate with Roomba (620) over a serial port. 
 *	It was designed to work on the TI,Beaglebone Black. Parameters such as BAUD_RATE are hard coded and can be found as macros in roomba_control.h.
 *	Functions bytes_to_int, get_high_byte and get_low_byte take care of issues caused by endians. Change this function if transporting to another platform.
 */
 
 #include <roomba_control.h>
 
struct roomba{
 	int fd;
};

typedef union {
	int16_t value;
	char bytes[2];
}combo_t;
 
int16_t bytes_to_int(char high, char low){	// BIG ENDIAN
	combo_t data;
	data.bytes[0]=low;
	data.bytes[1]=high;
	return data.value;
} 

char get_high_byte(int16_t number){			// BIG ENDIAN
	combo_t data;
	data.value = number;
	return data.bytes[1];
}

char get_low_byte(int16_t number){			// BIG ENDIAN
	combo_t data;
	data.value = number;
	return data.bytes[0];
}

roomba_t* roomba_create(){
	roomba_t* roomba = malloc(sizeof(roomba_t));
	if(roomba == NULL){
		return NULL;
	}
	char port[20] = SERIAL_INTERFACE; /* port to connect to */
	speed_t baud = BAUD_RATE; /* baud rate */
	roomba->fd = open(port, O_RDWR | O_NOCTTY); /* connect to port */

	/* set the other settings (in this case, 115200 8N1) */
	struct termios settings;
	tcgetattr(roomba->fd, &settings);

	cfsetospeed(&settings, baud); /* baud rate */
	cfsetispeed(&settings, baud); /* baud rate */
	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = 0; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */
	
	settings.c_cflag |= CREAD;
	settings.c_cc[VTIME] = 0;	/* inter-character timer unused */
	settings.c_cc[VMIN] = 1;	/* blocking read until 1 chars received */

	tcsetattr(roomba->fd, TCSANOW, &settings); /* apply the settings */
	tcflush(roomba->fd, TCOFLUSH);
	
	return roomba;
}

int roomba_destroy(roomba_t** roomba){
	if(roomba == NULL || *roomba == NULL){
		return -1;
	}
	int i = close((*roomba)->fd);
	free(*roomba);
	*roomba = NULL;
	return i;
}

int roomba_start(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x80 ;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_control(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x82;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_safe_mode(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x83;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_full_mode(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x84;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_drive(roomba_t* roomba, int16_t speed){
	if(roomba == NULL){
		return -1;
	}
	if(speed>500){
		speed = 500;
	}
	if(speed<-500){
		speed = -500;
	}
	char msg[5]={0x91, get_high_byte(speed), get_low_byte(speed), get_high_byte(speed), get_low_byte(speed)};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_forward(roomba_t* roomba, int16_t speed){
	if(roomba == NULL){
		return -1;
	}
	int i = roomba_drive(roomba, speed);
	return i;
}

int roomba_reverse(roomba_t* roomba, int16_t speed){
	if(roomba == NULL){
		return -1;
	}
	int i = roomba_forward(roomba, -speed);
	return i;
}

int roomba_clean(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x88;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_save_song_jingle_bells(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[25] = {0x8c, 0x00, 0x0B, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4f, 0x10, 0x48, 0x18, 0x4A, 0x08, 0x4c, 0x40};
	int i = write(roomba->fd, msg, 25);
	return i;
}

int roomba_save_song_imperial(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg1[21] = {0x8c, 0x00, 0x09, 0x45, 0x20, 0x45, 0x20, 0x45, 0x20, 0x41, 0x18, 0x48, 0x08, 0x45, 0x20, 0x41, 0x18, 0x48, 0x08, 0x45, 0x40};
	int i = write(roomba->fd, msg1, 21);
	char msg2[21] = {0x8c, 0x01, 0x09, 0x4c, 0x20, 0x4c, 0x20, 0x4c, 0x20, 0x4d, 0x18, 0x48, 0x08, 0x44, 0x20, 0x41, 0x18, 0x48, 0x08, 0x45, 0x40};
	i += write(roomba->fd, msg2, 21);
	char msg3[21] = {0x8c, 0x02, 0x09, 0x51, 0x20, 0x45, 0x18, 0x45, 0x08, 0x51, 0x20, 0x50, 0x18, 0x4F, 0x08, 0x4E, 0x08, 0x4D, 0x08, 0x4E, 0x10};
	i += write(roomba->fd, msg3, 21);
	char msg4[35] = {0x8c, 0x03, 0x10, 0x46, 0x10, 0x4b, 0x20, 0x4a, 0x18, 0x49, 0x08, 0x49, 0x08, 0x48, 0x08, 0x49, 0x10, 0x80, 0x10, 0x41, 0x10, 0x44, 0x20, 0x41, 0x18, 0x44, 0x08, 0x48, 0x20, 0x43, 0x18, 0x48, 0x08, 0x4c, 0x20};
	i += write(roomba->fd, msg4, 35);
	return i;
}

int roomba_play_song(roomba_t* roomba, char number){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8d, number};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_turn_cw(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[5]={0x89, 0, 0x64, 0xff, 0xff};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_turn_ccw(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[5]={0x89, 0, 0x64, 0x00, 0x01};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_stop(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[5]={0x89, 0x00, 0x00, 0x80, 0x00};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_drive_distance(roomba_t* roomba, int mm){
	if(roomba == NULL){
		return -1;
	}
	int i = roomba_forward(roomba, 100);
	usleep(mm*100000);
	i += roomba_stop(roomba);
	return i;
}

int roomba_play_script(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x99;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

double roomba_request_angle(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x02};
	write(roomba->fd, msg, 2);
	char data[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	int index = 0;
	int size = 0;
	while(index < 6){
		size = read(roomba->fd, &data[index], 6-index);
		if(size > 0){
			index += size;
		}
	}
	int16_t distance_difference = bytes_to_int(data[4], data[5]);
	double angle = distance_difference* 360 * 2 / (ROOMBA_WIDTH) ;
	return angle;
}


/*	Returns the distance in cm as a signed 16 bit integer. A positive integer indicates distance travelled forward.
	A negative distance means the roomba drove backwards*/
int16_t roomba_request_distance(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x02};
	write(roomba->fd, msg, 2);
	char data[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	int index = 0;
	int size = 0;
	while(index < 6){
		size = read(roomba->fd, &data[index], 6-index);
		if(size > 0){
			index += size;
		}
	}
	int16_t distance = bytes_to_int(data[2], data[3]);
	return -distance;
}


double roomba_turn_degree_cw(roomba_t* roomba, int16_t speed, double angle){
	if(roomba == NULL){
		return -1;
	}
	roomba_request_angle(roomba);
	char high, low;
	high = get_high_byte(speed);
	low = get_low_byte(speed);
	char msg[5]={0x89, high, low, 0xff, 0xff};
	write(roomba->fd, msg, 5);
	double time = 1000000*(ROOMBA_WIDTH/2)*M_PI*angle/(100*180);
	usleep(time);
	roomba_stop(roomba);
	
	double actually_turned = roomba_request_angle(roomba);
	
	return actually_turned;
}

double roomba_turn_degree_ccw(roomba_t* roomba, int16_t speed, double angle){
	if(roomba == NULL){
		return -1;
	}
	roomba_request_angle(roomba);
	char high, low;
	high = get_high_byte(speed);
	low = get_low_byte(speed);
	char msg[5]={0x89, high, low, 0x00, 0x01};
	write(roomba->fd, msg, 5);
	double time = 1000000*(ROOMBA_WIDTH/2)*M_PI*angle/(100*180);
	usleep(time);
	roomba_stop(roomba);
	
	double actually_turned = roomba_request_angle(roomba);
	
	return actually_turned;
}

double roomba_get_battery(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x03};
	write(roomba->fd, msg, 2);
	char data[10];
	int index = 0;
	int size = 0;
	while(index < 10){
		size = read(roomba->fd, &data[index], 10-index);
		if(size > 0){
			index += size;
		}
	}
	uint16_t capacity = bytes_to_int(data[8], data[9]);
	uint16_t charge  = bytes_to_int(data[6], data[7]);
	return 100.00 * charge / capacity;
}

int roomba_get_wall(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x01};
	write(roomba->fd, msg, 2);
	char data[10];
	int index = 0;
	int size = 0;
	while(index < 10){
		size = read(roomba->fd, &data[index], 10-index);
		if(size > 0){
			index += size;
		}
	}
	int wall = data[1];
	return wall;
}

bumpstate_t roomba_get_bumpstate(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x01};
	write(roomba->fd, msg, 2);
	char data[10];
	int index = 0;
	int size = 0;
	while(index < 10){
		size = read(roomba->fd, &data[index], 10-index);
		if(size > 0){
			index += size;
		}
	}
	char byte = data[0];
	byte = byte%4;
	if(byte == 3){
		return B_BOTH;
	}if(byte == 2){
		return B_LEFT;
	}if(byte == 1){
		return B_RIGHT;
	}
	return B_NONE;
}

cliffstate_t roomba_get_cliffstate(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg[2] = {0x8E, 0x01};
	write(roomba->fd, msg, 2);
	char data[10];
	int index = 0;
	int size = 0;
	while(index < 10){
		size = read(roomba->fd, &data[index], 10-index);
		if(size > 0){
			index += size;
		}
	}
	if((data[2]==1 || data[3]==1)&&(data[4]==1 || data[5]==1)){
		return C_FRONT;
	}if(data[2]==1 || data[3]==1){
		return C_LEFT;
	}if(data[4]==1 || data[5]==1){
		return C_RIGHT;
	}
	return C_NO_CLIFF;
}

int roomba_cover_and_dock(roomba_t* roomba){
	if(roomba == NULL){
		return -1;
	}
	char msg = 0x8F;
	return write(roomba->fd, &msg, 1);
}
