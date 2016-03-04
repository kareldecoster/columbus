#include <roomba_control.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int done = 0;
	roomba_t* roomba = roomba_create();
	if(roomba == NULL){
		return EXIT_FAILURE;
	}
	roomba_start(roomba);
	roomba_control(roomba);
	roomba_full_mode(roomba);
	roomba_save_song_jingle_bells(roomba);
	char cmd;
	while(!done){
		printf("Please enter a command:\n");
		scanf("%c",&cmd);
		if(cmd == 'q'){
			done = 1;
		}if(cmd == 'f'){
			roomba_forward(roomba);
		}if(cmd == 'c'){
			roomba_clean(roomba);
		}if(cmd == 'i'){
			roomba_save_song_imperial(roomba);
			roomba_play_song(roomba);
		}if(cmd == 'l'){
			roomba_turn_ccw(roomba);
		}if(cmd =='r'){
			roomba_turn_cw(roomba);
		}if(cmd == 'x'){
			roomba_full_mode(roomba);
			roomba_stop(roomba);
		}if(cmd == 'm'){
			roomba_safe_mode(roomba);
		}if(cmd == 'b'){
			roomba_reverse(roomba);
		}if(cmd == 'w'){
			roomba_drive_distance(roomba);
		}if(cmd == 'j'){
			roomba_save_song_jingle_bells(roomba);
			roomba_play_song(roomba);
		}
	}
	roomba_start(roomba);
	roomba_destroy(&roomba);
	return EXIT_SUCCESS;
}
