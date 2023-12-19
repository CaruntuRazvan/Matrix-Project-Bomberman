#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define UP_ARROW      byte(0)
#define DOWN_ARROW    byte(1)
#define BACK_ARROW    byte(2)
#define INCREASE_ARROW      byte(3)
#define DECREASE_ARROW      byte(4)
#define SAD_FACE      byte(5)
#define HEART         byte(6)
#define CHECK         byte(7)
#define BLANK         (char) 254

#define MAIN_MENU       0
#define START_MENU      1
#define START_EASY_MAP  12
#define START_HARD_MAP  13
#define BACK_HIGHSCORES_SUBMENU 14
#define SETTINGS_MENU   2
#define HIGHSCORE_MENU 3
#define ABOUT_MENU 4
#define HOWTOPLAY_MENU 5
#define LCD_CONTRAST_SUBMENU 6
#define MATRIX_BRIGHTNESS_SUBMENU 7
#define LCD_BRIGHTNESS_SUBMENU 8
#define BUZZER_SUBMENU 9
#define RESET_SUBMENU 10
#define BACK_SETTINGS_SUBMENU 11

#define BACK_SELECTED 0
#define RESTART_SELECTED 1

#define CHOICE_MAP 0
#define EASY_MAP 0
#define HARD_MAP 1

#define UPPER_MAIN_MENU 0
#define LOWER_MAIN_MENU 1
#define THIRD_MAIN_MENU 2

#define UPPER_SETTINGS_MENU 0
#define LOWER_SETTINGS_MENU 1
#define THIRD_SETTINGS_MENU 2

#define UPPER_HIGHSCORES_MENU 0
#define LOWER_HIGHSCORES_MENU 1

#define ITEM_HIGHSCORES_BACK 0
#define ITEM_HIGHSCORES_1 1
#define ITEM_HIGHSCORES_2 2
#define ITEM_HIGHSCORES_3 3

#define ITEM_START 0
#define ITEM_SETTINGS 1
#define ITEM_HIGHSCORE 2
#define ITEM_ABOUT 3
#define ITEM_HOWTOPLAY 4

#define ITEM_SETTINGS_BACK 0
#define ITEM_SETTINGS_LCD 1
#define ITEM_SETTINGS_MATRIX 2
#define ITEM_SETTINGS_LCD_B 3
#define ITEM_SETTINGS_BUZZER 4
#define ITEM_SETTINGS_RESET 5

#define SOUND_ON 0
#define SOUND_OFF 1

#define RESET_HIGHSCORES_YES 0
#define RESET_HIGHSCORES_NO 1

int menuState = MAIN_MENU;
int mainMenuState = UPPER_MAIN_MENU; 
int gameState = CHOICE_MAP;
int gameSubmenuState = EASY_MAP;
int settingsSubMenuState = UPPER_SETTINGS_MENU;

int highscoresSubMenuState = UPPER_HIGHSCORES_MENU;
int highscoresSelectedMenu = ITEM_HIGHSCORES_BACK;

int mainMenuSelectedValue = ITEM_START;
int settingsSelectedMenu = ITEM_SETTINGS_LCD;
int settingsSubmenuSound = SOUND_ON;
int settingsSubmenuReset = RESET_HIGHSCORES_NO;

const int lcdContrastEEPROMAddress = 0;
const int matrixBrightnessEEPROMAddress = 1;
const int lcdBrightnessEEPROMAddress = 3;
const int soundOnOffEEPROMAddress = 2;
const int highScoresEEPROMAddress = 4;

const byte rs = 11;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int dinPin = 12;
const int clockPin = 13;
const int loadPin = 2;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const byte LCDbrightnessPin = 10;
const int lcdContrastPin = 9;
const int numRows = 8;
const int numCols = 8;

const int xPin = A0;
const int yPin = A1;
const int swPin = A4;
const int buzzerPin = A5;
const int placingBombSoundDuration = 500; 
const int explosionSoundDuration = 400;   
const int swithcingThroughMenuDuration = 15;
const int lowerThreshold = 300;
const int upperThreshold = 600;
String chosenLevel="";
int destroyedWalls = 0;

const int playerBlinkRate = 550;
const int bombBlinkRate = 250; 
const int bombExplosionTime = 3000; //time between placing the bomb and the actual explosion
const int explosionRange = 1;//range of the bomb -1 unit
const int explosionSoundFrequency = 2500;//for buzzer
const int placingBombFrequency = 1000; //for buzzer
const int swithcingThroughMenuFrequency = 450; //for buzzer
bool bombIsActive = false;
bool playerIsDead = false;
bool allWallsDestroyed = false;
bool deathAnimationActive = false;
bool playerWins = false;
bool winAnimationActive = false;
const int redLedPin = 3; 
int gameBoard[numRows][numCols];
int playerRow = 0;
int playerCol = 0;
bool playerBlinkState = false;
bool mapGenerated = false;
//const int soundBombExplosionTimeFrequency = 500;
byte HTPscrollText = 0;
const int animationDelay = 100;
const int maxRandomValue = 100;
const int percentageWall = 5;
const int twoNumber = 2;
int lastIndex = 0;
int playerLives = 2;
const int freeSpaces = 2;
int xValue = 0;
int yValue = 0;
int yPressed = 0;
int swValue;
int currentSWValue = 0;
int previousSWValue = 0;
const unsigned long introDuration = 4000;
const unsigned long loadingDuration = 3000;
unsigned long lastActionTime = 0;
bool introMessage = false;
bool loadingMessage = false;
bool loadingMessageDisplayed = false; 
bool soundOn = true;
bool showSettingsImage = false;
int lcdContrastValue;
int matrixBrightnessValue;

//lcd contrast
const int lowBrightness = 5; 
const int mediumBrightness = 10; 
const int highBrightness = 15; 
const int minBrightness = 0;
const int maxBrightness = 255;
const int brightnessStep = 1;

//lcd brightness
const int brightnessStepLcd = 20;
const int minBrightnessLcd = 5;
const int maxBrightnessLcd = 255;
int lcdBrightnessValue = 0;

