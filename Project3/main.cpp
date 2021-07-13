#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include "Random.h"
#include "Board.h"
#include <sstream>
#include <string>
using namespace sf;
using namespace std;

struct objt {
    int xPos;
    int yPos;
    int state;
    bool isRevealed = false;
    Sprite t;
};

struct tiles {
    bool bomb[16][25];
    objt arr[16][25];

    void setBombs() {
        for (int i = 0; i < 16; i++) {
            for (int w = 0; w < 25; w++) {
                bomb[i][w] = false;
            }
        }

        int count2 = 0;
        while (count2 < 50) {
            int x2 = Random::Int(0, 25 - 1);
            int y2 = Random::Int(0, 16 - 1);
            if (bomb[y2][x2] == false) {
                bomb[y2][x2] = true;
                count2++;
            }
        }

        for (int i = 0; i < 16; i++) {
            for (int w = 0; w < 25; w++) {
                cout << bomb[i][w];
            }
            cout << endl;
        }
        cout << endl;

        for (int i = 0; i < 16; i++) {
            objt tileBomb;
            tileBomb.t.setTexture(Board::GetTexture("mine"));
            for (int w = 0; w < 25; w++) {
                if (bomb[i][w] == true) {
                    tileBomb.t.setPosition(Vector2f(w * tileBomb.t.getTextureRect().width, i * tileBomb.t.getTextureRect().height));
                    tileBomb.xPos = w;
                    tileBomb.yPos = i;
                    tileBomb.state = 9;
                    arr[i][w] = tileBomb;
                }
            }
        }
    }

    void setDigits() {
        for (int i = 0; i < 16; i++) {
            for (int w = 0; w < 25; w++) {
                if (bomb[i][w] == false) {
                    int noBombs = 0;
                    if (bomb[i - 1][w - 1] == true) {
                        if (w > 0 && i > 0) {
                            noBombs++;
                        }
                    }
                    if (bomb[i - 1][w] == true) {
                        if (i > 0) {
                            noBombs++;
                        }
                    }
                    if (bomb[i - 1][w + 1] == true) {
                        if (w < 24 && i > 0) {
                            noBombs++;
                        }
                    }
                    if (bomb[i][w - 1] == true) {
                        if (w > 0) {
                            noBombs++;
                        }
                    }
                    if (bomb[i][w + 1] == true) {
                        if (w < 24) {
                            noBombs++;
                        }
                    }
                    if (bomb[i + 1][w - 1] == true) {
                        if (w > 0 && i < 15) {
                            noBombs++;
                        }
                    }
                    if (bomb[i + 1][w] == true) {
                        if (i < 15) {
                            noBombs++;
                        }
                    }
                    if (bomb[i + 1][w + 1] == true) {
                        if (w < 24 && i < 15) {
                            noBombs++;
                        }
                    }

                    if (noBombs <= 0) {
                        objt tileU;
                        tileU.t.setTexture(Board::GetTexture("tile_revealed"));
                        tileU.t.setPosition(Vector2f(w * tileU.t.getTextureRect().width, i * tileU.t.getTextureRect().height));
                        tileU.xPos = w;
                        tileU.yPos = i;
                        tileU.state = 0;
                        arr[i][w] = tileU;
                    }
                    else {
                        string name = "number_";
                        name += to_string(noBombs);
                        objt tileDig;
                        tileDig.t.setTexture(Board::GetTexture(name));
                        tileDig.t.setPosition(Vector2f(w * tileDig.t.getTextureRect().width, i * tileDig.t.getTextureRect().height));
                        tileDig.xPos = w;
                        tileDig.yPos = i;
                        tileDig.state = noBombs;
                        arr[i][w] = tileDig;
                    }
                }
            }
        }
    }
};

