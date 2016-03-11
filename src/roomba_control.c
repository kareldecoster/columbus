/*
 *	Author: Karel De Coster - k.decoster94@gmail.com
 *	Date:	2016-02-29
 *	Description:	roomba_control.c and roomba_control.h is a simple interface to communicate with Roomba (620) over a serial port. 
 *	It was designed to work on the TI,Beaglebone Black. Parameters such as BAUD_RATE are hard coded and can be found as macros in roomba_control.h.
 *	Functions bytes_to_int and int_to_bytes takes care of issues caused by endians. Change this function if transporting to another platform.
 */
 
 #include <roomba_control.h>
 
struct roomba{
 	int fd;
};

typedef union {
	int16_t value;
	char bytes[2];
}combo_t;
 
int16_t bytes_to_int(char high, char low){
	combo_t data;
	data.bytes[0]=low;
	data.bytes[1]=high;
	return data.value;
} 

char get_high_byte(int16_t number){
	combo_t data;
	data.value = number;
	return data.bytes[1];
}

char get_low_byte(int16_t number){
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
	int i = close((*roomba)->fd);
	free(*roomba);
	*roomba = NULL;
	return i;
}

int roomba_start(roomba_t* roomba){
	char msg = 0x80 ;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_control(roomba_t* roomba){
	char msg = 0x82;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_safe_mode(roomba_t* roomba){
	char msg = 0x83;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_full_mode(roomba_t* roomba){
	char msg = 0x84;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_drive(roomba_t* roomba, int16_t speed){
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
	int i = roomba_drive(roomba, speed);
	return i;
}

int roomba_reverse(roomba_t* roomba, int16_t speed){
	int i = roomba_forward(roomba, -speed);
	return i;
}

int roomba_clean(roomba_t* roomba){
	char msg = 0x88;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

int roomba_save_song_jingle_bells(roomba_t* roomba){
	char msg[25] = {0x8c, 0x00, 0x0B, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4c, 0x10, 0x4c, 0x20, 0x4c, 0x10, 0x4f, 0x10, 0x48, 0x18, 0x4A, 0x08, 0x4c, 0x40};
	int i = write(roomba->fd, msg, 25);
	return i;
}

int roomba_save_song_imperial(roomba_t* roomba){
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
	char msg[2] = {0x8d, number};
	int i = write(roomba->fd, msg, 2);
	return i;
}

int roomba_turn_cw(roomba_t* roomba){
	char msg[5]={0x89, 0, 0x64, 0xff, 0xff};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_turn_ccw(roomba_t* roomba){
	char msg[5]={0x89, 0, 0x64, 0x00, 0x01};
	int i = write(roomba->fd, msg, 5);
	//char msg[16] = {0x98, 0x0D , 0x89, 0x00, 0x64, 0x00, 0x01,  0x9D, 0x00, 0x5A, 0x89, 0x00, 0x00, 0x80, 0x00, '\n'}; SCRIPT CODE
	//int i = write(roomba->fd, msg, 16);
	//i += roomba_play_script(roomba);
	return i;
}

int roomba_stop(roomba_t* roomba){
	char msg[5]={0x89, 0x00, 0x00, 0x80, 0x00};
	int i = write(roomba->fd, msg, 5);
	return i;
}

int roomba_drive_distance(roomba_t* roomba, int mm){
	//char msg[16]={0x98, 0x0D, 0x89, 0x01, 0x2c, 0x80, 0x00, 0x9B, 0x0A, 0x89, 0x00, 0x00, 0x80, 0x00, '\n'};	//10 cm
	//int i = write(roomba->fd, msg, 16);	
	//i += roomba_play_script(roomba);
	int i = roomba_forward(roomba, 100);
	usleep(mm*100000);
	i += roomba_stop(roomba);
	return i;
}

int roomba_play_script(roomba_t* roomba){
	char msg = 0x99;
	int i = write(roomba->fd, &msg, 1);
	return i;
}

double roomba_request_angle(roomba_t* roomba){
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
	double angle = (distance_difference * 360 )/ ( M_PI * 234 ); /* formula from roomba datasheet, angle in degrees */
	return angle;
}


/*	Returns the distance in cm as a signed 16 bit integer. A positive integer indicates distance travelled forward.
	A negative distance means the roomba drove backwards*/
int16_t roomba_request_distance(roomba_t* roomba){
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
