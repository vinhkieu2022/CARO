#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;

const int gridSize = 15;      // Số ô trên mỗi hàng và cột
const int cellSize = 40;      // Kích thước mỗi ô (pixel)
const int winLength = 5;      // Số lượng liên tiếp để thắng

class Board {
private:
    std::vector<std::vector<int>> grid;
    int gridSize;

public:
    Board(int size) : gridSize(size) {
        grid = std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0));
    }

    int getCell(int row, int col) const {
        return grid[row][col];
    }

    void setCell(int row, int col, int value) {
        grid[row][col] = value;
    }

    void reset() {
        grid = std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0));
    }

    bool checkWin(int player) const {
        for (int row = 0; row < gridSize; ++row) {
            for (int col = 0; col < gridSize; ++col) {
                if (grid[row][col] == player) {
                    // Kiểm tra hàng ngang
                    if (col + winLength <= gridSize) {
                        bool win = true;
                        for (int i = 1; i < winLength; ++i) {
                            if (grid[row][col + i] != player) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return true;
                    }

                    // Kiểm tra cột dọc
                    if (row + winLength <= gridSize) {
                        bool win = true;
                        for (int i = 1; i < winLength; ++i) {
                            if (grid[row + i][col] != player) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return true;
                    }

                    // Kiểm tra chéo chính
                    if (row + winLength <= gridSize && col + winLength <= gridSize) {
                        bool win = true;
                        for (int i = 1; i < winLength; ++i) {
                            if (grid[row + i][col + i] != player) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return true;
                    }

                    // Kiểm tra chéo phụ
                    if (row - winLength >= -1 && col + winLength <= gridSize) {
                        bool win = true;
                        for (int i = 1; i < winLength; ++i) {
                            if (grid[row - i][col + i] != player) {
                                win = false;
                                break;
                            }
                        }
                        if (win) return true;
                    }
                }
            }
        }
        return false;
    }
};

class Menu {
private:
    Texture menuTexture;

public:
    bool loadTexture(const std::string& path) {
        return menuTexture.loadFromFile(path);
    }

    void draw(RenderWindow& window) {
        window.clear(Color::White);
        Sprite menuSprite(menuTexture);
        window.draw(menuSprite);
        window.display();
    }

    void handleClick(const Event& event, bool& inMenu, bool& inGame, bool& inGuide, RenderWindow& window) {
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.x >= 140 && event.mouseButton.x <= 210 && event.mouseButton.y >= 540 && event.mouseButton.y <= 600) {
                inMenu = false;  // Vào game
                inGame = true;
            }
            else if (event.mouseButton.x >= 430 && event.mouseButton.x <= 540 && event.mouseButton.y >= 10 && event.mouseButton.y <= 40) {
                inMenu = false;  // Vào hướng dẫn
                inGuide = true;
            }
            else if (event.mouseButton.x >= 400 && event.mouseButton.x <= 500 && event.mouseButton.y >= 540 && event.mouseButton.y <= 590) {
                window.close();  // Thoát game
            }
        }
    }
};

class Guide {
private:
    Texture backgroundTexture;

public:
    bool loadTexture(const std::string& path) {
        return backgroundTexture.loadFromFile(path);
    }

    void draw(RenderWindow& window, Font& font) {
        Sprite backgroundSprite(backgroundTexture);
        window.clear(Color::White);
        window.draw(backgroundSprite);

        Text guideText("Huong dan choi Caro:\n\n"
            "- Nhan chuot trai de danh X hoac O.\n"
            "- Moi nguoi choi thay phien nhau danh.\n"
            "- Nguoi thang la nguoi tao duong 5 dau lien tiep.\n"
            "- Nhan ESC de quay lai menu.",
            font, 24);
        guideText.setFillColor(Color::Black);
        guideText.setPosition(50, 50);

        window.draw(guideText);
        window.display();
    }
};

class Game {
private:
    Board board;
    int currentPlayer;
    bool gameWon;

public:
    Game() : board(gridSize), currentPlayer(1), gameWon(false) {}

    void reset() {
        board.reset();
        currentPlayer = 1;
        gameWon = false;
    }

    void handleInput(const Event& event) {
        if (event.type == Event::MouseButtonPressed && !gameWon) {
            int col = event.mouseButton.x / cellSize;
            int row = event.mouseButton.y / cellSize;

            if (board.getCell(row, col) == 0) {
                board.setCell(row, col, currentPlayer);
                if (board.checkWin(currentPlayer)) {
                    gameWon = true;
                }
                else {
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                }
            }
        }
    }

    void draw(RenderWindow& window, Sprite& cellSprite, Sprite& xSprite, Sprite& oSprite, Font& font) {
        for (int row = 0; row < gridSize; ++row) {
            for (int col = 0; col < gridSize; ++col) {
                cellSprite.setPosition(col * cellSize, row * cellSize);
                window.draw(cellSprite);

                int cellValue = board.getCell(row, col);
                if (cellValue == 1) {
                    xSprite.setPosition(col * cellSize, row * cellSize);
                    window.draw(xSprite);
                }
                else if (cellValue == 2) {
                    oSprite.setPosition(col * cellSize, row * cellSize);
                    window.draw(oSprite);
                }
            }
        }

        if (gameWon) {
            Text winnerText((currentPlayer == 1 ? "X WIN!" : "O WIN!"), font, 150);
            winnerText.setFillColor(Color::Red);
            winnerText.setPosition(100, 200);
            window.draw(winnerText);
        }

        window.display();
    }
};

int main() {
    RenderWindow window(VideoMode(600, 600), "Caro");

    Font font;
    if (!font.loadFromFile("D:/GAME C++/CARO/data/arial.ttf")) {
        std::cerr << "Lỗi tải font!" << std::endl;
        return -1;
    }

    Texture cellTexture, xTexture, oTexture;
    if (!cellTexture.loadFromFile("D:/GAME C++/CARO/data/banco.png") ||
        !xTexture.loadFromFile("D:/GAME C++/CARO/data/dauX.png") ||
        !oTexture.loadFromFile("D:/GAME C++/CARO/data/dauO.png")) {
        std::cerr << "Lỗi tải hình ảnh bàn cờ!" << std::endl;
        return -1;
    }

    Sprite cellSprite(cellTexture);
    Sprite xSprite(xTexture);
    Sprite oSprite(oTexture);
    float scaleFactor = static_cast<float>(cellSize) / std::max(xTexture.getSize().x, xTexture.getSize().y);
    xSprite.setScale(scaleFactor, scaleFactor);
    oSprite.setScale(scaleFactor, scaleFactor);

    Menu menu;
    if (!menu.loadTexture("D:/GAME C++/CARO/data/menu.png")) {
        std::cerr << "Lỗi tải hình ảnh menu!" << std::endl;
        return -1;
    }

    Guide guide;
    if (!guide.loadTexture("D:/GAME C++/CARO/data/guide.png")) {
        std::cerr << "Lỗi tải hình ảnh hướng dẫn!" << std::endl;
        return -1;
    }

    Game game;

    bool inMenu = true;
    bool inGame = false;
    bool inGuide = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                if (inGame || inGuide) {
                    inMenu = true;
                    inGame = false;
                    inGuide = false;
                    game.reset();
                }
            }

            if (inMenu) {
                menu.handleClick(event, inMenu, inGame, inGuide, window);
            }
            else if (inGuide) {
                guide.draw(window, font);
            }
            else if (inGame) {
                game.handleInput(event);
                window.clear();
                game.draw(window, cellSprite, xSprite, oSprite, font);
            }
        }

        if (inMenu) {
            menu.draw(window);
        }
    }

    return 0;
}