//lcd contrast
const int lowContrastValue = 5; 
const int mediumContrastValue = 10; 
const int highContrastValue = 15; 
const int contrastStep = 2;
const int minContrastValue = 1;
const int maxContrastValue = 15;
const int lowestContrastValueMap = 15;
const int lowerContrastValueMap = 30;
const int mediumContrastValueMap = 150;
const int highestContrastValueMap = 220;

// for how to play section
byte scrollTextPosition = 0;
const String messageHTP = "Place bombs near walls to destroy them. The bomb explodes upward, downward, left, right. If you destroy all the walls, you will win the game.";
const int delayOnScroll = 350;
unsigned long prevScrollTime = 0;

const int maxHighScores = 3;
int highScores[maxHighScores] = {0}; // Initialize high scores to 0
bool newHighScore = false;

bool gameOver = false;
struct Bomb {
  int row;
  int col;
  bool blinkState;
  unsigned long explosionTime;
};

const int maxBombs = 5;
Bomb bombs[maxBombs];

const int sizeOfElement = 8;
//for lose animation
const uint64_t mainMenuImage[] = {
  0xc3c3c3dbffffe7c3
};
const uint64_t easyImage[] = {
  0x7e7e063e3e067e7e
};
const uint64_t hardImage[] = {
  0x6666667e7e666666
};
//for lose animation
const uint64_t loseImages[] = {
    0x0000000000000000,
    0x8142241818244281
};
//for win animation
const uint64_t winImages[] = {
    0x0000000000000000,
    0x00040a1120408000
};
const uint64_t bombImage[] = {
  0x3c7e7e7e7e3c1060
};
const uint64_t aboutImage[] = {
  0x3c4299999999423c
};
const uint64_t settingsImage[] = {
  0x03070e7cf8983830
};

const uint64_t highscoresImage[] = {
  0x3c18183cffbdff3c
};

const uint64_t howToPlayImage[] = {
  0x0800080818202418
};

byte downArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

byte upArrow[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte backArrow[] = {
  B00010,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00010,
  B00000
};
byte increaseArrow[] = {
  B00000,
  B01111,
  B00011,
  B00101,
  B01001,
  B10000,
  B00000,
  B00000
};

byte decreaseArrow[] = {
  B00000,
  B00000,
  B10000,
  B01001,
  B00101,
  B00011,
  B01111,
  B00000
};
byte sadFace[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001,
  B00000
};
byte heart[] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte check[] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  
  lcd.createChar(UP_ARROW, upArrow);
  lcd.home();
  lcd.createChar(DOWN_ARROW, downArrow);
  lcd.home();
  lcd.createChar(BACK_ARROW, backArrow);
  lcd.home();
  lcd.createChar(INCREASE_ARROW, increaseArrow);
  lcd.home();
  lcd.createChar(DECREASE_ARROW, decreaseArrow);
  lcd.home();
  lcd.createChar(SAD_FACE, sadFace);
  lcd.home();
  lcd.createChar(HEART, heart);
  lcd.home();
  lcd.createChar(CHECK, check);
  lcd.home();
    
  lcdContrastValue = EEPROM.read(lcdContrastEEPROMAddress);
  updateLcdContrast();
  matrixBrightnessValue = EEPROM.read(matrixBrightnessEEPROMAddress);
  soundOn = EEPROM.read(soundOnOffEEPROMAddress);
  lcdBrightnessValue = EEPROM.read(lcdBrightnessEEPROMAddress);
  updateLCDBrightness();
  lc.setIntensity(0, matrixBrightnessValue);
  loadHighScoresFromEEPROM();
  pinMode(swPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}
void loop() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  currentSWValue = !digitalRead(swPin);

  if (!introMessage) {
    displayWelcomeMessage();
  } else if (menuState == MAIN_MENU) {
    displayMainMenuImage();
    if (mainMenuState == UPPER_MAIN_MENU) {
      drawUpperMainMenu();
    } else if (mainMenuState == LOWER_MAIN_MENU) {
      drawLowerMainMenu();
    } else if (mainMenuState == THIRD_MAIN_MENU) {
      drawThirdMainMenu();
    }

    if (mainMenuSelectedValue == ITEM_START && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = START_MENU;
      lcd.clear();
    } else if (mainMenuSelectedValue == ITEM_SETTINGS && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = SETTINGS_MENU;
      lcd.clear();
    } else if (mainMenuSelectedValue == ITEM_HIGHSCORE && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = HIGHSCORE_MENU;
      lcd.clear();
    } else if (mainMenuSelectedValue == ITEM_ABOUT && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = ABOUT_MENU;
      lcd.clear();
    } else if (mainMenuSelectedValue == ITEM_HOWTOPLAY && currentSWValue == 1 && previousSWValue != currentSWValue) {
      HTPscrollText = 1;
      menuState = HOWTOPLAY_MENU;
      lcd.clear();
    }
    previousSWValue = currentSWValue;
  } else if (menuState == START_MENU) {
    if (gameState == CHOICE_MAP) {
      drawChoiceSubmenu();
    }
    if (gameSubmenuState == EASY_MAP) {
      displayEasyImage();
    }
    if (gameSubmenuState == HARD_MAP) {
      displayHardImage();
    }
    if (gameSubmenuState == EASY_MAP && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = START_EASY_MAP;
      lcd.clear();
    } else if (gameSubmenuState == HARD_MAP && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = START_HARD_MAP;
      lcd.clear();
    }
    previousSWValue = currentSWValue;

  } else if (menuState == SETTINGS_MENU) {
    showSettingsImage = true;
    displaySettingsImage();
    if (settingsSubMenuState == UPPER_SETTINGS_MENU) {
      drawUpperSettingsMenu();
    } else if (settingsSubMenuState == LOWER_SETTINGS_MENU) {
      drawLowerSettingsMenu();
    } else if (settingsSubMenuState == THIRD_SETTINGS_MENU) {
      drawThirdSettingsMenu();
    }
    if (settingsSelectedMenu == ITEM_SETTINGS_LCD && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = LCD_CONTRAST_SUBMENU;
      lcd.clear();
    } else if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = MATRIX_BRIGHTNESS_SUBMENU;
      lcd.clear();

    } else if (settingsSelectedMenu == ITEM_SETTINGS_LCD_B && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = LCD_BRIGHTNESS_SUBMENU;
      lcd.clear();
    } else if (settingsSelectedMenu == ITEM_SETTINGS_BUZZER && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = BUZZER_SUBMENU;
      lcd.clear();

    } else if (settingsSelectedMenu == ITEM_SETTINGS_RESET && currentSWValue == 1 && previousSWValue != currentSWValue) {
      menuState = RESET_SUBMENU;
      lcd.clear();

    } else if (settingsSelectedMenu == ITEM_SETTINGS_BACK && currentSWValue == 1 && previousSWValue != currentSWValue && showSettingsImage == true) {
      showSettingsImage = false;
      displayClearBoard();
      menuState = BACK_SETTINGS_SUBMENU;
      lcd.clear();
    }
    previousSWValue = currentSWValue;

  } else if (menuState == ABOUT_MENU) {
    drawAboutMenu();
  } else if (menuState == HIGHSCORE_MENU) {
    displayHighscoresImage();
    if (highscoresSubMenuState == UPPER_HIGHSCORES_MENU) {
      drawUpperHighscoresMenu();
    } else if (highscoresSubMenuState == LOWER_HIGHSCORES_MENU) {
      drawLowerHighscoresMenu();
    }
    if (highscoresSelectedMenu == ITEM_HIGHSCORES_BACK && currentSWValue == 1 && previousSWValue != currentSWValue) {
      displayClearBoard();
      menuState = BACK_HIGHSCORES_SUBMENU;
      lcd.clear();
    }
    previousSWValue = currentSWValue;
  } else if (menuState == HOWTOPLAY_MENU) {
    drawHowToPlayMenu();
  } else if (menuState == LCD_CONTRAST_SUBMENU) {
    drawLCDContrast();
  } else if (menuState == MATRIX_BRIGHTNESS_SUBMENU) {
    drawMatrixBrightness();
  } else if (menuState == LCD_BRIGHTNESS_SUBMENU) {
    drawLCDBrightness();
  } else if (menuState == BUZZER_SUBMENU) {
    drawSoundOnOff();

  } else if (menuState == RESET_SUBMENU) {
    drawResetHighscores();

  } else if (menuState == BACK_SETTINGS_SUBMENU) {
    menuState = MAIN_MENU;
    mainMenuState = LOWER_MAIN_MENU;
    lcd.clear();
    previousSWValue = currentSWValue;
  } else if (menuState == START_EASY_MAP) {
    drawStartEasyMap();
  } else if (menuState == START_HARD_MAP) {
    drawStartHardMap();
  } else if (menuState == BACK_HIGHSCORES_SUBMENU) {
    menuState = MAIN_MENU;
    mainMenuState = LOWER_MAIN_MENU;
    lcd.clear();
    previousSWValue = currentSWValue;
  }
}

