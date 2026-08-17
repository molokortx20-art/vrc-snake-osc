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
#include "OscReceiver.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Config cfg;
    cfg.Load("config.ini");

    std::cout << "Connecting to VRChat OSC on 127.0.0.1:9000...\n";

    OscSender oscSender;
    if (!oscSender.Initialize("127.0.0.1", 9000)) {
        std::cerr << "[ERROR] Failed to initialize Winsock UDP sender!\n";
        return 1;
    }

    OscReceiver oscReceiver;
    (void)oscReceiver.Initialize(9001); // Listen for VR controller inputs from VRChat

    std::cout << "[OK] OSC connected. Game running in background.\n";
    std::cout << "Discord: thedeathforxxx\n";
    std::cout << "GitHub: https://github.com/molokortx20-art\n";

    SnakeEngine engine(cfg);
    auto lastTick = std::chrono::high_resolution_clock::now();

    while (true) {
        bool vrRestart = false;
        oscReceiver.Poll(engine, vrRestart);

        // Restart on configured keyboard key or VR controller Jump
        if (vrRestart || (GetAsyncKeyState(cfg.restartKeyCode) & 0x8000)) {
            engine.ResetGame();
            oscSender.SendChatboxMessage(engine.RenderToVrcChatbox(), false);
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
            oscSender.SendChatboxMessage(engine.RenderToVrcChatbox(), false);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
