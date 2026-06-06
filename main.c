#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<termios.h>

/*** defines ***/
#define CTRL_KEY(k) ((k)&0x1f)

/*** data ***/
struct termios orig_termios;


/*** terminal ***/
void die(const char *s){
    perror(s);
    exit(1);
}

void disableRawMode(){
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios)==-1) {
        die("tcsetattr");
    }
}

void enableRawMode(){
    if(tcgetattr(STDIN_FILENO,&orig_termios)==-1){
        die("tcgetattr");
    }
    
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    //icanon send information for terminal buffer to program after enter so we are disabling it
    
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);

    raw.c_cc[VMIN] = 0; // the minimum number of bytes of input needed before read()
    raw.c_cc[VTIME] = 1; // the maximum amount of time to wait before read() 
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1){
        die("tcsetattr");
    }

}

char editorReadKey(){
    int nread;
    char c;
    while((nread=read(STDIN_FILENO,&c,1))!=1){
        if(nread == -1 && errno!=EAGAIN) die("read");
    }
    return c;
}

void editorProcessKeyPress(){
    char c = editorReadKey();
    switch(c){
        case CTRL_KEY('q'):
            exit(0);
            break;
    }
}

/*** init ***/
int main(){
    enableRawMode();
    while(1){
        editorProcessKeyPress();
    }
    
    return 0;
}


// testing some knowledge
// __attribute__((constructor))
// void init(void){
//     printf("working");
// }

// void _start(){
//     while(1);
// }