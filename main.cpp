#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

constexpr int resolution = 20;
constexpr int height = 800;
constexpr int width = 800;
constexpr float updateInterval = 1.0f/10.0f; 

using Matrix = std::vector<std::vector<int>>;

int binaryRandom() {
    static std::random_device random;
    static std::mt19937 gen(random());
    static std::bernoulli_distribution distribution(0.5);
    return distribution(gen);
}

sf::RectangleShape drawRect(float x, float y, float width, float height) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    return rect;
}

Matrix newRandomCellGrid(int cols, int rows) {
    Matrix grid(cols, std::vector<int>(rows));
    for (auto& col : grid) {
        for (auto& cell : col) {
            cell = binaryRandom();
        }
    }
    return grid;
}

Matrix newCellGrid(int cols, int rows) {
    return Matrix(cols, std::vector<int>(rows, 0));
}

void drawGrid(sf::RenderWindow& window,const Matrix& grid) {
    int cols = grid.size();
    int rows = grid[0].size();

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            int x = i * resolution;
            int y = j * resolution;
            sf::RectangleShape r = drawRect(x, y, resolution, resolution);
            r.setFillColor(grid[i][j] == 1 ? sf::Color::White : sf::Color::Black);
            window.draw(r);
        }
    }
}

void updateMatrix(Matrix& grid){
    int cols = grid.size();
    int rows = grid[0].size();

    Matrix next = newCellGrid(cols,rows);

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {

            int sum = 0; //total de vizinhos
            if (i > 0 && j > 0) sum += grid[i-1][j-1];                                          // diagonal superior esquerda
            if (i > 0) sum += grid[i-1][j];                                                     // cima
            if (i > 0 && j < cols - 1) sum += grid[i-1][j+1];                                   // diagonal superior direita
            if (j > 0) sum += grid[i][j-1];                                                     // esquerda
            if (j < cols - 1) sum += grid[i][j+1];                                              // direita
            if (i < rows - 1 && j > 0) sum += grid[i+1][j-1];                                   // diagonal inferior esquerda
            if (i < rows - 1) sum += grid[i+1][j];                                              // baixo
            if (i < rows - 1 && j < cols - 1) sum += grid[i+1][j+1];                            // diagonal inferior direita

            int state = grid[i][j];

            if(state == 0 && sum == 3){
                next[i][j] = 1;
            }
            else if (state == 1 &&(sum < 2 || sum > 3)){
                next[i][j]  = 0;
            }
            else{
                next[i][j] = state;
            }
        }
    }
    grid = next;

}

void printGrid(const Matrix& grid) {
    for (const auto& col : grid) {
        for (const auto& cell : col) {
            std::cout << cell;
        }
        std::cout << "\n";
    }
}


int main() {

    sf::RenderWindow window(sf::VideoMode(width, height), "Game of Life");
    sf::Clock clock;
    sf::Time elapsed = sf::Time::Zero;


    int cols = width/resolution;
    int rows = height/resolution;
    Matrix grid = newRandomCellGrid(cols, rows); //<== Inicia
  

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        elapsed += clock.restart();
        if (elapsed.asSeconds() >= updateInterval) {
            updateMatrix(grid);   // <== atualiza
            elapsed -= sf::seconds(updateInterval);
        }

        window.clear();
        drawGrid(window, grid); // <== desenha
        window.display();
    }

    return 0;
}