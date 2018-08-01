#include <SPI.h>
#include "Keyboard.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "latex_colors.h"
#include "icons.h"

//These #defines are specific to the ARDUINO PRO MICRO
#define RA8875_INT     8
#define RA8875_CS      10
#define RA8875_RESET   7

/* For a 4.3" LCD - Alter for other displays*/
#define LCD_WIDTH 480
#define LCD_HEIGHT 272

/* Mimics layout of StreamDeck */
#define NUM_ROWS 3
#define NUM_COLS 5

/* The bitmaps are the displayed buttons, these are the touch areas for the buttons*/
#define BOX_WIDTH 64
#define BOX_HEIGHT 64

#define LCD_MARGIN 20

#define DEFAULT_SCENE 0

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

/* Button Array stores the X and Y corners of the button */
uint16_t button_dims[NUM_ROWS*NUM_COLS][4];
uint8_t button = 0;
uint8_t active_scene = DEFAULT_SCENE;
bool vol_toggle, bcast_toggle, rec_toggle, mic_toggle, webcam_toggle = 0;

void toggleScene(uint8_t current_scene){
  switch(current_scene){
    case 0:
      tft.drawBitmap(button_dims[current_scene][0], button_dims[current_scene][2], icon_scen_intro, 64, 64, WHITE);
      break;
      
    case 1:
      tft.drawBitmap(button_dims[current_scene][0], button_dims[current_scene][2], icon_scen_1, 64, 64, WHITE);
      break;
      
    case 2:
      tft.drawBitmap(button_dims[current_scene][0], button_dims[current_scene][2], icon_scen_2, 64, 64, WHITE);
      break;
      
    case 3:
      tft.drawBitmap(button_dims[current_scene][0], button_dims[current_scene][2], icon_scen_3, 64, 64, WHITE);
      break;
      
    case 4:
      tft.drawBitmap(button_dims[current_scene][0], button_dims[current_scene][2], icon_scen_outro, 64, 64, WHITE);
      break;
      
    default:
    break;
    
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Welcome to OBS Control....");

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_480x272)) {
    Serial.println("ERROR: RA8875 LCD control board not found!");
    while (1);
  } else { 
    Serial.println("INFO: Found RA8875 LCD board...");
  }

  Serial.println("INFO: Initializing LCD...");
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  // With hardware accelleration this is instant
  tft.fillScreen(BLACK);

  //Draw the boxes for the input
  Serial.println("INFO: Drawing the buttons...");
  uint8_t col_spacing = (LCD_WIDTH-(NUM_COLS*BOX_WIDTH)-(LCD_MARGIN*2))/(NUM_COLS-1);
  uint8_t row_spacing = (LCD_HEIGHT-(NUM_ROWS*BOX_HEIGHT)-(LCD_MARGIN*2))/(NUM_ROWS-1);

  for(uint8_t row=0; row < NUM_ROWS; row++){
      for(uint8_t col=0; col < NUM_COLS; col++){
        //store the meta-data about the button
        button_dims[button][0] = LCD_MARGIN+(col*(BOX_WIDTH+col_spacing));  
        button_dims[button][1] = LCD_MARGIN+(col*(BOX_WIDTH+col_spacing))+BOX_WIDTH;
        button_dims[button][2] = LCD_MARGIN+row*(BOX_HEIGHT+row_spacing);
        button_dims[button][3] = LCD_MARGIN+row*(BOX_HEIGHT+row_spacing)+BOX_HEIGHT;
        
        switch (button) {
        case 0:
          if(button == DEFAULT_SCENE) {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_intro, 64, 64, BLUE);
            active_scene = 0;
            Keyboard.begin();
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.press(KEY_F1); 
            Keyboard.releaseAll();
            Keyboard.end();            
          } else {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_intro, 64, 64, WHITE);  
          }
          break;
          
        case 1:
          if(button == DEFAULT_SCENE) {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_1, 64, 64, BLUE);
            active_scene = 0;
            Keyboard.begin();
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.press(KEY_F2); 
            Keyboard.releaseAll();
            Keyboard.end();            
          } else {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_1, 64, 64, WHITE);  
          }
          break;
        
        case 2:
          if(button == DEFAULT_SCENE) {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_2, 64, 64, BLUE);
            active_scene = 0;
            Keyboard.begin();
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.press(KEY_F3); 
            Keyboard.releaseAll();
            Keyboard.end();            
          } else {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_2, 64, 64, WHITE);  
          }
          break;
        
        case 3:
          if(button == DEFAULT_SCENE) {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_3, 64, 64, BLUE);
            active_scene = 0;
            Keyboard.begin();
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.press(KEY_F4); 
            Keyboard.releaseAll();
            Keyboard.end();            
          } else {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_3, 64, 64, WHITE);  
          }
          break;
        
        case 4:
          if(button == DEFAULT_SCENE) {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_outro, 64, 64, BLUE);
            active_scene = 0;
            Keyboard.begin();
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_RIGHT_ALT);
            Keyboard.press(KEY_F5); 
            Keyboard.releaseAll();
            Keyboard.end();            
          } else {
            tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_outro, 64, 64, WHITE);  
          }
          break;      
                        
        case 5:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_webcam_on, 64, 64, WHITE);
          break;
          
        case 6:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_mic_on, 64, 64, WHITE);
          break;           

        case 7:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_snd_on, 64, 64, WHITE);
          break;
        
        case 8:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_rec_on, 64, 64, WHITE);
          break; 

        case 9:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_bcast_on, 64, 64, WHITE);
          break; 

        default:
          tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_blank, 64, 64, WHITE);
          break;         
        }

        button++;                
      }    
  }

  //Set up the touch interface w/ interrupts
  Serial.println("INFO: Setting up touch interface...");
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true);  

}

