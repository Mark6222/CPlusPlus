//#include "stdafx.h" <- Visual Studio only
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
// Make code easier to type with "using namespace"
using namespace sf;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side{LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
    Texture textureBackground;
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    Clock clock;
    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    bool paused = true;
    // Draw some text
    int score = 0;
    Text messageText;
    Text scoreText;
    // Choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Set font for message
    messageText.setFont(font);
    scoreText.setFont(font);
    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    // Choose a colour
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }

    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    side playerSide = side::LEFT;

    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setPosition(600, 860);
    spriteRIP.setTexture(textureRIP);

    Texture textureAXE;
    textureAXE.loadFromFile("graphics/axe.png");
    Sprite spriteAXE;
    spriteAXE.setTexture(textureAXE);
    spriteAXE.setPosition(700, 830);

    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1070;

    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive = false;
    float logSpeedx = 1000;
    float logSpeedY = -1500;

    bool acceptInput = false;
    // Prepare the sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);
    // out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);
    while (window.isOpen())
    {
        /*
         ****************************************
         Handle the players input
         ****************************************
         */
        //play the chop sound
        Event event;

        while(window.pollEvent(event)){
            if (event.type == Event::KeyReleased && !paused) {
                acceptInput = true;
                spriteAXE.setPosition(2000, spriteAXE.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 6;

            for (int i = 0; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
                // Make sure the gravestone is hidden
                spriteRIP.setPosition(675, 2000);
                // Move the player into position
                spritePlayer.setPosition(580, 720);
                // Move the axe into position
                spriteAXE.setPosition(700, 830);
                acceptInput = true;
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Wrap the player controls to make sure we are accepting input
        if (acceptInput) {
            // Pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                // Make sure the player is on the right
                playerSide = side::RIGHT;
                score++;
                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15; // reflect on this code
                spriteAXE.setPosition(AXE_POSITION_RIGHT, spriteAXE.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                chop.play();
                // update the branches
                updateBranches(score);
                // set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedx = -5000;
                logActive = true;
                acceptInput = false;
            }// End if (Keyboard :: isKeyPressed(Keyboard :: Right))
            // handle the left cursor key
            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                // Make sure the player is on the left
                playerSide = side::LEFT;
                score++;
                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;
                spriteAXE.setPosition(AXE_POSITION_LEFT, spriteAXE.getPosition().y);
                chop.play();
                spritePlayer.setPosition(580, 720);
                // update the branches
                updateBranches(score);
                // set the log flying
                spriteLog.setPosition(810, 720);
                logSpeedx = 5000;
                logActive = true;
                acceptInput = false;
            }
        }// End if (acceptInput)

        /*
         ****************************************
         Update the scene
         ****************************************
         */
        if (!paused) {
            Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            if (timeRemaining <= 0.0f) {
                // Pause the game
                paused = true;
                // Change the message shown to the player
                messageText.setString("Out of time!!");
                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                outOfTime.play();
            }

            if (!beeActive) {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else {
                spriteBee.setPosition(
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y);

                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }

            if (!cloud1Active) {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;
                }
            }
            if (!cloud2Active) {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;
                }
            }
            if (!cloud3Active) {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > 1920) {
                    cloud3Active = false;
                }
            }
            // Update the score text
            std::stringstream ss; // Note std. We're not using namespace
            ss << "Score = " << score; // << concatenates variables into a stringstream
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    // Move the sprite to the left side
                    branches[i].setPosition(600, height);
                    // Flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT) {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);
                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }//end for
            if (logActive) {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedx * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedx * dt.asSeconds()));
            }

            if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x>2000) {
                logActive = false;
                spriteLog.setPosition(810, 720);
            }

            if (branchPositions[5] == playerSide) {
                paused = true;
                acceptInput = false;
                spriteRIP.setPosition(525, 760);
                spritePlayer.setPosition(2000, 660);
                messageText.setString("SQUISHED!!!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                death.play();
            }
        }

         /*
          ****************************************
          Draw the scene
          ****************************************
          */
          // Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        // Draw The Branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAXE);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);
        // Draw the score
        window.draw(scoreText);
        window.draw(timeBar);
        if (paused) {
            // Draw our message
            window.draw(messageText);
        }
        // Show everything we just drew
        window.display();
    }
    return 0;
}
void updateBranches(int seed) {
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }
    // Spawn a new branch at position 0 - LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }// end switch
} // end function
