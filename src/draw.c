#include <stdio.h>
#include <math.h>

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "task.h"

#include "TUM_Draw.h" 
#include "TUM_Font.h"
#include "TUM_Utils.h"
#include "TUM_Event.h"

#include "draw.h"
#include "objects.h" //Bird struct
#include "defines.h"
#include "stateMachine.h"

/* Aimed FPS */
#define FPS_AVERAGE_COUNT 50
#define FPS_FONT_COLOUR White

/* Filenames */
#define BACKGROUND_FILENAME "background.png"
#define LOGO_FILENAME "FlappyBird.png"
#define GAMEOVER_FILENAME "gameover.png"
#define PAUSE_FILENAME "paused.png"
#define HIGHSCORE_FILENAME "Score.png"
#define GOLDCOIN_FILENAME "goldCoin.png"
#define SILVERCOIN_FILENAME "silverCoin.png"
#define BRONZECOIN_FILENAME "bronzeCoin.png"

/* Definitions for drawButton */
#define BOX_COLOUR Teal
#define BOX_FRAME_COLOUR Silver
#define BOX_OUTLINE_COLOUR Black
#define BUTTON_TEXT_COLOUR White

/* Image handles */
image_handle_t backroundImage = NULL;
image_handle_t logoImage = NULL;
image_handle_t gameOver = NULL;
image_handle_t pauseImage = NULL;
image_handle_t highScore = NULL;
image_handle_t goldCoin = NULL;
image_handle_t silverCoin = NULL;
image_handle_t bronzeCoin = NULL;
spritesheet_handle_t floorSpritesheet = NULL;
spritesheet_handle_t yellowBirdSpritesheet = NULL;
spritesheet_handle_t redBirdSpritesheet = NULL;
spritesheet_handle_t blueBirdSpritesheet = NULL;

/* Animation Handle */
sequence_handle_t forwardSequence = NULL;
sequence_handle_t yellowFlappingBird = NULL;
sequence_handle_t redFlappingBird = NULL;
sequence_handle_t blueFlappingBird = NULL;

void drawPause(){
    static int image_height;
    static int image_width;

    if (pauseImage == NULL)
        pauseImage = tumDrawLoadScaledImage( PAUSE_FILENAME,0.2);
    if ((image_height = tumDrawGetLoadedImageHeight(pauseImage)) != -1)
        image_width = tumDrawGetLoadedImageWidth(pauseImage);    
    tumDrawLoadedImage(pauseImage,SCREEN_CENTER.x - image_width/2, 0.5 * SCREEN_CENTER.y - image_height/2);

}

void drawPipe(pipes_t* pipe){
    if( xSemaphoreTake(pipe->lock, portMAX_DELAY ) == pdTRUE){
        tumDrawLoadedImage(pipe->lowerPipeImage, pipe->positionX, pipe->gap_center + GAP_HEIGHT/2);
        tumDrawLoadedImage(pipe->upperPipeImage, pipe->positionX, pipe->gap_center - GAP_HEIGHT/2 -pipe->image_height);
        xSemaphoreGive(pipe->lock);
    }

}

/* Function to set the backround immage */
int drawBackround() {
    static int image_height;

    if (backroundImage == NULL) {
        backroundImage = tumDrawLoadImage(BACKGROUND_FILENAME);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(backroundImage)) != -1) {
        tumDrawLoadedImage(backroundImage, 0, 0);
        return 1;

    } else { 
        printf("Failed to get size of image '%s', does it exist?\n",
                BACKGROUND_FILENAME);
        return 0;
    }
}

/* Function to draw the FlappyBird baner on the screen */
int drawLogo(coord_t pos) {
    static int image_height;
    static int image_width;

    if (logoImage == NULL) {
        logoImage = tumDrawLoadImage(LOGO_FILENAME);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(logoImage)) != -1) {
        image_width = tumDrawGetLoadedImageWidth(logoImage);
        tumDrawLoadedImage(logoImage, pos.x - image_width/2, pos.y - image_height/2);
        return 1;

    } else { 
        printf("Failed to get size of image '%s', does it exist?\n",
                BACKGROUND_FILENAME);
        return 0;
    }
}

