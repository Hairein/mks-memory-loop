// simple_example.c
// Author: Micah Koleoso Software, 2021

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>

#include <SFML/Graphics.hpp>

extern "C" {
    #include "mksmemoryloop.h"
}

struct frame_shape_data {
    bool enabled;
    float position[2];
    sf::Color color;
};

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("USAGE ./sfml_example <cfg_filename>\n");
    }

    printf("SFML example using mksmemoryloop.\n");

    printf("Close the window to quit.\n\n");

    uint8_t result = mksml_initialize(argv[1]);
    if(result != MKS_NO_ERROR) { 
        return  EXIT_SUCCESS;
    }
    
    struct frame_shape_data frame[MKS_MAX_FEDERATES];

    uint8_t platform_index = mksml_get_platform_index();
    uint8_t nos_federates = mksml_get_nos_federates();

    char title[128];
    sprintf(title, "SFML mksmemoryloop example(%d)", platform_index);

    sf::RenderWindow window(sf::VideoMode(640, 480), title);

    sf::CircleShape shape[3];
    shape[0].setRadius(25.0f);
    shape[1].setRadius(25.0f);
    shape[2].setRadius(25.0f);
    
    switch(platform_index) {
        case 0: 
            frame[0].enabled = true;

            frame[0].position[0] = 160.0f;
            frame[0].position[1] = 240.0f;

            frame[0].color = sf::Color::Red;
            break;
        case 1: 
            frame[1].enabled = true;

            frame[1].position[0] = 320.0f;
            frame[1].position[1] = 240.0f;

            frame[1].color = sf::Color::Green;
            break;
        case 2: 
            frame[2].enabled = true;

            frame[2].position[0] = 480.0f;
            frame[2].position[1] = 240.0f;

            frame[2].color = sf::Color::Blue;
            break;
        default:
            break;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                continue;
            }
        }

        for(int index = 0; index < nos_federates; index++) {
            // Use the frame pointer begin_frame/end_frame duration for as short a time as possible
            // since this prevents network updates during this period. Only access 1 pair for a frame at a time
            uint8_t* frame_pointer = mksml_begin_frame(index);
            if(frame_pointer != NULL) {              
                struct frame_shape_data* struct_pointer = (struct frame_shape_data*)frame_pointer;

                if(index != platform_index) {
                    // Take other data from frame
                    memcpy(&frame[index], struct_pointer, sizeof(struct frame_shape_data));
                } else {
                    // Copy own data to distribute
                    memcpy(struct_pointer, &frame[index], sizeof(struct frame_shape_data));
                }

                mksml_end_frame();
            }
        }

        window.clear();

        for(int index = 0; index < nos_federates; index++) {
            if(frame[index].enabled) {
                shape[index].setPosition(frame[index].position[0], frame[index].position[1]);
                shape[index].setFillColor(frame[index].color);

                window.draw(shape[index]);
            }
        }

        window.display();
    }

    printf("\n");

    mksml_uninitialize();

    return EXIT_SUCCESS;
}

