# _IntroductionToRobotics Matrix Project_
# Bomberman w/Radiation

<details>
  <summary>How to play</summary>
  It is inspired from the original Bomberman game. The concept of rooms is implemented, which means that at each level, you unlock a room (total no. of 4 rooms).
  
  * Levels:
    * Level 1: You just have to explode each bomb. If you are in the exploding diststance, the game ends.
    * Level 2: Now, an extra enemy is on the screen and he can kill the player.
    * Level 3: In 30 seconds you have to destroy as many walls as possible because a radiation will come. If the radiations finds the player in the room 3, he dies.
    * Level 4: There is an extra enemy; after you've blown all the walls, it displays a winning message and checks if you are in top 3.
  * Details:
    * The player can plant 2 bombs at the same time
    * The bombs that explode are the ones up, down, left and right at a distance of 1
    * If the Manhattan distance between player and bomb at the moment of explosion is 1, he gets killed by the bomb
    * The player can't access unlocked rooms. A room is unlocked when he is at a level >= roomIndex
    * To plant a bomb the player has to press the joystick button
    * The purpose is to finish the game ASAP
    * The game ends when all the walls are blown
    * The player can change his name and it will be saved to EEPROM
    
</details>
<details>
  <summary>Menu Requirements :bookmark_tabs:</summary>
  Create a menu for your game, emphasis on ‘the game. You should scroll on the LCD with the joystick. Remember you have quite a lot of flexibility here, but do not confuse that with a free ticket to slack off. The menu should include the following functionality:
  
* 1. <strong>Intro Message</strong> - When powering up a game, a greeting message should be shown for a few moments.
* 2. Should contain roughly the following categories:
* (a) <strong>Start game<strong/>, starts the initial level of your game
* (b) <strong>Highscore</strong>:
  *  Initially, we have 0.
  *  Update it when the game is done. Highest possible score should be achieved by starting at a higher level.
  *  Save the top 3+ values in EEPROM with name and score.
* (c) <strong>Settings</strong>:
  *  <strong>Enter name</strong>. The name should be shown in highscore. Maybe somewhere else, as well? You decide.
  *  <strong>LCD brightness control</strong> (mandatory, must change LED wire that’s directly connected to 5v). Save it to eeprom.
  *  <strong>Matrix brightness control</strong> (see function setIntesnity from the ledControl library). Make sure to display something on the matrix when selecting it. Save it to eeprom.
  *  <strong>Sounds on or off</strong>. Save it to eeprom.
  *  Extra stuff can include items specific to the game mechanics, or other settings such as chosen theme song etc. Again, save it to eeprom. You can even split the settings in 2: game settings and system settings.
* (d) <strong>About</strong>: should include details about the creator(s) of the game. At least game name, author and github link or user (use scrolling text?)
* (e) <strong>How to play</strong>: short and informative description
* 3. While playing the game: display all relevant info
  *  Lives
  *  Level
  *  Score
  *  Time?
  *  Player name?
  *  etc
* 4. Upon game ending:
  * Screen 1: a message such as ”Congratulations on reaching level/score X”. ”You did better than y people.” etc. Switches to screen 2 upon interaction (button press) or after a few moments.
  * Screen 2: display relevant game info: score, time, lives left etc. Must inform player if he/she beat the highscore. This menu should only be closed by the player, pressing a button.
</details>

<details>
  <summary>Game requrements :bookmark_tabs:</summary>
  The requimrements of the game:
  
* You must add basic sounds to the game (when ”eating” food, when dying, when finishing the level etc). Extra: add theme songs.
* Each level / instance should work on 16x16 matrix. You can apply the concept of visibility / fog of war (aka you only see 8x8 of the total 16x16 matrix, and you discover more as you move around) or you can use the concept of ”rooms”. Basically you will have 4 rooms that you need to go through on each level.
* It must be intuitive and fun to play.
* It must make sense in the current setup.
* You should have a feeling of progression in difficulty. Depending on the dynamic of the game, this is done in the same level or with multiple levels. You can make them progress dynamically or have a number of fixed levels with an endgame. Try to introduce some
randomness, though.
  
</details>

<details>
  <summary> Components :electric_plug:</h2> </summary>
  The components:
* 1xLCD  
  
* 1x joystick
  
* 1x 8x8 matrix
  
* 1x MAX7129
  
* 1x buzzer
  
* 1x potentiometer

*  resistors and wires

</details>

<details>
  <summary> Final Thoughts </summary>
  It was harder than expected to me, but i feel like doing this project helped me a lot. Even though all the functionalities are not perfect and may have bugs, I learned that modularisation of the code is essential. In a future projects i will learn from my mistakes. For example, at the beginning I added a single bomb. When i wanted to have multiple bombs, it was really hard for me to change the code and it took me hours. If i would've written a class at the beginning it would've been easier. All in all, I think this game makes me look smart and it is a pleasure to be able to work on things that people can use phisically.
</details>

<details>
  <summary>Picture</summary>
  ![image](https://github.com/stefanbrb10/arduino-bomberman/assets/35970743/a597ccaa-8b8c-4d13-a9b2-4fe316ad75ae)
</details>

<details>
  <summary>Video</summary>
  https://youtu.be/DVHCwYjfbso?si=O7dBzvCt1A7xGhVd
</details>
  
</details>
