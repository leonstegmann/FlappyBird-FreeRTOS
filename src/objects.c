/* Standard library includes */
#include "stdio.h"
#include <stdlib.h> // for malloc()
#include "math.h" // for % operator

/* FreeRTOS includes  */
#include "FreeRTOS.h" // for TickType_t
#include "semphr.h"
#include "task.h" 

/* Project includes  */
#include "objects.h"
#include "defines.h"

bird_t* createNewPlayer(){

    bird_t* ret = malloc(sizeof(bird_t));

    ret->height = 24;
    ret->width = 34;
    ret->velocityY = 0;
    ret->max_velocity = GRAVITY*11;
    ret->score = 0;
    ret->pos = (coord_t) {(SCREEN_WIDTH - ret->width)/2, (SCREEN_HEIGHT-FLOOR_HEIGHT)/2 };
    ret->lock = xSemaphoreCreateMutex(); // Locking mechanism
    ret->dead = false;

    return ret;
}

void resetPlayer(bird_t* player) {
    
    xSemaphoreTake(player->lock, portMAX_DELAY);
    player->velocityY = 0;
    player->pos = (coord_t) {(SCREEN_WIDTH - player->width)/2, (SCREEN_HEIGHT-FLOOR_HEIGHT)/2 };
    player->dead = false;
    player->score = 0;
    xSemaphoreGive(player->lock);
}

void updateBirdPosition( TickType_t xLastTimeUpdated, bird_t* player){
    TickType_t xtimepassed =  xTaskGetTickCount() - xLastTimeUpdated;
    if( xSemaphoreTake(player->lock, portMAX_DELAY )== pdTRUE){
        player->pos.y += player->velocityY * (int) xtimepassed /1000;   // devided by 1000 milliseconds
            
        if(player->velocityY <= player->max_velocity) {
            player->velocityY+=GRAVITY;
        }

        if(player->pos.y >= SCREEN_HEIGHT - FLOOR_HEIGHT - player->height/2) {
            player->pos.y = SCREEN_HEIGHT - FLOOR_HEIGHT - player->height/2;
        } 

        if(player->pos.y <= 0) {
            player->pos.y = 0;
            player->velocityY = 3*GRAVITY;
        }

        xSemaphoreGive(player->lock);
    }    
}

void checkScore(bird_t* player ,pipes_t* pipe1, pipes_t* pipe2) {
    /* Check if bird passed pipe1*/
    if(player->pos.x + player->width/8 -1 >= pipe1->positionX + pipe1->image_width &&
        player->pos.x <= pipe1->positionX + pipe1->image_width) {
            player->score++;
        } 
    /* Check if bird passed pipe2 */
    if(player->pos.x + player->width/8 -1>= pipe2->positionX + pipe2->image_width &&
        player->pos.x <= pipe2->positionX + pipe2->image_width) {
            player->score++;
        } 

}

short checkCollision(bird_t* player ,pipes_t* pipe1, pipes_t* pipe2) {
    short ret = 0;

    /* Check if the bird hit the ground */
    if ((player->pos.y) >= SCREEN_HEIGHT - FLOOR_HEIGHT - player->height/2){
        xSemaphoreTake(player->lock, portMAX_DELAY);
        player->velocityY = 0;
        player->dead = true;
        xSemaphoreGive(player->lock);
        return ret = 1;
    } 

    /* Check if the bird hit pipe1 */
    if(player->pos.x + player->width >= pipe1->positionX 
        && player->pos.x <= pipe1->positionX + pipe1->image_width) {

        if(player->pos.y < pipe1->gap_center - GAP_HEIGHT/2 
            || player->pos.y + player->height >= pipe1->gap_center + GAP_HEIGHT/2 ) {

            xSemaphoreTake(player->lock, portMAX_DELAY);
            player->velocityY = 0;
            player->dead = true;
            xSemaphoreGive(player->lock);
            return ret = 1;
        }
    }

    /* Check if the bird hit pipe2 */
    if(player->pos.x + player->width >= pipe2->positionX 
        && player->pos.x <= pipe2->positionX + pipe2->image_width) {

        if(player->pos.y < pipe2->gap_center - GAP_HEIGHT/2 
            || player->pos.y + player->height >= pipe2->gap_center + GAP_HEIGHT/2 ) {

            xSemaphoreTake(player->lock, portMAX_DELAY);
            player->velocityY = 0;
            player->dead = true;
            xSemaphoreGive(player->lock);
            return ret = 1;
        }
    }

    return ret;
}

pipes_t* newPipe(){

    pipes_t* ret = malloc(sizeof(pipes_t));
    
    ret->lowerPipeImage = tumDrawLoadScaledImage(LOWER_PIPE_FILENAME, 0.2);
    ret->upperPipeImage = tumDrawLoadScaledImage(UPPER_PIPE_FILENAME, 0.2);
    ret->image_height = tumDrawGetLoadedImageHeight(ret->lowerPipeImage);
    ret->image_width = tumDrawGetLoadedImageWidth(ret->lowerPipeImage);
    ret->gap_center = (SCREEN_HEIGHT-FLOOR_HEIGHT)/2;
    ret->positionX = SCREEN_WIDTH*2;
    ret->velocityX = PIPE_VELOCITY;
    ret->lock = xSemaphoreCreateMutex(); // Locking mechanism
  
    return ret;

}

void resetPipes(pipes_t* pipe1, pipes_t* pipe2) {

    xSemaphoreTake(pipe1->lock, portMAX_DELAY);
    pipe1->gap_center = (SCREEN_HEIGHT-FLOOR_HEIGHT)/2;
    pipe1->positionX = SCREEN_WIDTH*2;
    pipe1->velocityX = PIPE_VELOCITY;
    xSemaphoreGive(pipe1->lock);

    xSemaphoreTake(pipe2->lock, portMAX_DELAY);
    pipe2->gap_center = (SCREEN_HEIGHT-FLOOR_HEIGHT)/2;
    pipe2->positionX = SCREEN_WIDTH*2.5;
    pipe2->velocityX = PIPE_VELOCITY;
    xSemaphoreGive(pipe2->lock);
}

short updatePipePosition( TickType_t xLastTimeUpdated, pipes_t* pipe){
    short ret = 0;
    if(pipe != NULL){
        TickType_t xtimepassed =  xTaskGetTickCount() - xLastTimeUpdated;
        if( xSemaphoreTake(pipe->lock, portMAX_DELAY) == pdTRUE){
            pipe->positionX -= pipe->velocityX* xtimepassed /1000;
            if (pipe->positionX + pipe->image_width < 0){
                pipe->gap_center = randomGenerator( GAP_LOWEST, GAP_HIGHEST); // generate Random Gap center
                pipe->positionX += SCREEN_WIDTH + pipe->image_width; //respawn Pipe at Right end of the Screen
            }
            xSemaphoreGive(pipe->lock);
            ret = 1;
        }

    }
    return ret;
}

short randomGenerator(short min, short max){
    short ret = min + (rand() % max);
    return  ret;
}