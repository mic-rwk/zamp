#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include <iostream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>

class ComChannel : public AbstractComChannel {
    int _Socket{-1};
    std::mutex _Mutex;

public:
    ComChannel() = default;
    ~ComChannel() override = default;

    void Init(int Socket) override;
    int GetSocket() const override;
    void LockAccess() override;
    void UnlockAccess() override;
    std::mutex & UseGuard() override;

    bool Connect(const std::string &addr, int port) {
        struct sockaddr_in server;
        _Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_Socket < 0) return false;

        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(addr.c_str());

        if (connect(_Socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
            std::cerr << "Cannot connect to server on port " << port << "\n";
            return false;
        }
        return true;
    }

    bool Send(const std::string &msg) {
        LockAccess();
        ssize_t toSend = msg.size();
        const char *data = msg.c_str();

        while (toSend > 0) {
            ssize_t sent = write(_Socket, data, toSend);
            if (sent < 0) {
                std::cerr << "[ComChannel] Błąd wysyłania danych przez socket.\n";
                UnlockAccess();
                return false;
            }
            toSend -= sent;
            data += sent;
        }

        UnlockAccess();
        return true;

    }

    void Close() {
        if (_Socket >= 0) close(_Socket);
        _Socket = -1;
    }
};

#endif