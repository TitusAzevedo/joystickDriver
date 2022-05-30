#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <joystick.h>
#include <string.h>

//keymap reihenfolge: 
//0-3 joystick up/right/down/left
//4-7 button 1-4
//8-11 menu button -> nur für js0

int js0_keymap[12] = {25,40,39,38,24,26,29,53,9,36,23,64};
int js1_keymap[8] = {28,43,42,41,27,52,54,55};
int js2_keymap[8] = {31,46,45,44,30,32,57,58};
int js3_keymap[8] = {14,15,16,17,10,11,12,13};

int open_joystick(char *device_name){
    int joystick_fd = -1;

    if(device_name == NULL) return joystick_fd;

    joystick_fd = open(device_name, O_RDONLY | O_NONBLOCK);

    if(joystick_fd < 0) {
        printf("Could not locate joystick %s\n", device_name);
        return joystick_fd;
    }
    return joystick_fd;
}


int read_event(int joystick_fd, struct js_event *jse){
    int bytes;

    bytes = read(joystick_fd, jse, sizeof(*jse));

    if(bytes == -1) return 0;
    if (bytes == sizeof(*jse)) return 1;

    printf("Unexpected bytes from joystick:%d\n", bytes);

    return -1;
}

//for dev not perma
int sendKey(){
    Display *display;
    unsigned int keycode;
    display = XOpenDisplay(NULL);
    keycode = XKeysymToKeycode(display, XK_A);
    XTestFakeKeyEvent(display, keycode,True,0);
    XTestFakeKeyEvent(display, keycode,False,0);
    XFlush(display);
    return keycode;
}

int joystickEvent(int axis, int value, Display *display, int *keymap){

    switch(axis){
        //up down
        case 1: 

            if(value > 0){
                XTestFakeKeyEvent(display, keymap[0],True,0);
            }
            else if(value < 0){
                XTestFakeKeyEvent(display, keymap[2],True,0);
            }
            else{
                XTestFakeKeyEvent(display, keymap[0],False,0);
                XTestFakeKeyEvent(display, keymap[2],False,0);
            }
            break;
        //lef right
        case 0: 

            if(value >0){
                XTestFakeKeyEvent(display, keymap[1],True,0);
            }
            else if(value < 0){
                XTestFakeKeyEvent(display, keymap[3],True,0);
            }
            else{
                XTestFakeKeyEvent(display, keymap[1],False,0);
                XTestFakeKeyEvent(display, keymap[3],False,0);
            }
            break;
    }
    XFlush(display);
}

int buttonEvent(int button_id,  int value, Display *display, int *keymap){

    switch(button_id){
        case 1: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[4],True,0);}
            else { XTestFakeKeyEvent(display, keymap[4],False,0);}
            break;
        case 2: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[5],True,0);}
            else { XTestFakeKeyEvent(display, keymap[5],False,0);}
            break;
        case 3: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[6],True,0);}
            else { XTestFakeKeyEvent(display, keymap[6],False,0);}
            break;
        case 4: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[7],True,0);}
            else { XTestFakeKeyEvent(display, keymap[7],False,0);}
            break;
        case 5: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[8],True,0);}
            else { XTestFakeKeyEvent(display, keymap[8],False,0);}
            break;
        case 6: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[9],True,0);}
            else { XTestFakeKeyEvent(display, keymap[9],False,0);}
            break;
        case 7: 
            if(value == 1){ XTestFakeKeyEvent(display, keymap[10],True,0);}
            else { XTestFakeKeyEvent(display, keymap[10],False,0);}
            break;
        case 8: 
            // key code 113 = left arrow numpad
            if(value == 1){ 
                XTestFakeKeyEvent(display, keymap[11],True,0);
                XTestFakeKeyEvent(display, 113,True,0);
                }
            else {
                XTestFakeKeyEvent(display, keymap[11],False,0);
                XTestFakeKeyEvent(display, 113,False,0);
                }
            break;
    }
    XFlush(display);
    
}

int* getKeymap(char path[]){
    
    if(strcmp("/dev/input/js0", path) == 0){
        return js0_keymap;
    }
    if(strcmp("/dev/input/js1", path) == 0){
        return js1_keymap;
    }
    if(strcmp("/dev/input/js2", path) == 0){
        return js2_keymap;
    }
    if(strcmp("/dev/input/js3", path) == 0){
        return js3_keymap;
    }
}


int main(int argc, char *argv[]){

    int fd,result;
    struct js_event jse;
    fd = open_joystick(argv[1]);

    Display *display;
    display = XOpenDisplay(NULL);

    int* keymap = getKeymap(argv[1]);


    while(1){
        result = read_event(fd, &jse);
        usleep(1000);
        if(result == 1){
        //printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n",jse.time, jse.value, jse.type, jse.number);
        switch(jse.type){
            case 1: buttonEvent(jse.number, jse.value, display, keymap); break;
            case 2: joystickEvent(jse.number, jse.value, display, keymap); break;
        }
        
        }
        
    }

    return 0;
}

//gcc -lX11 -lXtst driver.c -o joystick