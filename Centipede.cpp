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


void handlePlayerInput(float player[], float bulletX[], float bulletY[], bool bulletExists[], sf::Event& event, sf::RenderWindow &window,const int MAX_BULLETS,	sf::Music& bgMusic);
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullets(float bulletX[], float bulletY[], bool bulletExists[], sf::Clock& bulletClock, const int MAX_BULLETS);
void drawBullets(sf::RenderWindow& window, float bulletX[], float bulletY[], bool bulletExists[], sf::Sprite& bulletSprite, const int MAX_BULLETS);
void generateMushrooms(int mushroomGrid[gameRows][gameColumns] , int count);
void checkCollisions(float bulletX[], float bulletY[], bool bulletExists[], int mushroomGrid[][gameColumns], int& score , const int MAX_BULLETS , const int MUSHROOM, sf::Music& killMusic);
void drawCentipede(sf::RenderWindow& window, float centipede[], sf::Sprite& centipedeSprite);
bool isCollisionWithMushroom(float centipedeX, float centipedeY, int mushroomGrid[][gameColumns]);










int main()
{

	
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede Game", sf::Style::Close | sf::Style::Titlebar);                                                                                                                                                   

	// Used to resize your window if it's too big or too small. Use according to your needs.
	  window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	//window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
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
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 3 / 4) * boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	const int MAX_BULLETS = 10; // Maximum number of bullets
 	float bulletX[MAX_BULLETS] = {};
	float bulletY[MAX_BULLETS] = {};
	bool bulletExists[MAX_BULLETS] = {};
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = true;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));


	// Drawing Mushrooms.
	Texture mushroomTexture;
	Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png"); 
  	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	int mushroomGrid[gameRows][gameColumns] = {}; 
	const int MUSHROOM = 1; 
	generateMushrooms(mushroomGrid , 50);

	// Drawing Centipede.
	float centipede[2]; // Position
	centipede[x] = resolutionX / 2;  
	centipede[y] = 0;
	float centipedeSpeed = 0.2f; 
	bool movingRight = true; 

	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedeSprite.setTexture(centipedeTexture);
	

	sf::Font font;
	font.loadFromFile("arial.ttf");

	

	//Score Display	
	int score = 0; // Score variable
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(40); 
	scoreText.setFillColor(sf::Color::Yellow); 
	scoreText.setPosition(750, 900); 

	
	


	

	
	

	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		

	
		
		window.draw(backgroundSprite);
		
	
		drawPlayer(window, player, playerSprite);
		drawCentipede(window, centipede, centipedeSprite);
		if (bullet[exists] == true) {
  		moveBullets(bulletX, bulletY, bulletExists, bulletClock , MAX_BULLETS);
    	drawBullets(window, bulletX, bulletY, bulletExists, bulletSprite, MAX_BULLETS);
		checkCollisions(bulletX, bulletY, bulletExists, mushroomGrid, score, MAX_BULLETS , MUSHROOM , killMusic);
		}
		for (int i = 0; i < gameRows; ++i) 
		{
    		for (int j = 0; j < gameColumns; ++j) 
			{
        		if (mushroomGrid[i][j] == MUSHROOM) 
				{
            	mushroomSprite.setPosition(static_cast<float>(i * boxPixelsX), static_cast<float>(j * boxPixelsY));
            	window.draw(mushroomSprite);
        		}
    		}
		}

		if (movingRight) 
		{
    	centipede[x] += centipedeSpeed;
    		if (centipede[x] > resolutionX - boxPixelsX) 
			{ 
        	centipede[y] += boxPixelsY; 
        	movingRight = false; 
    		}
		} 
		else 
		{
    	centipede[x] -= centipedeSpeed;
    		if (centipede[x] < 0) 
			{ 
        	centipede[y] += boxPixelsY; 
        	movingRight = true; 
    		}
		}

		

		scoreText.setString("Score: " + std::to_string(score));
		

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
			
		handlePlayerInput(player, bulletX, bulletY, bulletExists, e, window , MAX_BULLETS, bulletfireSound);
		 }
		
			
		window.display();
		window.clear();
		window.draw(scoreText); 
		
	}
}