void loadTestBoard(string filename, tiles& tiley) {
    ifstream file(filename);
    string line;
    int p = 0;
    string temp[16];

    while (getline(file, line))
    {
        stringstream file(line);
        getline(file, temp[p], ' ');
        p++;
    }

    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 25; c++) {
            if ((char)temp[r][c] == '0') {
                tiley.bomb[r][c] = false;
            }
            else if ((char)temp[r][c] == '1') {
                tiley.bomb[r][c] = true;
            }

        }
    }

    for (int i = 0; i < 16; i++) {
        objt tileBomb;
        tileBomb.t.setTexture(Board::GetTexture("mine"));
        for (int w = 0; w < 25; w++) {
            if (tiley.bomb[i][w] == true) {
                tileBomb.t.setPosition(Vector2f(w * tileBomb.t.getTextureRect().width, i * tileBomb.t.getTextureRect().height));
                tileBomb.xPos = w;
                tileBomb.yPos = i;
                tileBomb.state = 9;
                tiley.arr[i][w] = tileBomb;
            }
        }
    }

    tiley.setDigits();
}


bool checkBounds(int r, int c) {
    if (r < 0) { return false; }
    if (r > 15) { return false; }
    if (c < 0) { return false; }
    if (c > 24) { return false; }
    return true;
}

vector<objt> getNeighbors(int r, int c, tiles& tiley) {
    vector<objt> vals;
    
    if (checkBounds(r - 1, c - 1)) {
        vals.push_back(tiley.arr[r - 1][c - 1]);
    }
    if (checkBounds(r, c - 1)) {
        vals.push_back(tiley.arr[r][c - 1]);
    }
    if (checkBounds(r + 1, c - 1)) {
        vals.push_back(tiley.arr[r + 1][c - 1]);
    }
    if (checkBounds(r - 1, c)) {
        vals.push_back(tiley.arr[r - 1][c]);
    }
    if (checkBounds(r + 1, c)) {
        vals.push_back(tiley.arr[r + 1][c]);
    }
    if (checkBounds(r - 1, c + 1)) {
        vals.push_back(tiley.arr[r - 1][c + 1]);
    }
    if (checkBounds(r, c + 1)) {
        vals.push_back(tiley.arr[r][c + 1]);
    }
    if (checkBounds(r + 1, c + 1)) {
        vals.push_back(tiley.arr[r + 1][c + 1]);
    }
    
    return vals;
}

bool hasBombs(vector<objt> neighbors) {

    for (size_t i = 0; i < neighbors.size(); i++) {
        if (neighbors.at(i).state == 9) { return true; }
    }
    return false;

}

void reveal(int r, int c, tiles& tiley) {
    vector<objt> neighbors = getNeighbors(r, c, tiley);
    tiley.arr[r][c].isRevealed = true;
    
    for (size_t i = 0; i < neighbors.size(); i++) {
        if (!hasBombs(neighbors)) {
            if (!neighbors.at(i).isRevealed && neighbors.at(i).state != 9) {
                tiley.arr[neighbors.at(i).yPos][neighbors.at(i).xPos].isRevealed = true;
                reveal(neighbors.at(i).yPos, neighbors.at(i).xPos, tiley);
            }
        }
    }
}

