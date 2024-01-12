#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>


using namespace std;
using namespace sf;






// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;






/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void handlePlayerInput(float player[], float bulletX[], float bulletY[], bool bulletExists[], Event& event, RenderWindow &window,const int MAX_BULLETS,	Music& bgMusic);
void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite);
void moveBullets(float bulletX[], float bulletY[], bool bulletExists[], Clock& bulletClock, const int MAX_BULLETS);
void drawBullets(RenderWindow& window, float bulletX[], float bulletY[], bool bulletExists[], Sprite& bulletSprite, const int MAX_BULLETS);
void generateMushrooms(int mushroomGrid[gameRows][gameColumns], int count , const int Health);
void checkCollisionswithMushroom(float bulletX[], float bulletY[], bool bulletExists[], int mushroomGrid[][gameColumns], int& score, const int MAX_BULLETS, const int MUSHROOM, Music& killMusic) ;
bool isCollisionWithMushroom(float centipedeX, float centipedeY, int mushroomGrid[][gameColumns]);
void drawCentipede(RenderWindow& window, float centipedeSegments[][3], Sprite& centipedeSprite ,Sprite& centipedefaceSprite, int centipedeLength) ;
void updateCentipedePosition(float segments[][3], Sprite& sprite, Clock& movementClock, int obstacles[][gameColumns] , const int lengthOfCentipede , bool ascend ,int &round);
bool checkPlayerCentipedeCollision(float player[], float centipedeSegments[][3], int centipedeLength) ; bool moveRight[100];
bool checkcentipedemovement(float centipedeSegments[][3] , const int centipede_length , int &round );
void SplitCentipede(RenderWindow &window, float bulletX[], float bulletY[], bool bulletExists[], float centipedeSegments[][3], const int centipede_length, int MAX_BULLETS, int mushroomGrid[][gameColumns],int &score) ;
void displayGameOverScreen(RenderWindow &window, Font &font);










