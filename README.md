# Matrix-Project-Bomberman

<p>  This repository contains a reproduction of the bomberman game 💣, using Arduino as a project for the Introduction to Robotics Course.</p>
<details>
    <summary><h2><strong>Project description and details ℹ️</strong></h2></summary>
    <p>  This project begins by displaying the message "Let the explosions begin" on the LCD and a bomb-shaped animation on the matrix for 4 seconds as an introduction to the actual game. After this message, the main menu is displayed. It contains five key elements: Start game, Settings, Highscore, About and How to play.</p>
    <p>  When the "Start Game" option is selected, the player can choose between two variants: "Easy," which involves generating a map with fewer walls, or "Hard," which involves generating a map with more walls, occupying almost the entire surface of the map. After the player chooses the desired variant, the message "Loading..." will be displayed along with a bomb-shaped animation on the matrix for 3 seconds. After this period, the actual game will begin.</p>
    <p>  In the "Settings" category, there are various settings to control the contrast and brightness of the LCD and matrix, the sound generated by the buzzer during the game, and the ability to reset the high scores to zero. In the "Highscore" category, a kind of podium is displayed showcasing the top 3 scores recorded by the player, regardless of whether the player wins or loses. The score in this game is determined by the number of walls the player manages to destroy throughout a game.</p> 
    <p>  In the "About" category, is displayed information about the game title, the game creator, and the creator's GitHub. The "How to play" category provides a concise description to help the player understand how the game works. 
    </p>
</details>
<details>
    <summary><h2><strong>Game instructions 🎮</strong></h2></summary>
    <p>  When the actual game begins, a static map is generated with a certain number of walls based on the chosen difficulty category. The goal of this game is for the player to destroy as many walls as possible, considering that the side walls cannot be destroyed. The player is represented on the map by a blinking point.
The player can deploy a bomb by pressing the joystick, causing the bomb to detonate in one of the cardinal directions (up, down, left, or right). The range of the bomb is one unit. The time between placing the bomb and the moment of the explosion is 3 seconds. Subsequent bomb placement is allowed only after the previously deployed bomb has exploded. Throughout this process, an LED continuously blinks from the moment a bomb is placed until it explodes.If the game sound is turned on, a specific sound will be heard at the moment of bomb placement, and a different sound with a higher frequency will be played at the moment of the explosion.Throughout the game, the LCD will display the number of walls destroyed by the player, the player's remaining lives (starting with 2 lives), and also the game level.
        <p>  If the player exhausts the 2 lives, they lose the game, and the LCD will display the message 'You died.' On the matrix, an animation with the letter X appearing and disappearing three times will be shown. On the other hand, if the player manages to destroy all the walls generated on the map, the LCD will display the message 'Congratulations,' and a specific animation will appear and disappear three times on the matrix. After displaying that message, in both cases, pressing the joystick will return to the main menu.In the event that the player's score falls within the top 3 scores found in the Highscore category, this category will be updated with the new score.</p>
    </p>
</details>
<details>
    <summary><h2><strong>Matrix animations and images</strong></h2></summary>
    <ul>
    <li>Intro message and loading game matrix image</li>
    <p><img src="https://github.com/CaruntuRazvan/Matrix-Project-Bomberman/assets/115624498/81ede32b-266b-4636-b3f9-128cec56a4db" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Main menu matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Choose easy map matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Choose hard map matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Player loses matrix animation</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Player wins matrix animation</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Settings menu matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>Highscore content matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    <li>About content matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
     <li>How to play content matrix image</li>
    <p><img src="" alt="Text alternativ al imaginii" height="250" width="250"></p>
    </ul>
</details>

<details>
    <summary><h2><strong>The menu 🔗</strong></h2></summary>
    <p>  You can move through the menu using the joystick up and down. To enter an option press the joystick. If the game sound is enabled, a specific short sound will be heard when scrolling through the menu.</p>
    <p>  The options in the settings submenu that control the LCD contrast, brightness, and matrix brightness work as follows: when you select one of these three options, by moving the joystick up or down, you will increase or decrease the specific value of that setting. When you have reached the desired point, clicking the joystick will save this value, and you will exit that submenu.</p>
     <p><strong>Menu structure:</strong>
  <ul>
        <li><strong> Start game</strong>
            <ul>
                <li> Easy </li>
                <li> Hard </li>
            </ul>
        </li>
        <li><strong> Settings</strong> 🔧
            <ul>
                <li> Back (will exit to main menu)</li>
                <li> LCD contrast</li>
                <li> Matrix brightness</li>
                <li> LCD brightness</li>
                <li> Sound ON/OFF</li>
                    <ul>
                        <li> On 🔉</li>
                        <li> Off 🔇</li>
                    </ul>
                <li> Reset scores</li>
                    <ul>
                        <li> Yes</li>
                        <li> No</li>
                    </ul>
            </ul>
        </li>
        <li><strong> Highscore</strong> 🏆 (top 3 scores)</li>
        <li><strong> About</strong> (details about creator)</li>
        <li><strong> How to play</strong> (short description)</li>
  </ul>  
  </p>
</details>
<details>
    <summary><h2><strong>Electronic setup🔌</strong></h2></summary>
    <h3><strong>Setup photo 📷</strong></h3>
    <p><img src="https://github.com/CaruntuRazvan/Matrix-Project/assets/115624498/3c18702f-15e1-459e-bd7f-7d16dfb635f3" alt="Text alternativ al imaginii" height="650" width="650"></p>
    <h3><strong>Hardware components</strong></h3>
    <ul>
      <li>Arduino Uno</li>
      <li>one joystick</li>
      <li>one LCD 2x16</li>
      <li>8x8 LED Matrix</li>
      <li>a MAX7219 driver</li>
      <li>one ceramic capacitor and one electrolytic capacitor</li>
      <li>one LED</li>
      <li>one potentiometer (is not used)</li>
      <li>one buzzer</li>
      <li>wires and 4 resistors (1 with a resistance of 100 kΩ, 1 with a resistance of 330Ω for the red LED, 1 with a resistance of 220Ω for LCD and 1 with a resistance of 100Ω for buzzer)</li>
    </ul>
    <h3><strong>Used pins</strong></h3>
    <p>I tried to avoid using pins 3 and 11 at the same time because I encountered interference while attempting to solve the project.</p>
    <h4>Matrix pins:</h4>
    <ul>
      <li>din pin -> 12</li>
      <li>clock pin -> 13</li>
      <li>load pin ->2</li>
    </ul>
    <h4>LCD pins:</h4>
    <ul>
      <li>rs -> 11</li>
      <li>en -> 8</li>
      <li>d4 -> 7</li>
      <li>d5 -> 6</li>
      <li>d6 -> 5</li>
      <li>d7 -> 4</li>
    </ul>
    <h4>Joystick pins:</h4>
    <ul>
      <li>VRX pin -> A0</li>
      <li>VRY pin -> A1</li>
      <li>Sw pin -> A4</li>
    </ul>
     <h4>Other pins:</h4>
    <ul>
      <li>pin for controlling LCD brightness -> 10</li>
      <li>pin for controlling LCD contrast -> 9</li>
      <li>pin for controlling the buzzer -> A5</li>
      <li>pin for controlling the red led -> 3</li>
    </ul>   
   
</details>
<h2><strong>Demo Video 📹</strong></h2>
<p>  Watch the demo video of the game:<a href="https://www.youtube.com/shorts/vQ07wmHCx5U" target="_blank">Click here</a></p>

