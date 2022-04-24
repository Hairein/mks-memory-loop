// simple_example.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>

#include "mksmemoryloop.h"

int kbhit(void);

int main(int argc, char *argv[]) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    getcwd(buffer, 1024);

    if(argc != 2) {
        printf("USAGE ./simple_example <cfg_filename>\n");
    }

    printf("Simple example using mksmemoryloop.\n");
    //printf("CWD:%s\n", buffer);

    uint8_t result = mksml_initialize(argv[1]);

    if(result == MKS_NO_ERROR) { 
        printf("Hit a key to quit.\n\n");

        while(kbhit() == 0) {
            uint8_t platform_index = mksml_get_platform_index();
            uint8_t nos_federates = mksml_get_nos_federates();

            for(uint8_t index = 0; index < nos_federates; index++) {
                uint16_t counter = 0;

                uint8_t* frame_pointer = mksml_begin_frame(index);
                
                if(frame_pointer != NULL) {              
                    counter = *((uint16_t*)frame_pointer);

                    if(index == platform_index) {
                        *((uint16_t*)frame_pointer) = *((uint16_t*)frame_pointer) + 1;
                        if(*((uint16_t*)frame_pointer) >= 32000) {
                            *((uint16_t*)frame_pointer) = 0;
                        }
                    }

                    mksml_end_frame();
                }

                // write the counter
                printf("%5d(%5d) ", index, counter);
            }


            printf("\r");
        }

        printf("\n");

        mksml_uninitialize();
    }

    return EXIT_SUCCESS;
}

// kbhit function by Shashwat Kumar
// from here: https://stackoverflow.com/questions/22166074/is-there-a-way-to-detect-if-a-key-has-been-pressed
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}