void drawChoiceSubmenu(){
  lcd.setCursor(0, 0);
  lcd.print("Easy");
  if (gameSubmenuState == EASY_MAP){
    lcd.print(" <");
  }
  lcd.setCursor(0, 1);
  lcd.print("Hard");
  if (gameSubmenuState == HARD_MAP){
    lcd.print(" <");
  }

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
      tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    if (gameSubmenuState == HARD_MAP) {
      //lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print(BLANK);
      gameSubmenuState = EASY_MAP;
    } 
  } 
  else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
      tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    if (gameSubmenuState == EASY_MAP) {
        lcd.setCursor(5, 0);
        lcd.print(BLANK);
        gameSubmenuState = HARD_MAP;
    }

  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  } 
  
}

void drawStartEasyMap(){
  chosenLevel="Easy";
  if(!loadingMessageDisplayed){
    displayLoadingMessage();  
  } else {
    if (!mapGenerated) {
      generateEasyStaticMap();
      mapGenerated = true;
    }
    drawStartMenu();
    checkWinOrLose();
  }
}

void drawStartHardMap() {
  chosenLevel = "Hard";
  if (!loadingMessageDisplayed) {
    displayLoadingMessage();
  } else {
    if (!mapGenerated) {
      generateHardStaticMap();
      mapGenerated = true;
    }
    drawStartMenu();
    checkWinOrLose();
  }
}


void drawStartMenu(){
  //swValue= digitalRead(swPin);
   //Serial.print("SW Pin Value: ");
 // Serial.print(swValue);
  if(gameOver == false){
    showOnLcdDuringGameplay();
  }
  updatePlayerPosition(xValue, yValue);
  updatePlayerBlink();
  // Print joystick button state for debugging
  if (currentSWValue == 1 && previousSWValue == 0 && !playerIsDead) {
    Serial.println("Joystick Button Pressed");
    if (!bombIsActive) {
      placeBomb();
      bombIsActive = true;  // Set the flag to indicate an active bomb
    }
  }
  updateBombs();
  displayGameBoard();
  if (newHighScore) {
    saveHighScoresToEEPROM();
    displayHighScores();
    newHighScore = false;
  }
  delay(100);
}

void showOnLcdDuringGameplay(){
   lcd.setCursor(0, 0);
   lcd.print("Lives:");
   lcd.print(playerLives);
   lcd.write(HEART); 
   lcd.setCursor(0, 1);
   lcd.print("Score:");
   lcd.print(destroyedWalls);

   lcd.setCursor(9, 0);
   lcd.print("L:");
   lcd.print(chosenLevel);
   if(gameOver==true){
    lcd.clear();
   }
}
//display message on lcd and animation for winning/losing situation
void checkWinOrLose(){
  if (playerIsDead && !deathAnimationActive && playerLives == 0) {
    playAnimation(loseImages, sizeof(loseImages) / sizeOfElement);
    updateHighScores(destroyedWalls);
    gameOver = true;
    deathAnimationActive = true;
    lcd.setCursor(0, 0);
    lcd.print(" You died!      ");
    lcd.setCursor(10, 0);
    lcd.write(SAD_FACE);
    lcd.setCursor(0, 1);
    lcd.print(" Exit!");
    lcd.print(" <");
  }

  allWallsDestroyed = isMissionComplete();
  if (allWallsDestroyed && !playerIsDead && playerWins && !deathAnimationActive && !winAnimationActive) {
    playAnimation(winImages, sizeof(winImages) / sizeOfElement);
    updateHighScores(destroyedWalls);
    winAnimationActive = true;
    allWallsDestroyed = false;
    gameOver = true;
    //displayClearBoard();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print(" Exit!");
    lcd.print(" <");  
  }
  if (currentSWValue == 1 && previousSWValue != currentSWValue && gameOver == true) {
    resetParameters();
    menuState = MAIN_MENU;
    mainMenuState == UPPER_MAIN_MENU;
    displayClearBoard();
    lcd.clear();
  }
    previousSWValue = currentSWValue;
}

