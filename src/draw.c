#include <stdio.h>

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "task.h"

#include "TUM_Draw.h" 
#include "TUM_Font.h"
#include "TUM_Utils.h"
#include "TUM_Event.h"

#include "draw.h"

/* Aimed FPS */
#define FPS_AVERAGE_COUNT 50
#define FPS_FONT_COLOUR White

/* Filenames */
#define BACKGROUND_FILENAME "background.png"
#define LOGO_FILENAME "FlappyBird.png"

/* Definitions for drawButton */
#define BOX_COLOUR Teal
#define BOX_FRAME_COLOUR Silver
#define BOX_OUTLINE_COLOUR Black
#define BUTTON_TEXT_COLOUR White

/* Image handles */
image_handle_t backroundImage = NULL;
image_handle_t logoImage = NULL;
spritesheet_handle_t floorSpritesheet = NULL;
spritesheet_handle_t yellowBirdSpritesheet = NULL;

/* Animation Handle */
sequence_handle_t forwardSequence = NULL;
sequence_handle_t flappingBird = NULL;

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

/* Function to draw buttons on the screen */
int drawButton(coord_t pos, char *str) {

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
/* Init bird animations */
    char *yellowbird_spritesheet_path = tumUtilFindResourcePath("yellowbird_spritesheet.png");
    
    image_handle_t yellowBirdSpritesheetImage = tumDrawLoadImage(yellowbird_spritesheet_path);

    yellowBirdSpritesheet = tumDrawLoadSpritesheet(yellowBirdSpritesheetImage, 4, 1);

    animation_handle_t birdAnimation = tumDrawAnimationCreate(yellowBirdSpritesheet);

    tumDrawAnimationAddSequence(birdAnimation, "FLAPPING", 0, 0,
                                SPRITE_SEQUENCE_HORIZONTAL_POS, 4);

    flappingBird = tumDrawAnimationSequenceInstantiate(birdAnimation, "FLAPPING",
                                           120);
}

/* Function to draw floor animation */
void drawFloorAnnimations(TickType_t xLastFrameTime)
{
    tumDrawAnimationDrawFrame(forwardSequence,
        xTaskGetTickCount() - xLastFrameTime,
        0, SCREEN_HEIGHT - 70);
}

/* Function to draw floor animation */
void drawBirdAnnimations(TickType_t xLastFrameTime)
{
    tumDrawAnimationDrawFrame(flappingBird,
        xTaskGetTickCount() - xLastFrameTime,
        SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300);
}
