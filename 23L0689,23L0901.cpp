#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <SFML/System/Clock.hpp>
#include <fstream>

using namespace sf;
using namespace std;
RenderWindow window(VideoMode(1400, 700), "Ahmad's and Hamza's candycrush");
int candies = 6;
Texture texturecandies[9];
int const rows=9;
int const clms=9 ;
int score = 0;

void printboard(int array[rows][clms]) {
    int candies = 6;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            array[i][j] = (rand() % 6) + 1;
            cout << array[i][j] << '\t';
        }
        cout << endl;
    }
}
bool hint(int array[rows][clms]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == array[i - 1][j - 1] && array[i - 1][j - 2] == array[i][j]) {
                return true;

            }
            else if (array[i][j] == array[i + 1][j + 1] && array[i + 1][j + 2] == array[i][j]) {
                return true;
            }
            else if (j >= 3) {
                if (array[i][j] == array[i][j - 2] && array[i][j - 3] == array[i][j]) {
                    return true;
                }
            }
            else if (j <= 5) {
                if (array[i][j] == array[i][j + 2] && array[i][j + 3] == array[i][j]) {
                    return true;
                }
            }
            else if (j <= 7) {
                if (array[i][j] == array[i + 1][j + 1] && array[i - 1][j + 1] == array[i][j]) {
                    return true;
                }
            }
        }
    }
    return false;
}
void graphicboard(RenderWindow& window, int array[rows][clms]) {
    int cellsize = 60.0f;
    float padding = 5.0f;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clms; ++j) {
            Sprite candySprite;
            candySprite.setTexture(texturecandies[array[i][j]]);

            float scaleFactor = min(cellsize / candySprite.getGlobalBounds().width, cellsize / candySprite.getGlobalBounds().height);
            candySprite.setScale(scaleFactor, scaleFactor);

            float xPos = j * (cellsize + padding);
            float yPos = i * (cellsize + padding);
            candySprite.setPosition(xPos, yPos);

            window.draw(candySprite);
        }
    }
}

bool checkForMatches(int array[rows][clms]) {
    // Check for matches in rows
    
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < clms - 2; ++j) {
                if (array[i][j] == array[i][j + 1] && array[i][j] == array[i][j + 2]) {
                    return true;
                }
            }
        }
    
    // Check for matches in columns
    for (int j = 0; j < clms; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 2][j]) {
                return true;
            }
        }
    }

    // Check for matches in diagonals
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < clms - 2; j++) {
            if (array[i][j] == array[i + 1][j + 1] && array[i][j] == array[i + 2][j + 2]) {
                return true;
            }
            if (array[i][j + 2] == array[i + 1][j + 1] && array[i][j + 2] == array[i + 2][j]) {
                return true;
            }
        }
    }

    return false;
}