int main()
{

	bool moveRight[100];
	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window( VideoMode(resolutionX, resolutionY), "Centipede Game",   Style::Resize |    Style::Titlebar | Style::Close);                                                                                                                                                   

	// Used to resize your window if it's too big or too small. Use according to your needs.
	  window.setSize(Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	//window.setSize(Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(100, 0));

	// Initializing Background Music.
	Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(20);
	//Bullet FIRE SOUND
	Music bulletfireSound;
	bulletfireSound.openFromFile("Sound Effects/fire1.wav");
	//KILL SOUND
	Music killMusic;
	killMusic.openFromFile("Sound Effects/kill.wav");
	// Initializing GameGrid.


	// Initializing Background.
	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background1.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = gameColumns  * boxPixelsY-100;
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	const int MAX_BULLETS = 10; // Maximum number of bullets
 	float bulletX[MAX_BULLETS] = {};
	float bulletY[MAX_BULLETS] = {};
	bool bulletExists[MAX_BULLETS] = {};
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = true;
	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));


	// Mushroom Initialization 
	Texture mushroomTexture;
	Sprite mushroomSprite , mushroomtornSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png"); 
  	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	mushroomtornSprite.setTexture(mushroomTexture); // Set the texture for torn mushroom sprite
	int spriteIndex = 1;
	IntRect textureRect(boxPixelsX * spriteIndex, 0, boxPixelsX, boxPixelsY);
	mushroomtornSprite.setTextureRect(textureRect);
	const int MAX_HITS = 2; // Maximum hits before mushroom is destroyed
	int mushroomGrid[gameRows][gameColumns] = {}; // Initialize the mushroomGrid 
	const int MUSHROOM = 1; 
	generateMushrooms(mushroomGrid , 30 , MAX_HITS);
    int round = 1;




    //Centipede initialization and drawing
	const int centipedeLength = 12; // Initial length of centipede
	const int MAX_CENTIPEDE_SEGMENTS = 100; // Maximum number of segments for the centipede
	const int SEGMENT_DATA = 3; // x position, y position, and exists flag // 0 , 1 , 2 respectively :) 
	float centipedeSegments[MAX_CENTIPEDE_SEGMENTS][SEGMENT_DATA];

	for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; ++i) {
    centipedeSegments[i][0] = i * boxPixelsX; // X position
    centipedeSegments[i][1] = 0; // Y position
    centipedeSegments[i][2] = 1.0; // segment exists or not . 
	}


    //Centipede  Textures initialization
	Texture centipedeTexture;
	Sprite centipedeSprite;
	centipedeTexture.loadFromFile("Textures/body.png");
	centipedeSprite.setTexture(centipedeTexture);

    //Centipede face textures
	Texture centipedeFaceTexture;
	Texture faceLeftTexture, faceRightTexture;
	centipedeFaceTexture.loadFromFile("Textures/face_left.png");
	faceRightTexture.loadFromFile("Textures/face_right.png");
	Sprite centipedeFaceSprite;	
	centipedeFaceSprite.setTexture(centipedeFaceTexture);
	centipedeFaceSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	centipedeSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	Clock centipedeClock;



    
	



    //GAMEOVER Image and sound
    bool isGameOver = false;
    Texture gameOverTexture;
    gameOverTexture.loadFromFile("Textures/gameover1.png");
    Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTexture);
    gameOverSprite.setColor(Color(255, 255, 255, 255 * 0.20));
	 
    //Font for text display
	Font font;
	font.loadFromFile("Font/arial.ttf");

	

	//Score Display	
	int score = 0; // Score variable
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("Score");
	scoreText.setCharacterSize(40); 
	scoreText.setFillColor(Color::Black); 
	scoreText.setStyle(Text::Bold);
	scoreText.setPosition(750, 900); 
    //Instruction Display
	Text instructions;
    instructions.setFont(font);
    instructions.setString("Press SPACE to fire");
    instructions.setCharacterSize(40); // Adjust size as needed
    instructions.setFillColor(Color::Black);
    instructions.setStyle(Text::Bold);
    instructions.setPosition(0 ,900); // Position at the bottom of the screen


    // Define player area border dimensions
    

    // Create a rectangle to represent the border
    RectangleShape playerAreaBorder;
    playerAreaBorder.setSize(Vector2f(static_cast<float>(resolutionX), 5.0f));
    playerAreaBorder.setPosition(0, 768);
    playerAreaBorder.setFillColor(Color::Red); // Set the color of the border
    RectangleShape scoreBackground;
    scoreBackground.setSize(Vector2f(960, 50)); // Adjust size as needed
    scoreBackground.setFillColor(Color::Magenta);
    scoreBackground.setPosition(0, 900); // Adjust position to match scoreText





    
	
	


	
	

	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		

	
        //Gameover loop
        if (isGameOver) 
        {
                displayGameOverScreen(window, font);

            // Handle game over input (e.g., waiting for Enter or Escape)
            Event e;
            while (window.pollEvent(e)) 
            {
                if (e.type == Event::KeyPressed) 
                {
                    if (e.key.code == Keyboard::Enter) {
                        isGameOver=false;
                    } 
                    else if (e.key.code == Keyboard::Escape) 
                    {
                        window.close();
                    }
                }
            }
        continue; // Skip the rest of the game loop
        }


    //Gameloop
    while(isGameOver==false)
    {
        for (int i = 0; i < gameRows; ++i) 
        {
            for (int j = 0; j < gameColumns; ++j) 
            {
                if (mushroomGrid[i][j] == 1) 
                { // Check if a mushroom exists
                mushroomSprite.setPosition(static_cast<float>(i * boxPixelsX), static_cast<float>(j * boxPixelsY));
                window.draw(mushroomSprite);
                }
            }  
        }

        if (checkPlayerCentipedeCollision(player, centipedeSegments, centipedeLength)) {
        isGameOver = true;
        }        

        //Drawing GameElemts
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		

        //Collision with  bullet checking
		if (bullet[exists] == true) {
  		moveBullets(bulletX, bulletY, bulletExists, bulletClock , MAX_BULLETS);
    	drawBullets(window, bulletX, bulletY, bulletExists, bulletSprite, MAX_BULLETS);
		checkCollisionswithMushroom(bulletX, bulletY, bulletExists, mushroomGrid, score, MAX_BULLETS , MUSHROOM , killMusic);
		}

        //Drawing mushroom at random locations in game
		for (int i = 0; i < gameRows; ++i) 
		{
    		for (int j = 0; j < gameColumns; ++j) 
			{
        		if (mushroomGrid[i][j] >0) 
				{
				
            	mushroomSprite.setPosition(static_cast<float>(i * boxPixelsX), static_cast<float>(j * boxPixelsY));
				mushroomtornSprite.setPosition(static_cast<float>(i * boxPixelsX), static_cast<float>(j * boxPixelsY));
				if (mushroomGrid[i][j]>1)
            	window.draw(mushroomSprite);
				else
				window.draw(mushroomtornSprite);

        		}
    		}
		}

        
        //Movement check in player Area
		bool movement = checkcentipedemovement( centipedeSegments  , centipedeLength ,round);
		// Centipede Drawing/moving/splitting call
	    updateCentipedePosition(centipedeSegments, centipedeSprite, centipedeClock, mushroomGrid , centipedeLength , movement, round);
	    drawCentipede(window, centipedeSegments, centipedeSprite, centipedeFaceSprite, centipedeLength);
        SplitCentipede(window, bulletX, bulletY, bulletExists, centipedeSegments, centipedeLength, MAX_BULLETS, mushroomGrid,score);

        //Score add
		scoreText.setString("Score: " + std::to_string(score));
		
        //Player input handling , player movement and bullet fire with space
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				return 0;
			}
			
		handlePlayerInput(player, bulletX, bulletY, bulletExists, e, window , MAX_BULLETS, bulletfireSound);
		 }

        //DEbugging
		//std::cout << "Player Position - X: " << player[x] << ", Y: " << player[y] << std::endl;
			
		window.display();
		
		window.clear();

        //Drawing different features in game
        window.draw(scoreBackground);
		window.draw(scoreText); 
		window.draw(instructions);
        window.draw(playerAreaBorder);
        
		
	}
    }
}












