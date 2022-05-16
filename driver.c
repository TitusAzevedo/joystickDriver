#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <joystick.h>


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

int joystickEvent(int axis, int value){
    switch(axis){
        //up down
        case 1: break;
        //lef right
        case 0: break;
    }
}

int buttonEvent(int button_id,  int value){
    switch(button_id){
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
    }
}



int main(int argc, char *argv[]){
    printf("KEY: %d\n",sendKey());
    //test
    int fd,result;
    struct js_event jse;
    fd = open_joystick(argv[1]);

    while(1){
    result = read_event(fd, &jse);
    usleep(1000);
    if(result == 1)
    printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n",jse.time, jse.value, jse.type, jse.number);
    switch(jse.type){
        case 1: buttonEvent(jse.number, jse.value); break;
        case 2: joystickEvent(jse.number, jse.value); break;
    }
    
    }

    return 0;
}