#pragma once
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Config.hpp"

struct Point {
    int x, y;
    [[nodiscard]] constexpr bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

enum class Direction : uint8_t {
    Up,
    Down,
    Left,
    Right
};

class SnakeEngine {
public:
    explicit SnakeEngine(const Config& cfg) noexcept : _cfg(cfg) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        ResetGame();
    }

    void ResetGame() noexcept {
        _snake.clear();
        _snake.push_back({ _cfg.gridWidth / 2, _cfg.gridHeight / 2 });
        _snake.push_back({ _cfg.gridWidth / 2 - 1, _cfg.gridHeight / 2 });
        _snake.push_back({ _cfg.gridWidth / 2 - 2, _cfg.gridHeight / 2 });

        _dir = Direction::Right;
        _nextDir = Direction::Right;
        _score = 0;
        _gameOver = false;
        _foods.clear();
        for (int i = 0; i < _cfg.foodCount; ++i) {
            SpawnFood();
        }
    }

    void SetDirection(Direction newDir) noexcept {
        if (_gameOver) return;
        // Disallow immediate 180-degree reversal
        if ((_dir == Direction::Up && newDir != Direction::Down) ||
            (_dir == Direction::Down && newDir != Direction::Up) ||
            (_dir == Direction::Left && newDir != Direction::Right) ||
            (_dir == Direction::Right && newDir != Direction::Left)) {
            _nextDir = newDir;
        }
    }

    void Update() noexcept {
        if (_gameOver) return;

        _dir = _nextDir;
        Point nextHead = _snake.front();

        switch (_dir) {
            case Direction::Up:    nextHead.y--; break;
            case Direction::Down:  nextHead.y++; break;
            case Direction::Left:  nextHead.x--; break;
            case Direction::Right: nextHead.x++; break;
        }

        // Screen boundary wrap
        if (nextHead.x < 0) nextHead.x = _cfg.gridWidth - 1;
        if (nextHead.x >= _cfg.gridWidth) nextHead.x = 0;
        if (nextHead.y < 0) nextHead.y = _cfg.gridHeight - 1;
        if (nextHead.y >= _cfg.gridHeight) nextHead.y = 0;

        for (size_t i = 0; i < _snake.size() - 1; ++i) {
            if (_snake[i] == nextHead) {
                _gameOver = true;
                return;
            }
        }

        _snake.push_front(nextHead);

        // Check food collision
        auto foodIt = std::find(_foods.begin(), _foods.end(), nextHead);
        if (foodIt != _foods.end()) {
            _score += 1;
            if (_score > _highScore) _highScore = _score;
            _foods.erase(foodIt);

            if (_snake.size() >= static_cast<size_t>(_cfg.gridWidth * _cfg.gridHeight)) {
                _gameOver = true;
            } else {
                SpawnFood();
            }
        } else {
            _snake.pop_back();
        }
    }

    [[nodiscard]] std::string RenderToVrcChatbox() const {
        std::string result;
        result.reserve(256);

        for (int y = 0; y < _cfg.gridHeight; ++y) {
            for (int x = 0; x < _cfg.gridWidth; ++x) {
                Point p{ x, y };
                if (p == _snake.front()) {
                    result += _cfg.headSymbol;
                } else if (std::find(_foods.begin(), _foods.end(), p) != _foods.end()) {
                    result += _cfg.foodSymbol;
                } else {
                    bool isBody = false;
                    for (size_t i = 1; i < _snake.size(); ++i) {
                        if (_snake[i] == p) {
                            isBody = true;
                            break;
                        }
                    }
                    result += isBody ? _cfg.bodySymbol : _cfg.emptySymbol;
                }
            }
            result.push_back('\n');
        }

        char status[128];
        if (_gameOver) {
            snprintf(status, sizeof(status), "😿 SCORE: %d | %s - Restart", _score, _cfg.restartKeyName.c_str());
        } else {
            snprintf(status, sizeof(status), "%s SCORE: %d | BEST: %d", _cfg.headSymbol.c_str(), _score, _highScore);
        }
        result += status;
        return result;
    }

private:
    const Config& _cfg;
    std::deque<Point> _snake;
    std::vector<Point> _foods;
    Direction _dir{ Direction::Right };
    Direction _nextDir{ Direction::Right };
    int _score{ 0 };
    int _highScore{ 0 };
    bool _gameOver{ false };

    void SpawnFood() noexcept {
        std::vector<Point> freeCells;
        freeCells.reserve(_cfg.gridWidth * _cfg.gridHeight);

        for (int y = 0; y < _cfg.gridHeight; ++y) {
            for (int x = 0; x < _cfg.gridWidth; ++x) {
                Point p{ x, y };
                bool occupied = false;
                for (const auto& s : _snake) {
                    if (s == p) { occupied = true; break; }
                }
                if (!occupied && std::find(_foods.begin(), _foods.end(), p) == _foods.end()) {
                    freeCells.push_back(p);
                }
            }
        }
        if (!freeCells.empty()) {
            _foods.push_back(freeCells[std::rand() % freeCells.size()]);
        }
    }
};