////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////




void handlePlayerInput(float player[], float bulletX[], float bulletY[], bool bulletExists[], sf::Event& event, sf::RenderWindow &window,const int MAX_BULLETS, sf::Music& bgMusic)  {
    const float moveSpeed = 20.0f; // Movement speed

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left && player[x] > 0) {
            player[x] -= moveSpeed;
        }  if (event.key.code == sf::Keyboard::Right && player[x] < resolutionX - boxPixelsX) {
            player[x] += moveSpeed;
        }  if (event.key.code == sf::Keyboard::Up && player[y] > 0) {
            player[y] -= moveSpeed;
        }  if (event.key.code == sf::Keyboard::Down && player[y] < resolutionY - boxPixelsY) {
            player[y] += moveSpeed;
        }
		if (event.key.code == sf::Keyboard::Space) {
			
		
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

void generateMushrooms(int mushroomGrid[gameRows][gameColumns] , int count) {
	const int MUSHROOM = 1;
	int maxY = gameColumns * 2 / 3; // Maximum height of mushrooms
    
    for (int i = 0; i < count; ++i) {
        int randomX = rand() % gameRows; // Generates a random row
        int randomY = rand() % maxY; // Generates a random column

        if (mushroomGrid[randomX][randomY] == 0) { // Check if there's no mushroom already
            mushroomGrid[randomX][randomY] = MUSHROOM; // Place a mushroom
        } else {
            --i; // If a mushroom is already there, try again
        }
    }
}


void drawCentipede(sf::RenderWindow& window, float centipede[], sf::Sprite& centipedeSprite) {
	centipedeSprite.setPosition(centipede[x], centipede[y]);
	
	window.draw(centipedeSprite);

}

bool isCollisionWithMushroom(float centipedeX, float centipedeY, int mushroomGrid[][gameColumns]) {
    int gridX = centipedeX / boxPixelsX;
    int gridY = centipedeY / boxPixelsY;

    return mushroomGrid[gridX][gridY] == 1;
}









void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

void drawBullets(sf::RenderWindow& window, float bulletX[], float bulletY[], bool bulletExists[], sf::Sprite& bulletSprite, const int MAX_BULLETS) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bulletExists[i]) {
            bulletSprite.setPosition(bulletX[i], bulletY[i]);
            window.draw(bulletSprite);
        }
    }
}

void moveBullets(float bulletX[], float bulletY[], bool bulletExists[], sf::Clock& bulletClock, const int MAX_BULLETS) {
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

void checkCollisions(float bulletX[], float bulletY[], bool bulletExists[], int mushroomGrid[][gameColumns], int& score, const int MAX_BULLETS, const int MUSHROOM, sf::Music& killMusic) 
{
    for (int b = 0; b < MAX_BULLETS; ++b) 
	{
        if (bulletExists[b]) 
		{
            // Get the boundaries of the bullet
            float bulletLeft = bulletX[b];
            float bulletRight = bulletX[b] + boxPixelsX;
            float bulletTop = bulletY[b];
            float bulletBottom = bulletY[b] + boxPixelsY;

            for (int i = 0; i < gameRows; ++i) 
			{
                for (int j = 0; j < gameColumns; ++j) {
                    if (mushroomGrid[i][j] == MUSHROOM) 
					{
                        // Get the boundaries of the mushroom
                        float mushroomLeft = i * boxPixelsX;
                        float mushroomRight = mushroomLeft + boxPixelsX;
                        float mushroomTop = j * boxPixelsY;
                        float mushroomBottom = mushroomTop + boxPixelsY;

                        // Check for collision
                        if (bulletRight > mushroomLeft && bulletLeft < mushroomRight && bulletBottom > mushroomTop && bulletTop < mushroomBottom) 
						{
                            // Collision detected
                            mushroomGrid[i][j] = 0; // Remove mushroom
                            bulletExists[b] = false; // Remove bullet
                            killMusic.play();
                            killMusic.setVolume(80);
                            score += 5; // Increase score
                            return; // Exit the loop after the collision is handled
                        }
                    }
                }
            }
        }
    }
}