////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////





//Function that handles player movement with AWSD OR Arrow Keys & Space for firing of bullet 
void handlePlayerInput(float player[], float bulletX[], float bulletY[], bool bulletExists[], Event& event, RenderWindow &window,const int MAX_BULLETS, Music& bgMusic)  {
    const float moveSpeed = 15.0f; // Movement speed


	// Define the vertical movement boundaries for the player
    const float minY = 785; // Bottom 5 rows
    const float maxY = 860; // Lower boundary (1 row height from bottom)

    if (event.type == Event::KeyPressed) {
        if ((event.key.code == Keyboard::Left || event.key.code == Keyboard::A) && player[x] > 0) {
            player[x] -= moveSpeed;
        } 
        if ((event.key.code == Keyboard::Right || event.key.code == Keyboard::D) && player[x] < resolutionX - boxPixelsX) {
            player[x] += moveSpeed;
        } 
        if ((event.key.code == Keyboard::Up || event.key.code == Keyboard::W) && player[y] > minY) {
            player[y] -= moveSpeed;
        } 
        if ((event.key.code == Keyboard::Down || event.key.code == Keyboard::S) && player[y] < maxY) {
            player[y] += moveSpeed;
        }
		if (event.key.code == Keyboard::Space) {
			
		
        for (int i = 0; i < MAX_BULLETS; ++i) {
			bgMusic.play();
			bgMusic.setVolume(20);
            if (!bulletExists[i]) {
                bulletX[i] = player[x];
                bulletY[i] = player[y] - boxPixelsY;
                bulletExists[i] = true;
                break;
            }
        }
    }

		 
    }
}



//Fucction to check bullet  and mushroom collision collision
bool isCollisionWithMushroom(float centipedeX, float centipedeY, int mushroomGrid[][gameColumns]) {
    int gridX = centipedeX / boxPixelsX;
    int gridY = centipedeY / boxPixelsY;

    return mushroomGrid[gridX][gridY] == 2;
}

//FUnction to draw player
void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

//Function to draw bullet
void drawBullets(RenderWindow& window, float bulletX[], float bulletY[], bool bulletExists[], Sprite& bulletSprite, const int MAX_BULLETS) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bulletExists[i]) {
            bulletSprite.setPosition(bulletX[i], bulletY[i]);
            window.draw(bulletSprite);
        }
    }
}


//Function for movement of bullet in game
void moveBullets(float bulletX[], float bulletY[], bool bulletExists[], Clock& bulletClock, const int MAX_BULLETS) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bulletExists[i]) {
            bulletY[i] -= 10;
            if (bulletY[i] < -32) {
                bulletExists[i] = false;
            }
        }
    }
}

//Function for generation of mushroom
void generateMushrooms(int mushroomGrid[gameRows][gameColumns], int count ,const int Health) {
    for (int i = 0; i < count; ++i) {
        int randomX = rand() % gameRows; 
        int randomY = rand() % 22 + (1); 

        if (mushroomGrid[randomX][randomY] == 0) {
            mushroomGrid[randomX][randomY] = Health; 
        } else {
            --i; // Retry if mushroom already exists at position
        }
    }
}


