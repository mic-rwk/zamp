#include "../inc/ComChannel.hh"

void ComChannel::Init(int Socket) {
    _Socket = Socket;
    std::cout << "[ComChannel] Inicjalizowano socket: " << _Socket << "\n";
}

int ComChannel::GetSocket() const  {
    return _Socket;
}

void ComChannel::LockAccess() {
    _Mutex.lock();
}

void ComChannel::UnlockAccess() {
    _Mutex.unlock();
}

std::mutex & ComChannel::UseGuard() {
    return _Mutex;
}