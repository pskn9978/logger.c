#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <dirent.h>

//the function is returning the keyboard-event-symlink
char* getKbdEvent(){

	DIR *dp;
	struct dirent *ep;
	char path[64]="/dev/input/by-path/";
	char name[64];
	static char device[128];
	int fd;

	dp = opendir (path);
	if (dp != NULL)
	{
		while (ep = readdir (dp)){
			//the link that is containing kbd is for the keyboard-input
			if(strstr(ep->d_name, "kbd") != NULL){

				//the path and the name of the symlink are print to device
				sprintf(device, "%s%s", path, ep->d_name);

				//Open Device
				if ((fd = open (device, O_RDONLY)) == -1){
					printf ("%s is not a vaild device.\n", device);
				}
				//name of the device is selected
				ioctl (fd, EVIOCGNAME (sizeof (name)), name);

				//close file
				close(fd);

				printf("getKbdEvent() untersucht folgenden Namen: %s\n", name);

				(void) closedir (dp);
				return device;
			}
		}
	}
	else
		perror ("Couldn't open the directory");

	return NULL;
}

char getLetter(int code){

	if (code == KEY_1) return '1';
	if (code == KEY_2) return '2';
	if (code == KEY_3) return '3';
	if (code == KEY_4) return '4';
	if (code == KEY_5) return '5';
	if (code == KEY_6) return '6';
	if (code == KEY_7) return '7';
	if (code == KEY_8) return '8';
	if (code == KEY_9) return '9';
	if (code == KEY_0) return '0';
	if (code == KEY_Q) return 'q';
	if (code == KEY_W) return 'w';
	if (code == KEY_E) return 'e';
	if (code == KEY_R) return 'r';
	if (code == KEY_T) return 't';
	if (code == KEY_Y) return 'y';
	if (code == KEY_U) return 'u';
	if (code == KEY_I) return 'i';
	if (code == KEY_O) return 'o';
	if (code == KEY_P) return 'p';
	if (code == KEY_A) return 'a';
	if (code == KEY_S) return 's';
	if (code == KEY_D) return 'd';
	if (code == KEY_F) return 'f';
	if (code == KEY_G) return 'g';
	if (code == KEY_H) return 'h';
	if (code == KEY_J) return 'j';
	if (code == KEY_K) return 'k';
	if (code == KEY_L) return 'l';
	if (code == KEY_X) return 'x';
	if (code == KEY_C) return 'c';
	if (code == KEY_V) return 'v';
	if (code == KEY_B) return 'b';
	if (code == KEY_N) return 'n';
	if (code == KEY_M) return 'm';


	return '-';

}

int main (int argc, char *argv[])
{
	struct input_event ev;
	int fd, rd, value, size = sizeof (struct input_event);
	char name[256] = "Unknown";
	char *device = NULL;

	printf("Hello to the keyboardReader.\n");

	if ((getuid ()) != 0)
		printf ("You are not root! This may not work...\n");

	//keyboard-device ermitteln
	device=getKbdEvent();

	if(device==NULL){
		return -1;
	}

	//Open Device
	if ((fd = open (device, O_RDONLY)) == -1){
		printf ("%s is not a vaild device.\n", device);
		return -1;
	}

	//Print Device Name
	ioctl (fd, EVIOCGNAME (sizeof (name)), name);
	printf ("Reading From : %s (%s)\n", device, name);

	while (1){

		memset((void*) &ev, 0, sizeof(ev));

		if ((rd = read (fd, (void*) &ev, sizeof(ev))) < size)
			printf("Error, size to small\n");

		//type == 1 shows it is a EV_KEY (Key-Event) and value==0 shows its a keyReleaseEvents
		if(ev.type == 1 && ev.value == 0){

			printf("type: %d, code: %d, value: %d, rd: %d\n", ev.type, ev.code, ev.value, rd);
			
			printf("%c\n", getLetter(ev.code));

		}



	}

	return 0;
}