void displayWelcomeMessage() {
 
  const char* line1 = "LET THE";
  const char* line2 = "EXPLOSIONS BEGIN";
  unsigned long startTime = 0;
  unsigned long currentTime = millis();
  if(currentTime - startTime < introDuration){
    //lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);  
    lcd.print(line2);
    displayBombImage();
  }
  else{
    lcd.clear();
    introMessage = true;
    displayClearBoard();
  }
}
 
void displayLoadingMessage() {
  const char* line1 = "Loading";
  const char* line2 = " ... ";
  unsigned long currentTime = millis(); 
  static unsigned long startTimeLoading = 0;  // Static variable to store the start time
  
  // Check if the loading message hasn't been displayed yet
  if (!loadingMessageDisplayed) {
    // Check if it's the first time entering this block
    if (startTimeLoading == 0) {
      startTimeLoading = currentTime;  // Record the start time
    }

    if (currentTime - startTimeLoading < loadingDuration) {
      lcd.setCursor(4, 0);
      lcd.print(line1);
      lcd.setCursor(5, 1);
      lcd.print(line2);
      displayBombImage();
     
    } else {
      lcd.clear();
     loadingMessageDisplayed = true;
     startTimeLoading=0;
    }
  }
}

void drawUpperMainMenu() {
  // title
  lcd.setCursor(3, 0);
  lcd.print("Game Menu!");

  // items
  lcd.setCursor(0, 1);
  lcd.print("Start game");
  if (mainMenuSelectedValue == ITEM_START)
    lcd.print(" <");
    
  // scroll arrow
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);
  if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    lcd.clear();
    mainMenuState = LOWER_MAIN_MENU;
    mainMenuSelectedValue = ITEM_SETTINGS;
  } else {
    yPressed = 0;
  }
}

void drawLowerMainMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Settings");
  if (mainMenuSelectedValue == ITEM_SETTINGS) {
    lcd.print(" <");
  }
    
  lcd.setCursor(0, 1);
  lcd.print("Highscore");
  if (mainMenuSelectedValue == ITEM_HIGHSCORE) {
    lcd.print(" <");
  }
  // scroll arrow
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    //goes back to upper menu
    if (mainMenuSelectedValue == ITEM_SETTINGS) {
      lcd.clear();
      mainMenuState = UPPER_MAIN_MENU;
      mainMenuSelectedValue = ITEM_START;
      
    } else if (mainMenuSelectedValue == ITEM_HIGHSCORE) {
        lcd.setCursor(10, 1);
        lcd.print(BLANK);
        mainMenuSelectedValue = ITEM_SETTINGS;
    }
   }
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      //goes to third menu
      if (mainMenuSelectedValue == ITEM_HIGHSCORE) {
        mainMenuState = THIRD_MAIN_MENU;
        mainMenuSelectedValue = ITEM_ABOUT;
        lcd.clear();
      } else {
        lcd.setCursor(9, 0);
        lcd.print(BLANK);
        mainMenuSelectedValue = ITEM_HIGHSCORE;
      }
    
   } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
      yPressed = 0;
    }
}

void drawThirdMainMenu() {
  // items
  lcd.setCursor(0, 0);
  lcd.print("About");
  if (mainMenuSelectedValue == ITEM_ABOUT) {
    lcd.print(" <");
  }
  // scroll arrow
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);

  lcd.setCursor(0, 1);
  lcd.print("How to play");
  if (mainMenuSelectedValue == ITEM_HOWTOPLAY) {
    lcd.print(" <");
  }

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
    if (mainMenuSelectedValue == ITEM_ABOUT){
        yPressed = 1;
        lcd.clear();
        mainMenuState = LOWER_MAIN_MENU;
        mainMenuSelectedValue = ITEM_HIGHSCORE;
    } else if (mainMenuSelectedValue = ITEM_HOWTOPLAY) {
      lcd.setCursor(12, 1);
      lcd.print(BLANK);
      mainMenuSelectedValue = ITEM_ABOUT;
  }
     
  } else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (mainMenuSelectedValue == ITEM_ABOUT) {
        mainMenuSelectedValue = ITEM_HOWTOPLAY;
        lcd.setCursor(6, 0);
        lcd.print(BLANK);
      } 
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }
}

void drawUpperSettingsMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");
  
  if (settingsSelectedMenu == ITEM_SETTINGS_BACK) {
    lcd.print(" <");
  }

  lcd.setCursor(0, 1);
  lcd.print("LCD contrast");
  if (settingsSelectedMenu == ITEM_SETTINGS_LCD) {
    lcd.print(" <");
  }
  // scroll arrow
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    
    if (settingsSelectedMenu ==  ITEM_SETTINGS_LCD) {
      //lcd.clear();
      lcd.setCursor(13, 1);
      lcd.print(BLANK);
      settingsSelectedMenu  = ITEM_SETTINGS_BACK;
    } 
   } 
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (settingsSelectedMenu == ITEM_SETTINGS_LCD) {
          settingsSubMenuState = LOWER_SETTINGS_MENU;
          settingsSelectedMenu = ITEM_SETTINGS_MATRIX;
          lcd.clear();
        } else if (settingsSelectedMenu == ITEM_SETTINGS_BACK){
          lcd.setCursor(6, 0);
          lcd.print(BLANK);
          settingsSelectedMenu= ITEM_SETTINGS_LCD;
        }
  
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  } 
}

void drawLowerSettingsMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Matrix bright.");
  if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX) {
    lcd.print("<");
  }
  lcd.setCursor(0, 1);
  lcd.print("LCD bright.");
  if (settingsSelectedMenu == ITEM_SETTINGS_LCD_B) {
    lcd.print(" <");
  } 
  // scroll arrow
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
    
    if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX) {
      lcd.clear();
      settingsSubMenuState = UPPER_SETTINGS_MENU;
      settingsSelectedMenu = ITEM_SETTINGS_LCD;
      
    } 
    else if (settingsSelectedMenu == ITEM_SETTINGS_LCD_B) {
      settingsSelectedMenu = ITEM_SETTINGS_MATRIX;
      lcd.setCursor(12, 1);
      lcd.print(BLANK);
    } 
   }
   else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
    
    if (settingsSelectedMenu == ITEM_SETTINGS_MATRIX) {
      settingsSelectedMenu = ITEM_SETTINGS_LCD_B;
      lcd.setCursor(14, 0);
      lcd.print(BLANK);
    } 
    else if (settingsSelectedMenu == ITEM_SETTINGS_LCD_B) {
      lcd.clear();
      settingsSubMenuState = THIRD_SETTINGS_MENU;
      settingsSelectedMenu = ITEM_SETTINGS_BUZZER;
    }
   }
    else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
      yPressed = 0;
    }
}

void drawThirdSettingsMenu(){
  lcd.setCursor(0, 0);
  lcd.print("Sound ON/OFF");
  if (settingsSelectedMenu == ITEM_SETTINGS_BUZZER) {
    lcd.print(" <");
  } 
  lcd.setCursor(15, 1);
  lcd.write(UP_ARROW);

  lcd.setCursor(0, 1);
  lcd.print("Reset scores");
  if (settingsSelectedMenu == ITEM_SETTINGS_RESET) {
    lcd.print(" <");
  } 

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
    if (settingsSelectedMenu ==  ITEM_SETTINGS_BUZZER) {
      lcd.clear();
      settingsSubMenuState = LOWER_SETTINGS_MENU;
      settingsSelectedMenu = ITEM_SETTINGS_LCD_B;
    } else if(settingsSelectedMenu ==  ITEM_SETTINGS_RESET){
      lcd.clear();
      lcd.setCursor(13, 1);
      lcd.print(BLANK);
      settingsSelectedMenu = ITEM_SETTINGS_BUZZER;
    }
  } else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
    
    if (settingsSelectedMenu == ITEM_SETTINGS_BUZZER) {
      settingsSelectedMenu = ITEM_SETTINGS_RESET;
      lcd.setCursor(13, 0);
      lcd.print(BLANK);
    } 
   
   } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  } 
}


void drawLCDContrast() {
  
  lcd.setCursor(5, 0);
  lcd.print("Move:");
  lcd.setCursor(4, 1);
  lcd.print("UP");
  lcd.setCursor(6, 1);
  lcd.write(INCREASE_ARROW);
  lcd.setCursor(7, 1);
  lcd.print(" DOWN");
  lcd.setCursor(12, 1);
  lcd.write(DECREASE_ARROW);

  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    
    lcdContrastValue = constrain(lcdContrastValue - contrastStep, minContrastValue, maxContrastValue);
    updateLcdContrast();
  } else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    lcdContrastValue = constrain(lcdContrastValue + contrastStep, minContrastValue, maxContrastValue);
    updateLcdContrast();
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }

  if (currentSWValue == 1 && previousSWValue != currentSWValue) {
    // Save the selected brightness level to EEPROM
    EEPROM.write(lcdContrastEEPROMAddress, lcdContrastValue);
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = UPPER_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_LCD;
  }
  previousSWValue = currentSWValue;
}

void updateLcdContrast() {
  if (lcdContrastValue <= lowBrightness) {
    analogWrite(lcdContrastPin, map(lcdContrastValue, 1, lowContrastValue, lowestContrastValueMap, lowerContrastValueMap));
  } else if (lcdContrastValue <= mediumBrightness) {
    analogWrite(lcdContrastPin, map(lcdContrastValue, lowContrastValue + 1, mediumContrastValue, lowerContrastValueMap, mediumContrastValueMap));
  } else {
    analogWrite(lcdContrastPin, map(lcdContrastValue, mediumContrastValue + 1, highContrastValue, mediumContrastValueMap, highestContrastValueMap));
  }
}


void drawMatrixBrightness() {
  lcd.setCursor(5, 0);
  lcd.print("Move:");

  lcd.setCursor(4, 1);
  lcd.print("UP");
  lcd.setCursor(6, 1);
  lcd.write(INCREASE_ARROW);
  lcd.setCursor(7, 1);
  lcd.print(" DOWN");
  lcd.setCursor(12, 1);
  lcd.write(DECREASE_ARROW);

  // Adjust matrix brightness based on joystick input
  if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    matrixBrightnessValue = constrain(matrixBrightnessValue - brightnessStep, minBrightness, maxBrightness);
    lc.setIntensity(0, matrixBrightnessValue);
  } else if (yValue < lowerThreshold&& !yPressed) {
    yPressed = 1;
    matrixBrightnessValue = constrain(matrixBrightnessValue + brightnessStep, minBrightness, maxBrightness);
    lc.setIntensity(0, matrixBrightnessValue);
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }

  // Save matrix brightness level to EEPROM when the joystick button is pressed
  if (currentSWValue == 1 && previousSWValue != currentSWValue) {
    EEPROM.write(matrixBrightnessEEPROMAddress, matrixBrightnessValue);
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = LOWER_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_MATRIX;
  }

  previousSWValue = currentSWValue;
}

void drawLCDBrightness() {
  lcd.setCursor(5, 0);
  lcd.print("MOVE:");

  lcd.setCursor(4, 1);
  lcd.print("UP");
  lcd.setCursor(6, 1);
  lcd.write(INCREASE_ARROW);
  lcd.setCursor(7, 1);
  lcd.print(" DOWN");
  lcd.setCursor(12, 1);
  lcd.write(DECREASE_ARROW);

  // Adjust LCD brightness based on joystick input
  if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    lcdBrightnessValue = constrain(lcdBrightnessValue - brightnessStepLcd, minBrightnessLcd, maxBrightnessLcd);
    updateLCDBrightness();
  } else if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    lcdBrightnessValue = constrain(lcdBrightnessValue + brightnessStepLcd, minBrightnessLcd, maxBrightnessLcd);
    updateLCDBrightness();
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
    yPressed = 0;
  }

  // Save LCD brightness level to EEPROM when the joystick button is pressed
  if (currentSWValue == 1 && previousSWValue != currentSWValue) {
    EEPROM.write(lcdBrightnessEEPROMAddress, lcdBrightnessValue);
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = LOWER_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_LCD_B;
  }
  previousSWValue = currentSWValue;
}

