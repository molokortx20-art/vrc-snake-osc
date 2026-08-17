#pragma once
#include <string>
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "SnakeEngine.hpp"

#pragma comment(lib, "ws2_32.lib")

class OscReceiver {
public:
    OscReceiver() noexcept : _socket(INVALID_SOCKET), _initialized(false) {}
    ~OscReceiver() noexcept { Cleanup(); }

    [[nodiscard]] bool Initialize(uint16_t port = 9001) noexcept {
        _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (_socket == INVALID_SOCKET) return false;

        // Non-blocking socket mode for zero overhead in hot loop
        u_long mode = 1;
        ioctlsocket(_socket, FIONBIO, &mode);

        sockaddr_in bindAddr{};
        bindAddr.sin_family = AF_INET;
        bindAddr.sin_port = htons(port);
        bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(_socket, reinterpret_cast<const sockaddr*>(&bindAddr), sizeof(bindAddr)) == SOCKET_ERROR) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            return false;
        }

        _initialized = true;
        return true;
    }

    void Poll(SnakeEngine& engine, bool& requestRestart) noexcept {
        if (!_initialized || _socket == INVALID_SOCKET) return;

        uint8_t buffer[1024];
        sockaddr_in fromAddr{};
        int fromLen = sizeof(fromAddr);

        while (true) {
            int bytes = recvfrom(_socket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0,
                                 reinterpret_cast<sockaddr*>(&fromAddr), &fromLen);
            if (bytes <= 0) break;

            ParseOscPacket(buffer, bytes, engine, requestRestart);
        }
    }

    void Cleanup() noexcept {
        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
        _initialized = false;
    }

private:
    SOCKET _socket;
    bool _initialized;

    static void ParseOscPacket(const uint8_t* data, int size, SnakeEngine& engine, bool& requestRestart) noexcept {
        if (size < 8) return;

        std::string address;
        int i = 0;
        while (i < size && data[i] != 0) {
            address.push_back(static_cast<char>(data[i++]));
        }
        while (i < size && data[i] == 0) i++; // Align 4 bytes

        if (i >= size || data[i] != ',') return;
        std::string typeTag;
        while (i < size && data[i] != 0) {
            typeTag.push_back(static_cast<char>(data[i++]));
        }
        while (i < size && data[i] == 0) i++; // Align 4 bytes

        if (typeTag.size() < 2 || i + 4 > size) return;

        float floatVal = 0.0f;
        int intVal = 0;

        if (typeTag[1] == 'f') {
            uint32_t raw = (static_cast<uint32_t>(data[i]) << 24) |
                           (static_cast<uint32_t>(data[i + 1]) << 16) |
                           (static_cast<uint32_t>(data[i + 2]) << 8) |
                           static_cast<uint32_t>(data[i + 3]);
            memcpy(&floatVal, &raw, 4);
        } else if (typeTag[1] == 'i') {
            intVal = (static_cast<int>(data[i]) << 24) |
                     (static_cast<int>(data[i + 1]) << 16) |
                     (static_cast<int>(data[i + 2]) << 8) |
                     static_cast<int>(data[i + 3]);
            floatVal = static_cast<float>(intVal);
        } else if (typeTag[1] == 'T') {
            floatVal = 1.0f;
        } else if (typeTag[1] == 'F') {
            floatVal = 0.0f;
        }

        // VR controller thumbstick inputs
        if (address == "/input/Vertical") {
            if (floatVal > 0.35f) engine.SetDirection(Direction::Up);
            else if (floatVal < -0.35f) engine.SetDirection(Direction::Down);
        } else if (address == "/input/Horizontal") {
            if (floatVal > 0.35f) engine.SetDirection(Direction::Right);
            else if (floatVal < -0.35f) engine.SetDirection(Direction::Left);
        } else if (address == "/input/Jump" && floatVal > 0.5f) {
            requestRestart = true;
        }
    }
};