//Function that destroys mushroom when hit by bullet
void checkCollisionswithMushroom(float bulletX[], float bulletY[], bool bulletExists[], int mushroomGrid[][gameColumns], int& score, const int MAX_BULLETS, const int MUSHROOM , Music& killMusic) {
    for (int b = 0; b < MAX_BULLETS; ++b) {
        if (bulletExists[b]) {
            float bulletLeft = bulletX[b];
            float bulletRight = bulletX[b] + boxPixelsX;
            float bulletTop = bulletY[b];
            float bulletBottom = bulletY[b] + boxPixelsY;

            for (int i = 0; i < gameRows; ++i) {
                for (int j = 0; j < gameColumns; ++j) {
                    if (mushroomGrid[i][j] > 0) { // Check if mushroom exists
                        float mushroomLeft = i * boxPixelsX;
                        float mushroomRight = mushroomLeft + boxPixelsX;
                        float mushroomTop = j * boxPixelsY;
                        float mushroomBottom = mushroomTop + boxPixelsY;

                        if (bulletRight > mushroomLeft && bulletLeft < mushroomRight && bulletBottom > mushroomTop && bulletTop < mushroomBottom) {
                            bulletExists[b] = false; // Remove bullet
                            mushroomGrid[i][j]--; // Reduce mushroom health by 1

                            if (mushroomGrid[i][j] <= 0) {
                                mushroomGrid[i][j] = 0; // Destroy the mushroom
                                score += 1; // Increase score

                                
                                killMusic.stop();
                                
                
                                killMusic.play();
                                killMusic.setVolume(80);
                            }
                        }
                    }
                }
            }
        }
    }
}


//Function for drawing of centipede 
void drawCentipede(RenderWindow& window, float centipedeSegments[][3], Sprite& centipedeSprite, Sprite& centipedefaceSprite, int centipedeLength) {
    for (int i = 0; i < centipedeLength; ++i) {
        if (centipedeSegments[i][2] == 1.0) { // Check if the segment exists
            if (i == 0) { // If it's the head of the centipede
                centipedefaceSprite.setPosition(centipedeSegments[i][x], centipedeSegments[i][y]);
                window.draw(centipedefaceSprite);
            } else { // If it's a body segment
                centipedeSprite.setPosition(centipedeSegments[i][x], centipedeSegments[i][y]);
                window.draw(centipedeSprite);
            }
        }
    }
}


//Function for movement of centipede 
void updateCentipedePosition(float segments[][3], Sprite& sprite, Clock& movementClock, int obstacles[][gameColumns] ,const int lengthOfCentipede , bool checkcentipedemovement , int & round) {
    const int movementDelay = 50; // Time interval for movement updates

    if (movementClock.getElapsedTime().asMilliseconds() < movementDelay)
        return;

    movementClock.restart();

    for (int segmentIndex = 0; segmentIndex < lengthOfCentipede; ++segmentIndex) {
        if (segments[segmentIndex][exists]) {
            // Pre-calculate next position for collision check
            int upcomingX = (segments[segmentIndex][x] + (moveRight[segmentIndex] ? boxPixelsX : -boxPixelsX)) / boxPixelsX;
            int currentY = segments[segmentIndex][y] / boxPixelsY;

            // Determine if there's an obstacle in the next position
            if (upcomingX >= 0 && upcomingX < gameRows && obstacles[upcomingX][currentY] > 0) {
                // Adjust direction and descend if collision is imminent
                moveRight[segmentIndex] = !moveRight[segmentIndex];
                segments[segmentIndex][y] += boxPixelsY;
            } else {
                // Continue in current direction if path is clear
                segments[segmentIndex][x] += moveRight[segmentIndex] ? boxPixelsX : -boxPixelsX;
            }
        }
    }

    // Manage boundaries of the game area
    for (int segmentIndex = 0; segmentIndex < lengthOfCentipede; ++segmentIndex) {
        if (segments[segmentIndex][x] < 0) {
            if (segments[segmentIndex][exists]) {
                moveRight[segmentIndex] = 1;
               if (segments[segmentIndex][y]>860 && checkcentipedemovement == 0)
                {
                segments[segmentIndex][y] -= boxPixelsY;
                }
                else if (segments[segmentIndex][y]<860  && round % 2 == 1)
                {
                segments[segmentIndex][y] += boxPixelsY;
                }
                else if (segments[segmentIndex][y]>768  && round %2 == 0)
                {
                segments[segmentIndex][y] -= boxPixelsY;
                }
                else
                {
                segments[segmentIndex][y] += boxPixelsY;
                }
            
            }
        } else if (segments[segmentIndex][x] >= (resolutionX)) {
            if (segments[segmentIndex][exists]) {
                moveRight[segmentIndex] = 0;
                if (segments[segmentIndex][y]>860 && checkcentipedemovement == 0)
                {
                segments[segmentIndex][y] -= boxPixelsY;
                }
                else if (segments[segmentIndex][y]<860  && round % 2 == 1)
                {
                segments[segmentIndex][y] += boxPixelsY;
                }
                else if (segments[segmentIndex][y]>768  && round %2 == 0)
                {
                segments[segmentIndex][y] -= boxPixelsY;
                }
                else
                {
                segments[segmentIndex][y] += boxPixelsY;
                }
            }
        }
    }
}

