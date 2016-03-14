#include <roomba_control.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
	int done = 0;
	roomba_t* roomba = roomba_create();
	if(roomba == NULL){
		return EXIT_FAILURE;
	}
	roomba_start(roomba);
	roomba_control(roomba);
	roomba_full_mode(roomba);
	roomba_save_song_imperial(roomba);
	char cmd;
	double percent = roomba_get_battery(roomba);
	printf("Battery  --    %lf	%% \n", percent);
	while(!done){
		printf("Please enter a command:\n");
		scanf("%c",&cmd);
		if(cmd == 'q'){
			done = 1;
		}if(cmd == 'f'){
			roomba_forward(roomba, 200);
		}if(cmd == 'c'){
			roomba_clean(roomba);
		}if(cmd == 'i'){
			roomba_play_song(roomba, 0x00);
			usleep(4050000);
			roomba_play_song(roomba,0x01);
			usleep(4050000);
			roomba_play_song(roomba,0x02);
			usleep(3000000);
			roomba_play_song(roomba,0x03);
		}if(cmd == 'l'){
			double angle = roomba_turn_degree_ccw(roomba,100, 45.0);
			printf("angle = %lf\n", angle);
		}if(cmd =='r'){
			double angle = roomba_turn_degree_cw(roomba, 100, 45.0);
			printf("angle = %lf\n", angle);
		}if(cmd == 'x'){
			roomba_full_mode(roomba);
			roomba_stop(roomba);
		}if(cmd == 'm'){
			roomba_safe_mode(roomba);
		}if(cmd == 'b'){
			roomba_reverse(roomba, 50);
		}if(cmd == 'w'){
			int wall = roomba_get_wall(roomba);
			wall == 1 ? printf("Wall Detected.\n"): printf("No Wall Detected.\n");
		}if(cmd =='d'){
			int16_t distance = roomba_request_distance(roomba);
			printf("Distance = %d\n",distance);
		}if(cmd  == 'k'){
			bumpstate_t bump = roomba_get_bumpstate(roomba);
			switch(bump){
				case B_LEFT:
					printf("LEFT!\n");
					break;
				
				case B_RIGHT:
					printf("RIGHT!\n");
					break;
					
				case B_BOTH:
					printf("BOTH!\n");
					break;
					
				case B_NONE:
					printf("NONE!\n");
					break;
			}
		}if(cmd == 'g'){
			roomba_forward(roomba, 100);
			printf("Driving...\n");
			while(roomba_get_bumpstate(roomba) == B_NONE){
				usleep(1000);
			}
			printf("woepsie daysie! kben gebotst!\n");
			roomba_stop(roomba);
		}if(cmd == 'a'){
			cliffstate_t cliff = roomba_get_cliffstate(roomba);
			switch(cliff){
				case C_LEFT:
					printf("LEFT!\n");
					break;
				
				case C_RIGHT:
					printf("RIGHT!\n");
					break;
					
				case C_FRONT:
					printf("FRONT!\n");
					break;
					
				case C_NO_CLIFF:
					printf("NO CLIFF!\n");
					break;
			}
		}if(cmd == 'h'){
			roomba_cover_and_dock(roomba);
		}
	}
	roomba_start(roomba);
	roomba_destroy(&roomba);
	return EXIT_SUCCESS;
}
