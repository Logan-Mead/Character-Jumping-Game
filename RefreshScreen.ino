//creating what ther hero will look like
byte hero[8] = {
  B00110,
  B00110,
  B00000,
  B01110,
  B00111,
  B00110,
  B00101,
};

//creating what the obstcale will look like
byte obstcale[8]{
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
};

XY prevHeroLocation;                                                                     //create a type strut for prev herolocation and obstcale location as well as obstcale location
XY obstcaleLocation;
XY prevObstcaleLocation;
enum RefreshStates {refreshstart,draw, gameover, win};                                   //creating state name
RefreshStates refreshState;
int score = 0;                                                                           //score
int winscore = 15;                                                                       //win condition

void setupRefreshScreen(){                                                               //setup refreshScreen
  lcd.begin(16, 2);                                                                      //begin the lcd screen
  obstcaleLocation.x = 15;                                                               //start obstcale at (15,1)
  obstcaleLocation.y = 1;
  lcd.createChar(0, hero);                                                               //create hero with the bits
  lcd.createChar(1, obstcale);                                                           //create obsctale with bits
}

void loopRefreshScreen(){                                                                //runs function with the state transtions and actions
 tickRefresh();
}

void tickRefresh(){
  //State transitions
  switch(refreshState){
    case refreshstart:                                                                  
      refreshState = draw;                                                              //goes to draw state
      break;

    case draw:
      if(HeroLocation.x == obstcaleLocation.x && HeroLocation.y == obstcaleLocation.y){           //if both x and y cord of HeroLocation and ObstcaleLocation go to gameOver state
        refreshState = gameover;
      }
      else if(obstcaleLocation.x == 0){                                                //else if obsctale location is at x = 0
        obstcaleLocation.x = 15;                                                       //sets obsctaleLocation.x to 15
        score++;                                                                       //increase score
        refreshState = draw;                                                           //goes back to draw state
      }
      else if(winscore == score){                                                     //if winscore and score equal go to win state
        refreshState = win;
      }
      else
      refreshState = draw;                                                            //goes back to draw state
      break;

    case gameover:                                                                    //break
      break;

    case win:                                                                         //break
      break;  
  }
  //State Actions
  switch(refreshState){
    case refreshstart:                                                                //break
      break;

    case draw:
      drawHero();                                                                    //draws hero and obstcale
      drawObstcale();
      delay(250);                                                                   //delay 250ms
      eraseHero();                                                                  //erases prev hero and obsctale
      eraseObstcale();
      break;

    case gameover:
      gameOver();                                                                  //runs gameOver function
      break;

    case win:
      winGame();                                                                  //runs winGame function
      break;   
  }
}

void eraseHero(){                                                         //function to erase Hero
  lcd.setCursor(prevHeroLocation.x, prevHeroLocation.y);                  //sets cursor to prevHeroLocation and prints a blank space
  lcd.print(" ");
}

void drawHero(){                                                          //function to draw Hero
  lcd.setCursor(HeroLocation.x, HeroLocation.y);                          //sets cursor to current HeroLocation
  lcd.write(byte(0));                                                     //writes the byte 0 a.k.a. the Hero
  prevHeroLocation = HeroLocation;                                        //sets prevHeroLocation to current HeroLocation
}

void drawObstcale(){                                                      //function to draw obsctale
  lcd.setCursor(obstcaleLocation.x, obstcaleLocation.y);                  //sets cursor at current obsctale location
  lcd.write(byte(1));                                                     //write the byte 1 a.k.a. the obsctale
  prevObstcaleLocation = obstcaleLocation;                                //sets prevObsctaleLocation to current obsctaleLocation
  obstcaleLocation.x--;                                                   //subs one to current obsctaleLocation
}

void eraseObstcale(){
  lcd.setCursor(prevObstcaleLocation.x, prevObstcaleLocation.y);         //sets cursor to prevObsctaleLocation and prints a blank space
  lcd.print(" ");  
}

void gameOver(){                                                        //function to show gameOver state
    lcd.setCursor(3,0);                                                 //sets cursor to (3,0) and prints "GAME OVER!"
    lcd.print("GAME OVER!");
    lcd.setCursor(3,1);                                                 //sets cursor to (3,1) and prints "Score: "
    lcd.print("Score: ");
    lcd.setCursor(9,1);                                                 //sets cursor to (9,1) and prints the score
    lcd.print(score);
}

void winGame(){                                                        //function for winGame state
  lcd.setCursor(6,0);                                                  //sets cursor to (6,0) and prints "YOU"
  lcd.print("YOU");
  lcd.setCursor(6,1);                                                  //sets cursor to (6,1) and prints "WIN!"
  lcd.print("WIN!");
}