void drawGameOver() {
    static int image_height;
    static int image_width;

    if (gameOver == NULL) {
        gameOver = tumDrawLoadScaledImage(GAMEOVER_FILENAME,1);
    }
    if ((image_height = tumDrawGetLoadedImageHeight(gameOver)) != -1) {
        image_width = tumDrawGetLoadedImageWidth(gameOver);
        tumDrawLoadedImage(gameOver,SCREEN_CENTER.x - image_width/2, 0.5 * SCREEN_CENTER.y - image_height/2);
    }

}

/* Function to draw buttons on the screen */
int drawButton(coord_t pos, char *str, TickType_t lastFrameTime) {

    int mouse_x = tumEventGetMouseX();
    int mouse_y = tumEventGetMouseY();

    static int text_width;
    font_handle_t cur_font = tumFontGetCurFontHandle();

    // Draw outlines
    tumDrawFilledBox(pos.x , pos.y, BOX_WIDTH, BOX_HEIGHT + 3, BOX_OUTLINE_COLOUR);
    
    // Draw Box with Frame
    tumDrawFilledBox(pos.x + 2, pos.y + 2, BOX_WIDTH - 4, BOX_HEIGHT - 4, BOX_FRAME_COLOUR);

    if(mouse_x >= pos.x && mouse_x <= pos.x + BOX_WIDTH && mouse_y >= pos.y && mouse_y <= pos.y + BOX_HEIGHT) {

        tumDrawFilledBox(pos.x + 4, pos.y + 4, BOX_WIDTH - 8, BOX_HEIGHT - 8, Red);
        
        if(tumEventGetMouseLeft() && (xSemaphoreTake(stateMachine.lock, portMAX_DELAY) == pdTRUE)) {
            
            if((lastFrameTime - stateMachine.last_change) >= STATE_DEBOUNCE_DELAY) {

                stateMachine.str = str;
                handleStateInput(stateMachine.str, lastFrameTime);
                stateMachine.last_change = xTaskGetTickCount();
                stateMachine.str = NULL;

            }
            
            xSemaphoreGive(stateMachine.lock);     
        }   

    } else {
        tumDrawFilledBox(pos.x + 4, pos.y + 4, BOX_WIDTH - 8, BOX_HEIGHT - 8, BOX_COLOUR);
    }

    // Select Font
    tumFontSelectFontFromName(BUTTON_FONT);

    // Draw Text
    if (!tumGetTextSize((char *)str, &text_width, NULL)) {
        tumDrawText(str, pos.x + BOX_WIDTH/2 - text_width/2, 
                         pos.y + BOX_HEIGHT/2 - BUTTON_FONT_SIZE/6*3 , BUTTON_TEXT_COLOUR);
        
    }           

    // Reset Font
    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);

    return 1;
}

void drawScore(unsigned short count){
    
    static int text_width;
    static char string[10];
    font_handle_t cur_font = tumFontGetCurFontHandle();

    // Select Font
    tumFontSelectFontFromName(SCORE_FONT);

    sprintf(string, "%d", count);

    // Draw Text
    if (!tumGetTextSize((char *)string, &text_width, NULL)) {
        tumDrawText(string, SCREEN_WIDTH/2 - text_width/2 , SCREEN_HEIGHT/10 , White);
        
    }           

    // Reset Font
    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);
}