void updateLCDBrightness() {
   analogWrite(LCDbrightnessPin, lcdBrightnessValue);
}

void drawSoundOnOff() {
  lcd.setCursor(0, 0);
  lcd.print("On");
  if (settingsSubmenuSound == SOUND_ON){
    lcd.write(CHECK);
    lcd.print(" <");
  }
  lcd.setCursor(0, 1);
  lcd.print("Off");
  if (settingsSubmenuSound == SOUND_OFF){
    lcd.write(CHECK);
    lcd.print(" <");
  }
  if (yValue < lowerThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (settingsSubmenuSound == SOUND_OFF) {
        //lcd.clear();
         lcd.setCursor(3, 1);
        lcd.print(BLANK);
        lcd.setCursor(5, 1);
        lcd.print(BLANK);
        settingsSubmenuSound = SOUND_ON;
      } 
   } 
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (settingsSubmenuSound == SOUND_ON) {
          lcd.setCursor(2, 0);
          lcd.print(BLANK);
          lcd.setCursor(4, 0);
          lcd.print(BLANK);
          settingsSubmenuSound = SOUND_OFF;
      }

  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  }

   if (settingsSubmenuSound == SOUND_OFF && currentSWValue == 1 && previousSWValue != currentSWValue) {
    lcd.clear();
    soundOn = false;
    EEPROM.write(soundOnOffEEPROMAddress, soundOn);
    menuState = SETTINGS_MENU;
    settingsSubMenuState = THIRD_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_BUZZER;
    
  } else if(settingsSubmenuSound == SOUND_ON && currentSWValue == 1 && previousSWValue != currentSWValue){
    lcd.clear();
    soundOn = true;
    EEPROM.write(soundOnOffEEPROMAddress, soundOn);
    menuState = SETTINGS_MENU;
    settingsSubMenuState = THIRD_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_BUZZER;
  }
  previousSWValue = currentSWValue;

}

void drawUpperHighscoresMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(BACK_ARROW);
  lcd.print("Back");
  
  if (highscoresSelectedMenu == ITEM_HIGHSCORES_BACK) {
    lcd.print(" <");
  }
  lcd.setCursor(15, 1);
  lcd.write(DOWN_ARROW);
  lcd.setCursor(0,1);
  lcd.print("1. ");
  lcd.print(highScores[0]);
  if (highscoresSelectedMenu == ITEM_HIGHSCORES_1) {
    lcd.setCursor(7, 1);
    lcd.print("<");
  }
  
  if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    
    if ( highscoresSelectedMenu ==  ITEM_HIGHSCORES_1) {
      //lcd.clear();
      lcd.setCursor(7, 1);
      lcd.print(BLANK);
      highscoresSelectedMenu = ITEM_HIGHSCORES_BACK;
    } 
   } 
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (highscoresSelectedMenu == ITEM_HIGHSCORES_1) {
          highscoresSubMenuState = LOWER_HIGHSCORES_MENU;
          highscoresSelectedMenu = ITEM_HIGHSCORES_2;
          lcd.clear();
        } else if (highscoresSelectedMenu == ITEM_HIGHSCORES_BACK){
          lcd.setCursor(6, 0);
          lcd.print(BLANK);
          highscoresSelectedMenu= ITEM_HIGHSCORES_1;
        }
  
  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  } 
  
}

void drawLowerHighscoresMenu() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2. ");
  lcd.print(highScores[1]);
  if (highscoresSelectedMenu == ITEM_HIGHSCORES_2) {
    lcd.setCursor(7, 0);
    lcd.print("<");
  }
  lcd.setCursor(15, 0);
  lcd.write(UP_ARROW);
  lcd.setCursor(0,1);
  lcd.write("3. ");
  lcd.print(highScores[2]);
  if (highscoresSelectedMenu == ITEM_HIGHSCORES_3) {
    lcd.setCursor(7, 1);
    lcd.print("<");
  }
   if (yValue < lowerThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    
    if (highscoresSelectedMenu == ITEM_HIGHSCORES_2) {
      lcd.clear();
      highscoresSubMenuState = UPPER_HIGHSCORES_MENU;
      highscoresSelectedMenu = ITEM_HIGHSCORES_1;

    } 
    else if (highscoresSelectedMenu == ITEM_HIGHSCORES_3) {
      highscoresSelectedMenu = ITEM_HIGHSCORES_2;
      lcd.setCursor(7, 1);
      lcd.print(BLANK);
    } 
   }
   else if (yValue > upperThreshold && !yPressed) {
    yPressed = 1;
    if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
    }
    
    if (highscoresSelectedMenu == ITEM_HIGHSCORES_2) {
      highscoresSelectedMenu = ITEM_HIGHSCORES_3;
      lcd.setCursor(7, 0);
      lcd.print(BLANK);
    } 
   }
    else if (yValue >= lowerThreshold && yValue <= upperThreshold) {
      yPressed = 0;
    }

}
void drawResetHighscores(){
  lcd.setCursor(0, 0);
  lcd.print("Yes");
  if (settingsSubmenuReset == RESET_HIGHSCORES_YES){
    lcd.write(CHECK);
    lcd.print(" <");
  }
  lcd.setCursor(0, 1);
  lcd.print("No");
  if (settingsSubmenuReset == RESET_HIGHSCORES_NO){
    lcd.write(CHECK);
    lcd.print(" <");
  }
  if (yValue < lowerThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (settingsSubmenuReset == RESET_HIGHSCORES_NO) {
        //lcd.clear();
         lcd.setCursor(2, 1);
        lcd.print(BLANK);
        lcd.setCursor(4, 1);
        lcd.print(BLANK);
        settingsSubmenuReset = RESET_HIGHSCORES_YES;
      } 
   } 
   else if (yValue > upperThreshold && !yPressed) {
      yPressed = 1;
      if(soundOn){
        tone(buzzerPin, swithcingThroughMenuDuration, swithcingThroughMenuFrequency);
      }
      if (settingsSubmenuReset == RESET_HIGHSCORES_YES) {
          lcd.setCursor(3, 0);
          lcd.print(BLANK);
          lcd.setCursor(5, 0);
          lcd.print(BLANK);
          settingsSubmenuReset = RESET_HIGHSCORES_NO;
      }

  } else if (yValue >= lowerThreshold && yValue <= upperThreshold){
    yPressed = 0;
  }

   if (settingsSubmenuReset == RESET_HIGHSCORES_NO && currentSWValue == 1 && previousSWValue != currentSWValue) {
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = THIRD_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_RESET;
    
  } else if(settingsSubmenuSound == RESET_HIGHSCORES_YES && currentSWValue == 1 && previousSWValue != currentSWValue){
    resetHighScores();
    saveHighScoresToEEPROM();
    lcd.clear();
    menuState = SETTINGS_MENU;
    settingsSubMenuState = THIRD_SETTINGS_MENU;
    settingsSelectedMenu = ITEM_SETTINGS_RESET;
  }
  previousSWValue = currentSWValue;
}