void eliminateMatchesAndDrop(int array[rows][clms]) {
    // Eliminate matches
      // elbow
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 1][j + 1] && array[i][j] != 0) {
                if (array[i][j] != 0 && array[i + 1][j] != 0 && array[i + 1][j + 1] != 0) {
                    array[i][j] = array[i + 1][j + 1] = array[i + 1][j] = 0;
                }
            }
            if (array[i][j] == array[i][j + 1] && array[i][j] == array[i + 1][j] && array[i][j] != 0) {
                if (array[i][j] != 0 && array[i][j + 1] != 0 && array[i + 1][j] != 0) {
                    array[i][j] = array[i][j + 1] = array[i + 1][j] = 0;
                }
            }
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 1][j - 1] && array[i][j] != 0) {
                if (array[i][j] != 0 && array[i + 1][j] != 0 && array[i + 1][j - 1] != 0) {
                    array[i][j] = array[i + 1][j] = array[i + 1][j - 1] = 0;
                }
            }
            if (array[i][j] == array[i][j + 1] && array[i][j] == array[i + 1][j + 1] && array[i][j] != 0) {
                if (array[i][j] != 0 && array[i][j + 1] != 0 && array[i + 1][j + 1] != 0) {
                    array[i][j] = array[i][j + 1] = array[i + 1][j + 1] = 0;
                }
            }
        }


    }


    //five matched
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms - 4; j++) {
            if (array[i][j] == array[i][j + 1] && array[i][j] == array[i][j + 2] && array[i][j] == array[i][j + 3] && array[i][j] ==
                array[i][j + 4]) {
                if (array[i][j] != 0 && array[i][j + 1] != 0 && array[i][j + 2] != 0 && array[i][j + 3] != 0 &&
                    array[i][j + 4] != 0) {
                    array[i][j] = 8;
                    array[i][j + 1] = 0;
                    array[i][j + 2] = 0;
                    array[i][j + 3] = 0;
                    array[i][j + 4] = 0;
                }
            }
        }
    }
    for (int j = 0; j < clms; ++j) {
        for (int i = 0; i < rows - 4; ++i) {
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 2][j] && array[i][j] == array[i + 3][j]
                && array[i][j] == array[i + 4][j]) {
                if (array[i][j] != 0 && array[i + 1][j] != 0 && array[i + 2][j] != 0 && array[i + 3][j] != 0 &&
                    array[i + 4][j] != 0) {
                    array[i][j] = 8;
                    array[i + 1][j] = 0;
                    array[i + 2][j] = 0;
                    array[i + 3][j] = 0;
                    array[i + 4][j] = 0;
                }
            }
        }
    }

    //four matched

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms - 3; j++) {
            if (array[i][j] == array[i][j + 1] && array[i][j] == array[i][j + 3] && array[i][j] == array[i][j + 2]) {
                array[i][j] = 7;
                array[i][j + 1] = 0;
                array[i][j + 2] = 0;
                array[i][j + 3] = 0;
            }
        }
    }

    for (int j = 0; j < clms; j++) {
        for (int i = 0; i < rows - 3; i++) {
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 3][j] && array[i][j] == array[i + 2][j]) {
                array[i][j] = 7;
                array[i + 1][j] = 0;
                array[i + 2][j] = 0;
                array[i + 3][j] = 0;

            }
        }
    }
    //five diagonal right
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == array[i + 1][j - 1] && array[i][j] == array[i + 2][j - 2] && array[i][j] == array[i + 3][j - 3]
                && array[i][j] == array[i + 4][j - 4]) {

                array[i][j] = array[i + 1][j - 1] = array[i + 2][j - 2] = array[i + 3][j - 3] = array[i + 4][j - 4] = 0;
            }
        }
    }
    //five diagonal left
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == array[i + 1][j + 1] && array[i][j] == array[i + 2][j + 2] && array[i + 3][j + 3] == array[i][j] &&
                array[i][j] == array[i + 4][j + 4] && array[i][j] != 0 && array[i + 1][j] != 0 && array[i + 1]) {
                array[i][j] = array[i + 1][j + 1] = array[i + 2][j + 2] = array[i + 3][j + 3] = array[i + 4][j + 4]
                    = 0;
            }
        }
    }
    //four diagonal right
      for (int i = 0; i < rows; i++) {
    for (int j = 0; j < clms; j++) {
        if (array[i][j] == array[i + 1][j - 1] && array[i][j] == array[i + 2][j - 2] && array[i][j] == array[i + 3][j - 3]
            ) {

            array[i][j] = array[i + 1][j - 1] = array[i + 2][j - 2] = array[i + 3][j - 3] = 0;
        }
    }
    }
      //four diagonal left
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < clms; j++) {
          if (array[i][j] == array[i + 1][j + 1] && array[i][j] == array[i + 2][j + 2] && array[i + 3][j + 3] == array[i][j] &&
              array[i][j] != 0 && array[i + 1][j] != 0 && array[i + 1]) {
              array[i][j] = array[i + 1][j + 1] = array[i + 2][j + 2] = array[i + 3][j + 3] = 0;

          }
      }
    }
    //diagonals
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clms - 2; ++j) {
            if (array[i][j] == array[i][j + 1] && array[i][j] == array[i][j + 2]) {
                array[i][j] = 0;
                array[i][j + 1] = 0;
                array[i][j + 2] = 0;
            }
        }
    }

    for (int j = 0; j < clms; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            if (array[i][j] == array[i + 1][j] && array[i][j] == array[i + 2][j]) {
                array[i][j] = 0;
                array[i + 1][j] = 0;
                array[i + 2][j] = 0;
            }
        }
    }

    for (int i = 0; i < rows - 2; ++i) {
        for (int j = 0; j < clms - 2; ++j) {
            if (array[i][j] == array[i + 1][j + 1] && array[i][j] == array[i + 2][j + 2]) {
                array[i][j] = 0;
                array[i + 1][j + 1] = 0;
                array[i + 2][j + 2] = 0;
            }
        }
    }

    for (int i = 0; i < rows - 2; ++i) {
        for (int j = 2; j < clms; ++j) {
            if (array[i][j] == array[i + 1][j - 1] && array[i][j] == array[i + 2][j - 2]) {
                array[i][j] = 0;
                array[i + 1][j - 1] = 0;
                array[i + 2][j - 2] = 0;
            }
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clms - 2; ++j) {
            // Check for horizontal matches
            if (array[i][j] != 0 && array[i][j] == array[i][j + 1] && array[i][j] == array[i][j + 2]) {
                array[i][j] = array[i][j + 1] = array[i][j + 2] = 0;
            }
        }
    }

    for (int j = 0; j < clms; ++j) {
        for (int i = 0; i < rows - 2; ++i) {
            // Check for vertical matches
            if (array[i][j] != 0 && array[i][j] == array[i + 1][j] && array[i][j] == array[i + 2][j]) {
                array[i][j] = array[i + 1][j] = array[i + 2][j] = 0;
            }
        }
    }


    // Drop candies
    for (int j = 0; j < clms; ++j) {
        int k = rows - 1;
        for (int i = rows - 1; i >= 0; --i) {
            if (array[i][j] != 0) {
                array[k][j] = array[i][j];
                if (k != i) {
                    array[i][j] = 0;
                }
                --k;
            }
        }
    }
}
  void fourbombs(int array[][clms]) {
            //row bomb four
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < clms; j++) {
                    if (array[i][j] == 7) {
                        array[i][j] = 0;
                        array[i][j + 1] = 0;
                        array[i][j - 1] = 0;
                        array[i + 1][j] = 0;
                        array[i + 1][j + 1] = 0;
                        array[i + 1][j - 1] = 0;
                        array[i - 1][j] = 0;
                        array[i - 1][j + 1] = 0;
                        array[i - 1][j - 1] = 0;
                        score += 50;
                    }
                }
            }
        
    // coloumn bomb four
    for (int j = 0; j < clms; j++) {
        for (int i = 0; i < rows; i++) {
            if (array[i][j] == 7) {
                array[i][j] = 0;
                array[i + 1][j] = 0;
                array[i - 1][j] = 0;
                array[i][j + 1] = 0;
                array[i + 1][j + 1] = 0;
                array[i + 1][j - 1] = 0;
                array[i][j - 1] = 0;
                array[i - 1][j + 1] = 0;
                array[i - 1][j - 1] = 0;
                score += 50;
            }
        }
    }
  }