//Function to check movement of centipede when in player field
bool checkcentipedemovement(float centipedeSegments[][3], const int centipede_length, int &round) {
    // Update the round based on centipede's position
    if (centipedeSegments[11][y] > 860 && round %2 == 1) {
        round += 1;
    } else if (centipedeSegments[11][y] == 768 && round %2 == 0) {
        round += 1;
    }

    // Determine the movement based on the updated round
    if (round % 2 == 0) {
        return true; 
    } else {
        return false;
    }
}



//Function for splitting of centipede
void SplitCentipede(RenderWindow &window, float bulletX[], float bulletY[], bool bulletExists[], float centipedeSegments[][3], const int centipede_length, int MAX_BULLETS, int mushroomGrid[][gameColumns], int& score) {
    for (int b = 0; b < MAX_BULLETS; ++b) {
        if (bulletExists[b]) {
            float bulletLeft = bulletX[b];
            float bulletRight = bulletX[b] + boxPixelsX;
            float bulletTop = bulletY[b];
            float bulletBottom = bulletY[b] + boxPixelsY;

            for (int i = 0; i < centipede_length; ++i) {
                if (centipedeSegments[i][exists]) {
                    if (bulletRight > centipedeSegments[i][x] && bulletLeft < centipedeSegments[i][x] + boxPixelsX &&
                        bulletBottom > centipedeSegments[i][y] && bulletTop < centipedeSegments[i][y] + boxPixelsY) {
                        bulletExists[b] = false; // Remove bullet
                        
                        // Check if the hit segment is the head
                        if (i == 0) {
                            score += 20; // Increase score by 20 for head
                        } else {
                            score += 10; // Increase score by 10 for body
                        }

                        // Set centipede segment as destroyed
                        centipedeSegments[i][exists] = false;

                        // Calculate grid position for the mushroom
                        int gridX = static_cast<int>(centipedeSegments[i][x] / boxPixelsX);
                        int gridY = static_cast<int>(centipedeSegments[i][y] / boxPixelsY);
                        mushroomGrid[gridX][gridY] = 2; // Mark the grid cell with a mushroom

                        break;
                    }
                }
            }
        }
    }
}

//Function to check player and centipede collision and then game over
bool checkPlayerCentipedeCollision(float player[], float centipedeSegments[][3], int centipedeLength) {
    for (int i = 0; i < centipedeLength; ++i) {
        if (centipedeSegments[i][exists]) {
            // Calculate the bounds of the centipede segment
            float centipedeLeft = centipedeSegments[i][x];
            float centipedeRight = centipedeSegments[i][x] + boxPixelsX;
            float centipedeTop = centipedeSegments[i][y];
            float centipedeBottom = centipedeSegments[i][y] + boxPixelsY;

            // Calculate the bounds of the player
            float playerLeft = player[x];
            float playerRight = player[x] + boxPixelsX;
            float playerTop = player[y];
            float playerBottom = player[y] + boxPixelsY;

            // Check for collision
            if (centipedeRight > playerLeft && centipedeLeft < playerRight &&
                centipedeBottom > playerTop && centipedeTop < playerBottom) {
                return true;
            }
        }
    }
    return false;
}

//Function to draw game over screen
void displayGameOverScreen(RenderWindow &window, Font &font) {
    // black background
    RectangleShape background(Vector2f(static_cast<float>(resolutionX), static_cast<float>(resolutionY)));
    background.setFillColor(Color::Black);

    //  "Game Over" text
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(70); // Adjust the size as needed
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setPosition(resolutionX / 2 - gameOverText.getLocalBounds().width / 2, resolutionY / 2 - gameOverText.getLocalBounds().height / 2);

    // continuation prompt text
    Text continueText;
    continueText.setFont(font);
    continueText.setString("Press Enter to continue or Escape to exit");
    continueText.setCharacterSize(30); // Adjust the size as needed
    continueText.setFillColor(Color::White);
    continueText.setPosition(resolutionX / 2 - continueText.getLocalBounds().width / 2, resolutionY / 2 + 100);

    // Draw everything
    window.draw(background);
    window.draw(gameOverText);
    window.draw(continueText);
    window.display();
}