void resetHighScores(){
  for (int i = 0; i < maxHighScores; ++i) {
        highScores[i] = 0;
    }
}

void drawAboutMenu() {
  const int delayOnScroll = 350;
  const int printCharacters = 30;
  static unsigned long lastScrollTime = 0;
  bool showAboutImage = false;
  lcd.setCursor(4,0);
  lcd.print("Title: Bomberman");

  lcd.setCursor(1, 1);
  lcd.print("Creator: Razvan, Git: CaruntuRazvan");
  if(!showAboutImage){
    displayAboutImage();//for matrix
    showAboutImage = true;
  }
    
  if (millis() - lastScrollTime >= delayOnScroll) {
    lcd.scrollDisplayLeft();
    lastScrollTime = millis();
  }

  if (currentSWValue == 1 && previousSWValue != currentSWValue && showAboutImage == true) {
    lcd.clear();
    menuState = MAIN_MENU;
    showAboutImage= false;
    displayClearBoard();
  }
  previousSWValue = currentSWValue;
}

void drawHowToPlayMenu(){
  bool showHTPImage = false;
    if(!showHTPImage){
    displayHowToPlayImage();//for matrix
    showHTPImage = true;
  }
    if (HTPscrollText == 1 && scrollTextPosition < messageHTP.length() - 15) {
      lcd.setCursor(0, 0);
      lcd.print("Read:");
      lcd.setCursor(0, 1);
      if (millis() - prevScrollTime >= delayOnScroll) {
        prevScrollTime = millis();
        lcd.setCursor(0, 1);
        int remainingChars = messageHTP.length() - scrollTextPosition;
        int substringLength = min(16, remainingChars);
        lcd.print(messageHTP.substring(scrollTextPosition, scrollTextPosition + substringLength));
        scrollTextPosition++;
      }
  }
   if (currentSWValue == 1 && previousSWValue != currentSWValue && showHTPImage == true) {
    lcd.clear();
    menuState = MAIN_MENU;
    mainMenuSelectedValue = ITEM_HOWTOPLAY;
    HTPscrollText = 0 ;
    showHTPImage= false;
    scrollTextPosition = 0;
    displayClearBoard();
    
  }
  previousSWValue = currentSWValue;  
}

// Function to check if the mission is complete
bool isMissionComplete() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (gameBoard[row][col] == 1 && !isIndestructibleWall(row, col)) {
        return false;
      }
    }
  }

  bombIsActive = false;
  bombs[lastIndex].explosionTime = 0;
  digitalWrite(redLedPin, LOW);
  playerRow = -1;
  playerCol = -1;
  playerWins = true; 
  displayClearBoard();
  return true;
}

void generateEasyStaticMap() {
  int staticMap[numRows][numCols] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 0, 1, 1, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  };

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      gameBoard[row][col] = staticMap[row][col];
      lc.setLed(0, row, col, gameBoard[row][col]);
    }
  }

  playerRow = 1;
  playerCol = 1;
  lc.setLed(0, playerRow, playerCol, playerBlinkState);
}

void generateHardStaticMap() {
  int staticMap[numRows][numCols] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
  };

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      gameBoard[row][col] = staticMap[row][col];
      lc.setLed(0, row, col, gameBoard[row][col]);
    }
  }

  playerRow = 1;
  playerCol = 1;
  lc.setLed(0, playerRow, playerCol, playerBlinkState);
}

bool isIndestructibleWall(int row, int col) {
  return row == 0 || row == numRows - 1 || col == 0 || col == numCols - 1;
}

void updatePlayerPosition(int xValue, int yValue) {
  static int lastXValue = xValue;
  static int lastYValue = yValue;
  static unsigned long lastUpdateTime = millis();
  const int sensibilityThreshold = 20;
  
  if (millis() - lastUpdateTime >= sensibilityThreshold) {
    //move left or right
    if (xValue < lowerThreshold && playerCol > 0 && !gameBoard[playerRow][playerCol - 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerCol;
    } else if (xValue > upperThreshold && playerCol < numCols - 1 && !gameBoard[playerRow][playerCol + 1]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerCol;
    }
    //move up and down
    if (yValue < lowerThreshold && playerRow > 0 && !gameBoard[playerRow - 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      --playerRow;
    } else if (yValue > upperThreshold && playerRow < numRows - 1 && !gameBoard[playerRow + 1][playerCol]) {
      lc.setLed(0, playerRow, playerCol, 0);
      ++playerRow;
    }
    
    lastXValue = xValue;
    lastYValue = yValue;
    lastUpdateTime = millis();
  }
}
//blinking player
void updatePlayerBlink() {
  static unsigned long lastBlinkTime = 0;
  if (millis() - lastBlinkTime >= playerBlinkRate) {
    playerBlinkState = !playerBlinkState;
    lastBlinkTime = millis();
  }
}

void placeBomb() {
  for (int i = 0; i < maxBombs; ++i) {
    if (bombs[i].explosionTime == 0) {
      bombs[i].row = playerRow;
      bombs[i].col = playerCol;
      bombs[i].blinkState = false;
      bombs[i].explosionTime = millis() + bombExplosionTime;
      lc.setLed(0, bombs[i].row, bombs[i].col, bombs[i].blinkState);
      if(soundOn){
        tone(buzzerPin, placingBombFrequency, placingBombSoundDuration);
      }
      break;
    }
  }
}

void updateBombs() {
  // Iterate through all active bombs
  for (int i = 0; i < maxBombs; ++i) {
    if (bombs[i].explosionTime > 0) {
      unsigned long lastBlinkTime = bombs[i].blinkState;

      // Check if it's time to toggle the bomb's blink state
      if (millis() - lastBlinkTime >= bombBlinkRate) {
        // Toggle the blink state
        bombs[i].blinkState = !bombs[i].blinkState;
        lc.setLed(0, bombs[i].row, bombs[i].col, bombs[i].blinkState);

        // Update the last blink time for the current bomb
        lastBlinkTime = millis();
      }

      // Check if it's time for the bomb to explode
      if (millis() >= bombs[i].explosionTime) {
        // Call the function to handle the bomb explosion
        explodeBomb(i);
        
      } else {
        // Toggle the red LED for a blinking effect during bomb countdown
        digitalWrite(redLedPin, millis() % (twoNumber * bombBlinkRate) < bombBlinkRate);    
      }
    }
  }
}

void displayGameBoard() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      if (row == playerRow && col == playerCol) {
        lc.setLed(0, row, col, playerBlinkState);
      } else {
        lc.setLed(0, row, col, gameBoard[row][col] == 1);
      }
    }
  }
}