int drawHighscore(coord_t pos) {
    static int image_height;
    static int image_width;
    static int text_width;
    static char string[10];
    font_handle_t cur_font = tumFontGetCurFontHandle();

    if (highScore == NULL) {
        highScore = tumDrawLoadScaledImage(HIGHSCORE_FILENAME, 1.25);
    }

    if (goldCoin == NULL) {
        goldCoin = tumDrawLoadScaledImage(GOLDCOIN_FILENAME, 1.25);
    }

    if (silverCoin == NULL) {
        silverCoin = tumDrawLoadScaledImage(SILVERCOIN_FILENAME, 1.25);
    }

    if (bronzeCoin == NULL) {
        bronzeCoin = tumDrawLoadScaledImage(BRONZECOIN_FILENAME, 1.25);
    }

    if ((image_height = tumDrawGetLoadedImageHeight(highScore)) != -1) {
        image_width = tumDrawGetLoadedImageWidth(highScore);
        tumDrawLoadedImage(highScore, pos.x - image_width/2, pos.y - image_height/2);

    } else { 
        printf("Failed to get size of image '%s', does it exist?\n",
                HIGHSCORE_FILENAME);
        return 0;
    }

    // Select Font
    tumFontSelectFontFromName(HIGHSCORE_FONT);

    /* print last score */
    sprintf(string, "%d", highscore.score[1]);

    if (!tumGetTextSize((char *)string, &text_width, NULL)) {
        tumDrawText(string, pos.x - text_width/2 + (image_width/8)*3 , pos.y - DEFAULT_FONT_SIZE , White);  
    }

    /* print highest score */
    sprintf(string, "%d", highscore.score[0]);

    if (!tumGetTextSize((char *)string, &text_width, NULL)) {
        tumDrawText(string, pos.x - text_width/2 + (image_width/8)*3 , pos.y + DEFAULT_FONT_SIZE*1.5 , White);  
    }                

    // Reset Font
    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);

    if(highscore.score[0] >= 15) {
        image_height = tumDrawGetLoadedImageHeight(goldCoin);
        image_width = tumDrawGetLoadedImageWidth(goldCoin);
        tumDrawLoadedImage(goldCoin, pos.x - image_width*2 , pos.y - image_height/2 + 7);
        return 1;
        
    } else if(highscore.score[0] >= 10) {
        image_height = tumDrawGetLoadedImageHeight(silverCoin);
        image_width = tumDrawGetLoadedImageWidth(silverCoin);
        tumDrawLoadedImage(silverCoin, pos.x - image_width*2 , pos.y - image_height/2 + 7);
        return 1;

    } else if(highscore.score[0] >= 5) {
        image_height = tumDrawGetLoadedImageHeight(bronzeCoin);
        image_width = tumDrawGetLoadedImageWidth(bronzeCoin);
        tumDrawLoadedImage(bronzeCoin, pos.x - image_width*2 , pos.y - image_height/2 + 7);
        return 1;
    }      

    return 1;
}

/* Function to draw FPS on the screen */
void drawFPS(void)
{
    static unsigned int periods[FPS_AVERAGE_COUNT] = { 0 };
    static unsigned int periods_total = 0;
    static unsigned int index = 0;
    static unsigned int average_count = 0;
    static TickType_t xLastWakeTime = 0, prevWakeTime = 0;
    static char str[10] = { 0 };
    static int text_width;
    int fps = 0;
    font_handle_t cur_font = tumFontGetCurFontHandle();

    if (average_count < FPS_AVERAGE_COUNT) {
        average_count++;
    }
    else {
        periods_total -= periods[index];
    }

    xLastWakeTime = xTaskGetTickCount();

    if (prevWakeTime != xLastWakeTime) {
        periods[index] =
            configTICK_RATE_HZ / (xLastWakeTime - prevWakeTime);
        prevWakeTime = xLastWakeTime;
    }
    else {
        periods[index] = 0;
    }

    periods_total += periods[index];

    if (index == (FPS_AVERAGE_COUNT - 1)) {
        index = 0;
    }
    else {
        index++;
    }

    fps = periods_total / average_count;

    tumFontSelectFontFromName(FPS_FONT);

    sprintf(str, "FPS: %2d", fps);

    if (!tumGetTextSize((char *)str, &text_width, NULL)) {
        tumDrawText(str, SCREEN_WIDTH - text_width - 10,
                         SCREEN_HEIGHT - DEFAULT_FONT_SIZE * 1.5,
                         FPS_FONT_COLOUR);
    }

    tumFontSelectFontFromHandle(cur_font);
    tumFontPutFontHandle(cur_font);
}

