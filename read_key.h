#ifndef READ_KEY_H
#define READ_KEY_H

enum Keys {    
    K_UP  , K_DOWN   ,
    K_LEFT, K_RIGHT  ,
    K_F5  , K_F6     ,
    K_ESC , K_ENTER  ,
    K_L   , K_S      ,
    K_R   , K_T      ,
    K_I   , K_Q      ,
    K_E	  , K_UNKNOWN
};

void termInit();
void getDefaultTermSettings(struct termios *termState);
int readKey (enum Keys *key);
int termSave (struct termios *termState);
int termRestore (struct termios const *currState);
int termRegime (struct termios const *currState, int regime, int vtime, int vmin, int echo, int sigint);

#endif // READ_KEY_H