void loop() {
  int x, y;
  float xScale = 1024.0F/tft.width();
  float yScale = 1024.0F/tft.height();
  
  /* Wait around for touch events */
  if (! digitalRead(RA8875_INT)) 
  {
    while (tft.touched()) 
    {
      tft.touchRead(&tx, &ty);
      x = tx/xScale;
      y = ty/yScale;
      //Serial.print(x); Serial.print(", "); Serial.println(y);
      delay(2);
    }
      for(uint8_t button=0; button<(NUM_ROWS*NUM_COLS); button++){
        if((x >= button_dims[button][0]) && (x <= button_dims[button][1])){
          if((y >= button_dims[button][2]) && (y <= button_dims[button][3])){

            switch(button) {
            case 0:
              if(active_scene == button) {
                  break;        
              } else {
                toggleScene(active_scene);
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_intro, 64, 64, BLUE);
                active_scene = 0;
                Keyboard.begin();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press(KEY_F1); 
                Keyboard.releaseAll();
                Keyboard.end();
                break;  
              }
              break;

            case 1:
              if(active_scene == button) {
                  break;        
              } else {
                 toggleScene(active_scene);
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_1, 64, 64, BLUE);
                active_scene = 1;
                Keyboard.begin();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press(KEY_F2); 
                Keyboard.releaseAll();
                Keyboard.end();
                break;               
              }
              break;

            case 2:
              if(active_scene == button) {
                  break;        
              } else {
                toggleScene(active_scene);
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_2, 64, 64, BLUE);
                active_scene = 2;
                Keyboard.begin();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press(KEY_F3); 
                Keyboard.releaseAll();
                Keyboard.end();
                break;                
              }
              break;

            case 3:
              if(active_scene == button) {
                  break;        
              } else {
                toggleScene(active_scene);
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_3, 64, 64, BLUE);
                active_scene = 3;
                Keyboard.begin();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press(KEY_F4); 
                Keyboard.releaseAll();
                Keyboard.end();
                break;                
              }
              break;

            case 4:
              if(active_scene == button) {
                  break;        
              } else {
                toggleScene(active_scene);
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_scen_outro, 64, 64, BLUE);
                active_scene = 4;
                Keyboard.begin();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press(KEY_F5); 
                Keyboard.releaseAll();
                Keyboard.end();               
              }
              break;

            case 5:
              webcam_toggle = !webcam_toggle;
              if(webcam_toggle == 1){
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_webcam_on, 64, 64, RED);
              } else {
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_webcam_on, 64, 64, WHITE);
              }
              Keyboard.begin();
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_RIGHT_ALT);
              Keyboard.press(KEY_F8); 
              Keyboard.releaseAll();
              Keyboard.end();
              break;
              
            case 6:
              mic_toggle = !mic_toggle;
              if(mic_toggle == 1){
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_mic_on, 64, 64, RED);
              } else {
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_mic_on, 64, 64, WHITE);
              }
              Keyboard.begin();
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_RIGHT_ALT);
              Keyboard.press(KEY_F9); 
              Keyboard.releaseAll();
              Keyboard.end();
              break;

              case 7:
              vol_toggle = !vol_toggle;
              if(vol_toggle == 1){
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_snd_on, 64, 64, RED);
              } else {
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_snd_on, 64, 64, WHITE);
              }
              Keyboard.begin();
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_RIGHT_ALT);
              Keyboard.press(KEY_F12); 
              Keyboard.releaseAll();
              Keyboard.end();
              break;
              
              case 8:
              rec_toggle = !rec_toggle;
              if(rec_toggle == 1){
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_rec_on, 64, 64, RED);
              } else {
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_rec_on, 64, 64, WHITE);
              }

              Keyboard.begin();
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_RIGHT_ALT);
              Keyboard.press(KEY_F10); 
              Keyboard.releaseAll();
              Keyboard.end();
              break;
                            
            case 9:
              bcast_toggle = !bcast_toggle;
              if(bcast_toggle == 1){
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_bcast_on, 64, 64, RED);
              } else {
                tft.drawBitmap(button_dims[button][0], button_dims[button][2], icon_bcast_on, 64, 64, WHITE);
              }
              Keyboard.begin();
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press(KEY_RIGHT_ALT);
              Keyboard.press(KEY_F11); 
              Keyboard.releaseAll();
              Keyboard.end();
              break;            
             
            default:
              break;              
            }
          }
        }
      }

  }

}