/* Function to initialize the Animations */
void drawInitAnnimations(void)
{   
    /* Init floor Animations */
    // Set filenames to the spritesheets
    char *floor_spritesheet_path = tumUtilFindResourcePath("floor_spritesheet.png");  

    // Create handles for the images
    image_handle_t floorSpritesheetImage = tumDrawLoadImage(floor_spritesheet_path);

    // Load spritesheets with number of columns and rows
    floorSpritesheet = tumDrawLoadSpritesheet(floorSpritesheetImage, 9, 1);

    // Create animation handle with respective spritesheet
    animation_handle_t floorAnimation = tumDrawAnimationCreate(floorSpritesheet);

    // Add Sequence with starting column and row, direction and #frames
    tumDrawAnimationAddSequence(floorAnimation, "FORWARDS", 0, 0,
                                SPRITE_SEQUENCE_HORIZONTAL_POS, 8);

    // Set sequence handle with time in ms between the frames
    forwardSequence = tumDrawAnimationSequenceInstantiate(floorAnimation, "FORWARDS",
                                            40);

    /* Init yellow bird animations */
    char *yellowbird_spritesheet_path = tumUtilFindResourcePath("yellowbird_spritesheet.png");
    
    image_handle_t yellowBirdSpritesheetImage = tumDrawLoadImage(yellowbird_spritesheet_path);

    yellowBirdSpritesheet = tumDrawLoadSpritesheet(yellowBirdSpritesheetImage, 4, 1);

    animation_handle_t yellowBirdAnimation = tumDrawAnimationCreate(yellowBirdSpritesheet);

    tumDrawAnimationAddSequence(yellowBirdAnimation, "FLAPPING", 0, 0,
                                SPRITE_SEQUENCE_HORIZONTAL_POS, 4);

    yellowFlappingBird = tumDrawAnimationSequenceInstantiate(yellowBirdAnimation, "FLAPPING",
                                           120);
                                        
    /* Init yellow bird animations */
    char *redbird_spritesheet_path = tumUtilFindResourcePath("redbird_spritesheet.png");
    
    image_handle_t redBirdSpritesheetImage = tumDrawLoadImage(redbird_spritesheet_path);

    redBirdSpritesheet = tumDrawLoadSpritesheet(redBirdSpritesheetImage, 4, 1);

    animation_handle_t redBirdAnimation = tumDrawAnimationCreate(redBirdSpritesheet);

    tumDrawAnimationAddSequence(redBirdAnimation, "FLAPPING", 0, 0,
                                SPRITE_SEQUENCE_HORIZONTAL_POS, 4);

    redFlappingBird = tumDrawAnimationSequenceInstantiate(redBirdAnimation, "FLAPPING",
                                           120);
                                        
    /* Init yellow bird animations */
    char *bluebird_spritesheet_path = tumUtilFindResourcePath("bluebird_spritesheet.png");
    
    image_handle_t blueBirdSpritesheetImage = tumDrawLoadImage(bluebird_spritesheet_path);

    blueBirdSpritesheet = tumDrawLoadSpritesheet(blueBirdSpritesheetImage, 4, 1);

    animation_handle_t blueBirdAnimation = tumDrawAnimationCreate(blueBirdSpritesheet);

    tumDrawAnimationAddSequence(blueBirdAnimation, "FLAPPING", 0, 0,
                                SPRITE_SEQUENCE_HORIZONTAL_POS, 4);

    blueFlappingBird = tumDrawAnimationSequenceInstantiate(blueBirdAnimation, "FLAPPING",
                                           120);
}

/* Function to draw floor animation */
void drawFloorAnnimations(TickType_t xLastFrameTime)
{
    tumDrawAnimationDrawFrame(forwardSequence,
        xTaskGetTickCount() - xLastFrameTime,
        0, SCREEN_HEIGHT - FLOOR_HEIGHT);
}

/* Function to draw bird animation */
void drawBirdAnnimations(TickType_t xLastFrameTime, int colour) 
{   
    if (colour == YELLOW) {
        tumDrawAnimationDrawFrame(yellowFlappingBird,
            xTaskGetTickCount() - xLastFrameTime,
            SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300 - 10*sin(xLastFrameTime/(80*3.14)));
    }
    else if (colour == RED) {
        tumDrawAnimationDrawFrame(redFlappingBird,
            xTaskGetTickCount() - xLastFrameTime,
            SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300 - 10*sin(xLastFrameTime/(80*3.14)));
    }
    else if (colour == BLUE) {
        tumDrawAnimationDrawFrame(blueFlappingBird,
            xTaskGetTickCount() - xLastFrameTime,
            SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300 - 10*sin(xLastFrameTime/(80*3.14)));
    }

}

/* Function to draw bird animation */
void drawBirdAnnimationsInGame(TickType_t xLastFrameTime, bird_t* player, int colour) 
{
    
    if (colour == YELLOW) {
        tumDrawAnimationDrawFrame(yellowFlappingBird,
        xTaskGetTickCount() - xLastFrameTime,
        player->pos.x, player->pos.y);;
    }
    else if (colour == RED) {
        tumDrawAnimationDrawFrame(redFlappingBird,
        xTaskGetTickCount() - xLastFrameTime,
        player->pos.x, player->pos.y);
    }
    else if (colour == BLUE) {
        tumDrawAnimationDrawFrame(blueFlappingBird,
        xTaskGetTickCount() - xLastFrameTime,
        player->pos.x, player->pos.y);
    }

}

