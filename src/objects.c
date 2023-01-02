
#include "objects.h"
#include "TUM_Draw.h"


void initBird(bird_t* player){
    player->pos = (coord_t) {SCREEN_HEIGHT/2, SCREEN_WIDTH/2};
    player->velocityY = 0;
    player->height = 34;
    player->width = 34;
}