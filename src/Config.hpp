#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <windows.h>

struct Config {
    int gridWidth{ 7 };
    int gridHeight{ 7 };
    int foodCount{ 1 };
    float tickInterval{ 1.15f };
    int restartKeyCode{ 'F' };
    std::string restartKeyName{ "F" };
    std::string headSymbol{ "\xF0\x9F\x90\xB1" };       // ??
    std::string bodySymbol{ "\xE2\x9A\xAA" };           // ?
    std::string foodSymbol{ "\xF0\x9F\x90\x9F" };       // ??
    std::string emptySymbol{ "\xE2\x96\xAB\xEF\xB8\x8F" }; // ??

    void SaveDefault(const std::string& path) const {
        std::ofstream file(path);
        if (!file.is_open()) return;

        file << "[Game]\n"
             << "// Grid width (default: 7)\n"
             << "grid_width = 7\n\n"
             << "// Grid height (default: 7)\n"
             << "grid_height = 7\n\n"
             << "// Simultaneous food count (default: 1)\n"
             << "food_count = 1\n\n"
             << "// Update interval in seconds (default: 1.15)\n"
             << "tick_interval = 1.15\n\n"
             << "[Controls]\n"
             << "// Restart key (default: F)\n"
             << "restart_key = F\n\n"
             << "[Appearance]\n"
             << "// Head symbol (default: ??)\n"
             << "head_symbol = ??\n\n"
             << "// Body symbol (default: ?)\n"
             << "body_symbol = ?\n\n"
             << "// Food symbol (default: ??)\n"
             << "food_symbol = ??\n\n"
             << "// Empty tile symbol (default: ??)\n"
             << "empty_symbol = ??\n";
    }

    bool Load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            SaveDefault(path);
            return true;
        }

        std::string line;
        while (std::getline(file, line)) {
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }));
            line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), line.end());

            if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') continue;
            if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;

            const size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key = line.substr(0, eqPos);
            std::string val = line.substr(eqPos + 1);

            auto Trim = [](std::string& s) {
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
                s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
            };
            Trim(key);
            Trim(val);

            if (key == "grid_width") {
                gridWidth = std::clamp(std::stoi(val), 4, 12);
            } else if (key == "grid_height") {
                gridHeight = std::clamp(std::stoi(val), 4, 10);
            } else if (key == "food_count") {
                foodCount = std::clamp(std::stoi(val), 1, 10);
            } else if (key == "tick_interval") {
                tickInterval = std::clamp(std::stof(val), 0.5f, 5.0f);
            } else if (key == "restart_key") {
                restartKeyName = val;
                restartKeyCode = ParseKeyCode(val);
            } else if (key == "head_symbol" && !val.empty()) {
                headSymbol = val;
            } else if (key == "body_symbol" && !val.empty()) {
                bodySymbol = val;
            } else if (key == "food_symbol" && !val.empty()) {
                foodSymbol = val;
            } else if (key == "empty_symbol" && !val.empty()) {
                emptySymbol = val;
            }
        }
        return true;
    }

private:
    static int ParseKeyCode(const std::string& key) noexcept {
        if (key.empty()) return 'F';
        std::string upperKey = key;
        for (char& c : upperKey) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

        if (upperKey == "SPACE") return VK_SPACE;
        if (upperKey == "ENTER" || upperKey == "RETURN") return VK_RETURN;
        if (upperKey == "TAB") return VK_TAB;
        if (upperKey == "SHIFT") return VK_SHIFT;
        if (upperKey == "CTRL" || upperKey == "CONTROL") return VK_CONTROL;

        if (upperKey.size() == 1 && upperKey[0] >= 'A' && upperKey[0] <= 'Z') {
            return upperKey[0];
        }
        if (upperKey.size() == 1 && upperKey[0] >= '0' && upperKey[0] <= '9') {
            return upperKey[0];
        }
        return 'F';
    }
};
