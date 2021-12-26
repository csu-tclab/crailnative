#ifndef _CRAIL_CLIENT_H
#define _CRAIL_CLIENT_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

#include "crail/client/crail_store.h"

class CrailClient {
public:
    explicit CrailClient(std::string ip, uint16_t port);
    ~CrailClient();
    
    // non-copyable
    CrailClient(const CrailClient& memcachedClient) = delete;
    CrailClient& operator=(const CrailClient& memcachedClient) = delete;

    int Connect();
    int Disconnect();
    int Set(std::string key, const std::string &value);
    int Get(std::string key, std::string &value);
private:
    // libcrail connect instance
    std::shared_ptr<CrailStore> _crailStore;

    // addr
    std::string _addr;

    // port
    uint16_t _port;

    // 
    bool _connected;
};

#endif // _CRAIL_CLIENT_H