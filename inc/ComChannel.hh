#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include <iostream>
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
};

#endif