void swapCandies(int array[rows][clms], Vector2i first, Vector2i second) {
    // Check if candies are adjacent (one place up, down, left, or right)
    if ((abs(first.x - second.x) == 1 && first.y == second.y) || (abs(first.y - second.y) == 1 && first.x == second.x)) {
        int temp = array[first.y][first.x];
        array[first.y][first.x] = array[second.y][second.x];
        array[second.y][second.x] = temp;
    }
}

void addRandomCandiesToTop(int array[rows][clms]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == 0) {
                array[i][j] = (rand() % candies) + 1;
                cout << array[i][j] << '\t';
            }
        }
    }
}
void fivebombs(int array[][clms]) {
    //row bomb
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == 8) {
                for (int j = 0; j < 9; j++) {
                    array[i][j] = 0;
                }
                for (int i = 0; i < 9; i++) {
                    array[i][j] = 0;
                }
            }
        }
    }
    // coloumn bomb
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clms; j++) {
            if (array[i][j] == 8) {
                for (int i = 0; i < rows; i++) {
                    array[i][j] = 0;
                }
                for (int j = 0; j < clms; j++) {
                    array[i][j] = 0;
                }
            }
        }

    }
}
void nomoves(int array[][clms]) {
    if (!checkForMatches) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < clms; ++j) {
                array[i][j] = 0;
            }

        }
    }
    printboard(array);
    graphicboard(window, array);
}
void loadTextures() {
    texturecandies[1].loadFromFile("blue.jpg");
    texturecandies[2].loadFromFile("green.jpg");
    texturecandies[3].loadFromFile("orange.jpg");
    texturecandies[4].loadFromFile("purple.jpg");
    texturecandies[5].loadFromFile("yellow.jpg");
    texturecandies[6].loadFromFile("red.jpg");
    texturecandies[7].loadFromFile("coconut.jpg");
    texturecandies[8].loadFromFile("disco.jpg");
}
void saveGame(int array[rows][clms], int score, int moves) {
    ofstream file("savegame.txt");

    // Save the board
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clms; ++j) {
            file << array[i][j] << " ";
        }
        file << '\n';
    }

    // Save the score and moves
    file << score << " " << moves;

    file.close();
}

void loadGame(int array[rows][clms], int& score, int& moves) {
    ifstream file("savegame.txt");

    // Load the board
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < clms; ++j) {
            file >> array[i][j];
        }
    }

    // Load the score and moves
    file >> score >> moves;

    file.close();
}