// Function to explode walls in a specific direction
void explodeInDirection(int row, int col, int rowChange, int colChange, int direction) {
  for (int i = 0; i < explosionRange; ++i) {
    row += rowChange;
    col += colChange;
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
      if (gameBoard[row][col] == 1 && !isIndestructibleWall(row, col)) {
        gameBoard[row][col] = 0;
        lc.setLed(0, row, col, 0);
        destroyedWalls++;
      }
    } else {
      break;
    }
  }
}

// explode walls in all directions
void explodeWalls(int index) {
  explodeInDirection(bombs[index].row, bombs[index].col, -1, 0, 0);  // Up
  explodeInDirection(bombs[index].row, bombs[index].col, 1, 0, 1);   // Down
  explodeInDirection(bombs[index].row, bombs[index].col, 0, -1, 2);  // Left
  explodeInDirection(bombs[index].row, bombs[index].col, 0, 1, 3);   // Right
}

//check if the player is in the explosion range
bool isPlayerInExplosionRange(int index) {
  int rowDiff = abs(playerRow - bombs[index].row);
  int colDiff = abs(playerCol - bombs[index].col);
  return (rowDiff == 0 && colDiff <= explosionRange) || (colDiff == 0 && rowDiff <= explosionRange);
}

//handle player death and clear the game board
void handlePlayerDeath() {
  if (playerLives > 0) {
    playerLives--;
    showOnLcdDuringGameplay();
  }

  if(playerLives == 0){
    playerRow = -1;
    playerCol = -1;
    playerIsDead = true;
    displayClearBoard();
  }
}

// Main explodeBomb function
void explodeBomb(int index) {
  explodeWalls(index);
  // Check if the player is in the explosion range
  if (isPlayerInExplosionRange(index)) {
    handlePlayerDeath();
  }
  if(soundOn){
    tone(buzzerPin, explosionSoundFrequency , explosionSoundDuration);
  }
 
  digitalWrite(redLedPin, LOW);
  bombs[index].explosionTime = 0;
  lastIndex = index;
  bombIsActive = false; // Reset the bomb flag
  showOnLcdDuringGameplay();
}

void updateHighScores(int score) {
  bool scoreAlreadyExists = false;

  // Check if the score is already in the top 3
  for (int i = 0; i < maxHighScores; ++i) {
    if (score == highScores[i]) {
      scoreAlreadyExists = true;
      break;
    }
  }
  if (!scoreAlreadyExists) {
    for (int i = 0; i < maxHighScores; ++i) {
      if (score > highScores[i]) {
        // Insert the new high score and shift the existing scores
        for (int j = maxHighScores - 1; j > i; --j) {
          highScores[j] = highScores[j - 1];
        }
        highScores[i] = score;
        newHighScore = true;
        break;
      }
    }
  }
}
//for serial monitor
void displayHighScores() {
  Serial.println("Top 3 High Scores:");
  for (int i = 0; i < maxHighScores; ++i) {
    Serial.print("Rank ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(highScores[i]);
  }
}

void saveHighScoresToEEPROM() {
  // Save each high score to EEPROM
  EEPROM.put(highScoresEEPROMAddress, highScores);
}

void loadHighScoresFromEEPROM() {
  // Load each high score from EEPROM
   EEPROM.get(highScoresEEPROMAddress, highScores);
}

void playAnimation(const uint64_t animationImages[], int imagesLen){
  const int animationSpeed = 500;  // 0.5 seconds in milliseconds
  const unsigned long animationDuration = 1500;  // 1.5 seconds in milliseconds
  const int numFrames = animationDuration / animationSpeed;
  for (int frame = 0; frame < numFrames; ++frame) {
    for (int i = 0; i < imagesLen; ++i) {
      for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
          int bit = (animationImages[i] & (1ULL << (col + row * numCols))) ? 1 : 0;
          lc.setLed(0, row, col, bit);
        }
      }
      delay(animationSpeed);
    }
  }
}

void displayBombImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (bombImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayEasyImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (easyImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayHardImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (hardImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayAboutImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (aboutImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayHowToPlayImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (howToPlayImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayMainMenuImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (mainMenuImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}
void displaySettingsImage() {
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (settingsImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayHighscoresImage(){
  // Iterate through the image data and set LEDs accordingly
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      int bit = (highscoresImage[row] & (1ULL << col + row * numCols)) ? 1 : 0;
      lc.setLed(0, row, col, bit);
    }
  }
}

void displayClearBoard() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
        gameBoard[row][col] = 0;
        lc.setLed(0, row, col, 0);
      } 
    }
}

void resetParameters() {
  mapGenerated = false;
  playerIsDead = false;
  deathAnimationActive = false;
  winAnimationActive = false;
  bombIsActive = false;
  playerLives = 2;
  playerBlinkState = false;
  gameOver=false;
  allWallsDestroyed = false;
  loadingMessageDisplayed = false;
  destroyedWalls = 0;
  playerWins = false;
} 