int main()
{
    RenderWindow window(VideoMode(800, 600), "Minesweeper");
    const int LEN = 16;
    const int WID = 25;
    const int BOMB = 50;
    tiles board;
    vector<objt> t_vals(400);
    vector<objt> layerTop(400);
    int flagCount = BOMB;
    bool continueGame = true;
    bool cond = true;
    bool wonGame = false;
    bool inDebug = false;
    int debugCount = 0;
    int ind = 0;
    int testState = 0;
    int whatEv = 0;

    objt tileDef;
    tileDef.t.setTexture(Board::GetTexture("tile_hidden"));

    //---------------Initialize Board-------------------------

    objt tileSmile;
    tileSmile.t.setTexture(Board::GetTexture("face_happy"));
    tileSmile.t.setPosition(Vector2f(11.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileSmile.state = 11;

    objt tileDead;
    tileDead.t.setTexture(Board::GetTexture("face_lose"));
    tileDead.t.setPosition(Vector2f(11.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileDead.state = 12;

    objt tileWin;
    tileWin.t.setTexture(Board::GetTexture("face_win"));
    tileWin.t.setPosition(Vector2f(11.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileWin.state = 20;

    objt tileDebug;
    tileDebug.t.setTexture(Board::GetTexture("debug"));
    tileDebug.t.setPosition(Vector2f(15.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileDebug.state = 13;

    objt tileTest1;
    tileTest1.t.setTexture(Board::GetTexture("test_1"));
    tileTest1.t.setPosition(Vector2f(17.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileTest1.state = 14;

    objt tileTest2;
    tileTest2.t.setTexture(Board::GetTexture("test_2"));
    tileTest2.t.setPosition(Vector2f(19.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileTest2.state = 15;

    objt tileTest3;
    tileTest3.t.setTexture(Board::GetTexture("test_3"));
    tileTest3.t.setPosition(Vector2f(21.5 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileTest3.state = 16;

    objt tileCounter1;
    tileCounter1.t.setTexture(Board::GetTexture("digits"));
    tileCounter1.t.setPosition(Vector2f(0 * tileDef.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileCounter1.state = 0;
    tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));

    objt tileCounter2;
    tileCounter2.t.setTexture(Board::GetTexture("digits"));
    tileCounter2.t.setPosition(Vector2f(tileCounter1.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileCounter2.state = 5;
    tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));

    objt tileCounter3;
    tileCounter3.t.setTexture(Board::GetTexture("digits"));
    tileCounter3.t.setPosition(Vector2f(2 * tileCounter1.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileCounter3.state = 0;
    tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

    objt tileCounter4;
    tileCounter4.t.setTexture(Board::GetTexture("digits"));
    tileCounter4.t.setPosition(Vector2f(3 * tileCounter1.t.getTextureRect().width, LEN * tileDef.t.getTextureRect().height));
    tileCounter4.state = 0;


    while (window.isOpen())
    {
        Event event;
        if (cond == true) {
            board.setBombs();
            board.setDigits();

            ind = 0;
            for (int i = 0; i < LEN; i++) {
                for (int w = 0; w < WID; w++) {
                    t_vals.at(ind) = board.arr[i][w];
                    ind++;
                }
            }

            ind = 0;
            for (int i = 0; i < LEN; i++) {
                for (int w = 0; w < WID; w++) {
                    objt tileC;
                    tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                    tileC.state = -1;
                    tileC.t.setPosition(Vector2f(w * tileC.t.getTextureRect().width, i * tileC.t.getTextureRect().height));
                    layerTop.at(ind) = tileC;
                    ind++;
                }
            }
        }
        ind = 0;
        cond = false;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                objt tileFlag;
                tileFlag.t.setTexture(Board::GetTexture("flag"));
                tileFlag.state = 10;

                objt tileC;
                tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                tileC.state = -1;

                if (event.mouseButton.button == Mouse::Left)
                {
                    auto posL = Mouse::getPosition(window);

                    for (int i = 0; i < layerTop.size(); i++) {
                        if (layerTop.at(i).t.getGlobalBounds().contains(Vector2f(posL.x, posL.y)) && layerTop.at(i).state != 10 && t_vals.at(i).state != 9 && continueGame == true) {
                            //cout << "Left-clicked on index: " << i << endl;
                            if (layerTop.at(i).state != t_vals.at(i).state) {
                                reveal(t_vals.at(i).yPos, t_vals.at(i).xPos, board);                                   
                                ind = 0;
                                for (int i = 0; i < LEN; i++) {
                                    for (int w = 0; w < WID; w++) {
                                        if (board.arr[i][w].isRevealed == true && layerTop.at(ind).state != 10) {
                                            layerTop.at(ind).t = t_vals.at(ind).t;
                                            layerTop.at(ind).state = t_vals.at(ind).state;
                                        }
                                        ind++;
                                    }
                                }
                                //cout << t_vals.at(i).xPos << ", " << t_vals.at(i).yPos << endl;
                            }
                        }
                        else if (layerTop.at(i).t.getGlobalBounds().contains(Vector2f(posL.x, posL.y)) && t_vals.at(i).state == 9 && layerTop.at(i).state != 10 && inDebug == false) {
                            if (wonGame == false) {
                                for (int j = 0; j < layerTop.size(); j++) {
                                    if (t_vals.at(j).state == 9) {
                                        layerTop.at(j).t = t_vals.at(j).t;
                                    }
                                    if (layerTop.at(j).state != 10) {
                                        layerTop.at(j).state = 1000;
                                    }
                                }
                                continueGame = false;
                                inDebug = true;
                                debugCount = 400;
                            }
                        }
                        else if (tileSmile.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y)) || tileDead.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y)) || tileWin.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y))) {
                            cond = true;
                            continueGame = true;
                            inDebug = false;
                            debugCount = 0;

                            tileCounter1.state = 0;
                            tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));

                            tileCounter2.state = 5;
                            tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));

                            tileCounter3.state = 0;
                            tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

                            tileCounter4.state = 0;
                            tileCounter4.t.setTextureRect(IntRect(21 * tileCounter4.state, 0, 21, 32));

                            wonGame = false;
                            flagCount = 50;
                            testState = 0;
                        }
                        else if (tileDebug.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y))) {
                            if (inDebug == false) {
                                debugCount++;
                                if (debugCount == 400) {
                                    inDebug = true;
                                }
                            }
                            else if (inDebug == true) {
                                //inDebug = false;
                                ind = 0;
                                for (int i = 0; i < LEN; i++) {
                                    for (int w = 0; w < WID; w++) {
                                        if (t_vals.at(ind).state == 9) {
                                            objt tileC;
                                            if (layerTop.at(ind).state != 10) {
                                                tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                                                tileC.state = -1;
                                                tileC.t.setPosition(Vector2f(w * tileC.t.getTextureRect().width, i * tileC.t.getTextureRect().height));
                                                layerTop.at(ind) = tileC;
                                            }
                                        }
                                        ind++;
                                    }
                                }

                                debugCount--;
                                if (debugCount == 0) {
                                    inDebug = false;
                                }
                            }
                        }
                        else if (tileTest1.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y))) {
                            loadTestBoard("boards/testboard1.brd", board);
                            if (inDebug == true) {
                                inDebug = false;
                                debugCount = 0;
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    t_vals.at(ind) = board.arr[i][w];
                                    ind++;
                                }
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    objt tileC;
                                    tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                                    tileC.state = -1;
                                    tileC.t.setPosition(Vector2f(w * tileC.t.getTextureRect().width, i * tileC.t.getTextureRect().height));
                                    layerTop.at(ind) = tileC;
                                    ind++;
                                }
                            }

                            tileCounter1.state = 0;
                            tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));

                            tileCounter2.state = 0;
                            tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));

                            tileCounter3.state = 1;
                            tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

                            if (continueGame == false) {
                                continueGame = true;
                            }
                            wonGame = false;
                            flagCount = 1;
                            testState = 1;
                        }
                        else if (tileTest2.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y))) {
                            loadTestBoard("boards/testboard2.brd", board);
                            if (inDebug == true) {
                                inDebug = false;
                                debugCount = 0;
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    t_vals.at(ind) = board.arr[i][w];
                                    ind++;
                                }
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    objt tileC;
                                    tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                                    tileC.state = -1;
                                    tileC.t.setPosition(Vector2f(w * tileC.t.getTextureRect().width, i * tileC.t.getTextureRect().height));
                                    layerTop.at(ind) = tileC;
                                    ind++;
                                }
                            }

                            tileCounter1.state = 0;
                            tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));

                            tileCounter2.state = 9;
                            tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));

                            tileCounter3.state = 8;
                            tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

                            if (continueGame == false) {
                                continueGame = true;
                            }
                            wonGame = false;
                            flagCount = 98;
                            testState = 2;
                        }
                        else if (tileTest3.t.getGlobalBounds().contains(Vector2f(posL.x, posL.y))) {
                            loadTestBoard("boards/testboard3.brd", board);
                            if (inDebug == true) {
                                inDebug = false;
                                debugCount = 0;
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    t_vals.at(ind) = board.arr[i][w];
                                    ind++;
                                }
                            }

                            ind = 0;
                            for (int i = 0; i < LEN; i++) {
                                for (int w = 0; w < WID; w++) {
                                    objt tileC;
                                    tileC.t.setTexture(Board::GetTexture("tile_hidden"));
                                    tileC.state = -1;
                                    tileC.t.setPosition(Vector2f(w * tileC.t.getTextureRect().width, i * tileC.t.getTextureRect().height));
                                    layerTop.at(ind) = tileC;
                                    ind++;
                                }
                            }

                            tileCounter1.state = 0;
                            tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));

                            tileCounter2.state = 8;
                            tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));

                            tileCounter3.state = 3;
                            tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

                            if (continueGame == false) {
                                continueGame = true;
                            }
                            wonGame = false;
                            flagCount = 83;
                            testState = 3;
                        }

                        int winCheck = 0;
                        for (int i = 0; i < layerTop.size(); i++) {
                            if (layerTop.at(i).state == -1 || layerTop.at(i).state == 1000 || layerTop.at(i).state == 10) {
                                winCheck++;
                            }
                        }

                        if (winCheck == 50 && testState == 0 || winCheck == 1 && testState == 1 || winCheck == 98 && testState == 2 || winCheck == 83 && testState == 3) {
                            for (int j = 0; j < layerTop.size(); j++) {
                                if (t_vals.at(j).state == 9) {
                                    tileFlag.t.setPosition(Vector2f(t_vals.at(j).xPos * tileFlag.t.getTextureRect().width, t_vals.at(j).yPos * tileFlag.t.getTextureRect().height));
                                    layerTop.at(j) = tileFlag;
                                    layerTop.at(j).t = tileFlag.t;
                                    layerTop.at(j).state = 20;
                                }
                                else {
                                    layerTop.at(j).state = 1000;
                                }
                            }
                            continueGame = false;
                            wonGame = true;
                            cout << "winner!" << endl;
                            cout << endl;
                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    auto posR = Mouse::getPosition(window);

                    for (int i = 0; i < layerTop.size(); i++) {
                        if (layerTop.at(i).t.getGlobalBounds().contains(Vector2f(posR.x, posR.y)) && layerTop.at(i).state == -1) {
                            //cout << "Right-clicked on index: " << i;
                            tileFlag.t.setPosition(Vector2f(t_vals.at(i).xPos * tileFlag.t.getTextureRect().width, t_vals.at(i).yPos * tileFlag.t.getTextureRect().height));
                            layerTop.at(i) = tileFlag;
                            layerTop.at(i).t = tileFlag.t;
                            layerTop.at(i).state = 10;
                            flagCount--;

                            if (tileCounter3.state == 0) {
                                tileCounter2.state--;
                                tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));
                                tileCounter3.state = 9;
                                tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                            }
                            else {
                                tileCounter3.state--;
                                tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                            }

                            if (flagCount < 0) {

                                tileCounter1.state = 10;
                                tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));
                                tileCounter2.state = 0;
                                tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));
                                tileCounter3.state = 0;
                                tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                                if (-1 * tileCounter4.state == 9) {
                                    tileCounter4.state = 0;
                                    tileCounter4.t.setTextureRect(IntRect(21 * tileCounter4.state, 0, 21, 32));
                                    tileCounter3.state++;
                                    tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                                }
                                else {
                                    tileCounter4.state--;
                                    tileCounter4.t.setTextureRect(IntRect(21 * (-1 * tileCounter4.state), 0, 21, 32));
                                }
                            }

                            cout << flagCount << ", " << tileCounter3.state << endl;
                        }
                        else if (layerTop.at(i).t.getGlobalBounds().contains(Vector2f(posR.x, posR.y)) && layerTop.at(i).state == 10) {
                            //cout << "Right-clicked on index: " << i;
                            tileC.t.setPosition(Vector2f(t_vals.at(i).xPos * tileFlag.t.getTextureRect().width, t_vals.at(i).yPos * tileFlag.t.getTextureRect().height));
                            layerTop.at(i).t = tileC.t;
                            layerTop.at(i).state = -1;
                            flagCount++;
                            if (tileCounter3.state == 9) {
                                tileCounter2.state++;
                                tileCounter2.t.setTextureRect(IntRect(21 * tileCounter2.state, 0, 21, 32));
                                tileCounter3.state = 0;
                                tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                            }
                            else {
                                if (flagCount > 0) {
                                    tileCounter3.state++;
                                    tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));
                                }
                                else if (flagCount == 0) {
                                    tileCounter1.state = 0;
                                    tileCounter1.t.setTextureRect(IntRect(21 * tileCounter1.state, 0, 21, 32));
                                }
                                else if (flagCount == -9) {
                                    tileCounter3.state--;
                                    tileCounter3.t.setTextureRect(IntRect(21 * tileCounter3.state, 0, 21, 32));

                                    tileCounter4.state = -9;
                                    tileCounter4.t.setTextureRect(IntRect(21 * (-1 * tileCounter4.state), 0, 21, 32));
                                    cout << "1st: " << tileCounter4.state << endl;
                                }
                                else
                                {
                                    tileCounter4.state++;
                                    tileCounter4.t.setTextureRect(IntRect(21 * (-1 * tileCounter4.state), 0, 21, 32));
                                    cout << "2nd: " << tileCounter4.state << endl;
                                }
                            }
                            cout << flagCount << ", " << tileCounter3.state << endl;
                        }
                    }
                }
            }
        }

        window.clear(Color::White);

        ind = 0;
        for (int i = 0; i < LEN; i++) {
            for (int w = 0; w < WID; w++) {
                if (layerTop.at(ind).state == 10 || layerTop.at(ind).state == 20) {
                    Sprite tileSpriteC(Board::GetTexture("tile_hidden"));
                    tileSpriteC.setPosition(Vector2f(w * tileSpriteC.getTextureRect().width, i * tileSpriteC.getTextureRect().height));
                    window.draw(tileSpriteC);
                }
                else {
                    Sprite tileSpriteU(Board::GetTexture("tile_revealed"));
                    tileSpriteU.setPosition(Vector2f(w * tileSpriteU.getTextureRect().width, i * tileSpriteU.getTextureRect().height));
                    window.draw(tileSpriteU);
                }
                ind++;
            }
        }

        for (int i = 0; i < LEN * WID; i++) {
            window.draw(layerTop.at(i).t);
            if (inDebug == true && wonGame == false) {
                if (t_vals.at(i).state == 9) {
                    window.draw(t_vals.at(i).t);
                }
            }
        }

        if (wonGame == true) {
            window.draw(tileWin.t);
        }
        else if (continueGame == true) {
            window.draw(tileSmile.t);
        }
        else {
            window.draw(tileDead.t);
        }

        window.draw(tileDebug.t);
        window.draw(tileTest1.t);
        window.draw(tileTest2.t);
        window.draw(tileTest3.t);

        window.draw(tileCounter1.t);
        window.draw(tileCounter2.t);
        window.draw(tileCounter3.t);

        if (flagCount < 0) {
            window.draw(tileCounter4.t);
        }

        window.display();
    }

    return 0;
}