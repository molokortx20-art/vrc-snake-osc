#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "Config.hpp"
#include "SnakeEngine.hpp"
#include "OscSender.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Config cfg;
    cfg.Load("config.ini");

    std::cout << "Connecting to VRChat OSC on 127.0.0.1:9000...\n";

    OscSender osc;
    if (!osc.Initialize("127.0.0.1", 9000)) {
        std::cerr << "[ERROR] Failed to initialize Winsock UDP socket!\n";
        return 1;
    }

    std::cout << "[OK] OSC connected. Game running in background.\n";
    std::cout << "Discord: thedeathforxxx\n";
    std::cout << "GitHub: https://github.com/molokortx20-art\n";

    SnakeEngine engine(cfg);
    auto lastTick = std::chrono::high_resolution_clock::now();

    bool running = true;
    while (running) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;
        }

        // Restart game on configured key
        if (GetAsyncKeyState(cfg.restartKeyCode) & 0x8000) {
            engine.ResetGame();
            osc.SendChatboxMessage(engine.RenderToVrcChatbox(), false);
            lastTick = std::chrono::high_resolution_clock::now();
        }

        const bool up    = (GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000);
        const bool down  = (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000);
        const bool left  = (GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000);
        const bool right = (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000);

        if (up)    engine.SetDirection(Direction::Up);
        if (down)  engine.SetDirection(Direction::Down);
        if (left)  engine.SetDirection(Direction::Left);
        if (right) engine.SetDirection(Direction::Right);

        const auto now = std::chrono::high_resolution_clock::now();
        const float elapsed = std::chrono::duration<float>(now - lastTick).count();

        if (elapsed >= cfg.tickInterval) {
            lastTick = now;
            engine.Update();
            osc.SendChatboxMessage(engine.RenderToVrcChatbox(), false);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}
