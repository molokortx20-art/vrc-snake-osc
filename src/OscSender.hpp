#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class OscSender {
public:
    OscSender() noexcept : _socket(INVALID_SOCKET), _initialized(false) {}
    ~OscSender() noexcept { Cleanup(); }

    [[nodiscard]] bool Initialize(const std::string& ip = "127.0.0.1", uint16_t port = 9000) noexcept {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return false;
        }

        _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (_socket == INVALID_SOCKET) {
            WSACleanup();
            return false;
        }

        memset(&_targetAddr, 0, sizeof(_targetAddr));
        _targetAddr.sin_family = AF_INET;
        _targetAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &_targetAddr.sin_addr);

        _initialized = true;
        return true;
    }

    void SendChatboxMessage(const std::string& utf8Text, bool sendTypingNotification = false) noexcept {
        if (!_initialized || _socket == INVALID_SOCKET) return;

        std::vector<uint8_t> packet;
        packet.reserve(64 + utf8Text.size());
        
        AppendOscString(packet, "/chatbox/input");
        AppendOscString(packet, ",sTF");
        AppendOscString(packet, utf8Text);

        sendto(_socket, reinterpret_cast<const char*>(packet.data()), static_cast<int>(packet.size()), 0,
               reinterpret_cast<const sockaddr*>(&_targetAddr), sizeof(_targetAddr));
    }

    void Cleanup() noexcept {
        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
        if (_initialized) {
            WSACleanup();
            _initialized = false;
        }
    }

private:
    SOCKET _socket;
    sockaddr_in _targetAddr;
    bool _initialized;

    static void AppendOscString(std::vector<uint8_t>& buf, const std::string& str) noexcept {
        for (char c : str) buf.push_back(static_cast<uint8_t>(c));
        buf.push_back(0);
        while (buf.size() % 4 != 0) buf.push_back(0);
    }
};