int main() {
    Clock timer;
    float padding = 5.0f;

    Font font;
    if (!font.loadFromFile("Roboto-Italic.ttf")) {
        return EXIT_FAILURE;
    }

    int moves = 10;
    int score = 0;
    int ninebynineboard[rows][clms];
    ifstream savedGameFile("savegame.txt");
    if (savedGameFile.is_open()) {
        savedGameFile.close();
        loadGame(ninebynineboard, score, moves);
    }
    else {
        printboard(ninebynineboard);
    }

    loadTextures();

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(60);
    scoreText.setFillColor(Color::Cyan);
    scoreText.setPosition(800.0f, 100.0f);

   Text movesText;
    movesText.setFont(font);
    movesText.setCharacterSize(60);
    movesText.setFillColor(Color::Cyan);
    movesText.setPosition(800.0f, 200.0f);

   Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(80);
    timerText.setFillColor(Color::Yellow);
    timerText.setPosition(950.0f, 600.0f);

   Texture texture;
    if (!texture.loadFromFile("cc.jpg")) {
       cout << "Error occurred" << endl;
    }

   Sprite bg(texture);
    float setsize1 = static_cast<float>(window.getSize().x) / bg.getTexture()->getSize().x;
    float setsize2 = static_cast<float>(window.getSize().y) / bg.getTexture()->getSize().y;
    bg.setScale(setsize1, setsize2);

    
    printboard(ninebynineboard);

    loadTextures();

    Vector2i selectedCandy(-1, -1);

    scoreText.setString("Score: " + to_string(score));
    movesText.setString("Moves: " + to_string(moves));

    bool showHint = false;

    // Load background music
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("theme_tune.ogg")) {
        cout << "Failed to load background music!" << endl;
        return EXIT_FAILURE;
    }

    backgroundMusic.setVolume(50);
    backgroundMusic.setLoop(true);

    backgroundMusic.play();

    while (window.isOpen() && moves > 0) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed) {
                timer.restart();
                if (event.mouseButton.button == Mouse::Left) {
                   Vector2i mousePos = Mouse::getPosition(window);
                    int col = mousePos.x / (60 + padding);
                    int row = mousePos.y / (60 + padding);

                    if (selectedCandy.x == -1 && selectedCandy.y == -1) {
                        selectedCandy =Vector2i(col, row);
                    }
                    else {
                        swapCandies(ninebynineboard, selectedCandy, Vector2i(col, row));
                        selectedCandy = Vector2i(-1, -1);
                        moves--;

                        if (checkForMatches(ninebynineboard)) {
                            eliminateMatchesAndDrop(ninebynineboard);
                            addRandomCandiesToTop(ninebynineboard);
                            loadTextures();
                            score += 10;
                        }

                        int selectedCandyValue = ninebynineboard[row][col];
                        if (selectedCandyValue == 7) {
                            fourbombs(ninebynineboard);
                            score += 100;
                        }
                        else if (selectedCandyValue == 8) {
                            fivebombs(ninebynineboard);
                            score += 200;
                        }
                    }
                }
            }
            else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::H) {
                    showHint = !showHint;
                }
            }
        }

        float elapsedSeconds = timer.getElapsedTime().asSeconds();

        if (elapsedSeconds >= 10.0f) {
            moves--;
            timer.restart();
        }

        scoreText.setString("Score: " + to_string(score));
        movesText.setString("Moves: " + to_string(moves));
        timerText.setString("Timer: " + to_string(static_cast<int>(10.0f - elapsedSeconds)));

        window.clear();
        window.draw(bg);
        window.draw(scoreText);
        window.draw(movesText);
        window.draw(timerText);

        if (showHint) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < clms; ++j) {
                    if (hint(ninebynineboard)) {
                        RectangleShape highlight;
                        highlight.setSize(Vector2f(60, 60));
                        highlight.setOutlineThickness(5);
                        highlight.setFillColor(Color::Black);
                        float xPos = j * (60 + padding);
                        float yPos = i * (60 + padding);
                        highlight.setPosition(xPos, yPos);
                        window.draw(highlight);
                    }
                }
            }
        }

        if (selectedCandy.x != -1 && selectedCandy.y != -1) {
            RectangleShape highlight;
            highlight.setSize(Vector2f(60, 60));
            highlight.setOutlineThickness(5);
            highlight.setOutlineColor(Color::Yellow);

            float xPos = selectedCandy.x * (60 + padding);
            float yPos = selectedCandy.y * (60 + padding);
            highlight.setPosition(xPos, yPos);

            window.draw(highlight);
        }

        graphicboard(window, ninebynineboard);
        checkForMatches(ninebynineboard);
        eliminateMatchesAndDrop(ninebynineboard);
        addRandomCandiesToTop(ninebynineboard);
        eliminateMatchesAndDrop(ninebynineboard);
        loadTextures();
        window.display();

    }
    nomoves(ninebynineboard);
    backgroundMusic.stop();
    saveGame(ninebynineboard, score, moves);
    return 0;
}