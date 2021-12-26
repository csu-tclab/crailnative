#include "crail/client/api/crail_client.h"
#include "crail/client/utils/log.h"
#include "crail/client/crail_file.h"

CrailClient::CrailClient(std::string ip, uint16_t port)  : _crailStore(nullptr), _addr(""), _port(0), _connected(false) {
    // network info
    this->_addr = ip;
    this->_port = port;

    log_info("ADDR -> [%s], PORT -> [%d]", this->_addr.c_str(), this->_port);
    // no auth for now

    // create CrailStore obj
    this->_crailStore.reset(new CrailStore(this->_addr, this->_port));
}

CrailClient::~CrailClient() {
    try {
        this->Disconnect();
    } catch(...) {
        // ignore exceptions
    }
}

int CrailClient::Connect() {
    int ret = 0;

    if (this->_connected) {
        log_debug("already connected! abort");
        return 0;
    }

    log_debug("Connect to [%s]:[%d] begin", this->_addr.c_str(), this->_port);
    // invoke connect
    ret = this->_crailStore->Initialize();
    log_debug("Connect to [%s]:[%d] end", this->_addr.c_str(), this->_port);

    // check result
    if (ret < 0) {
        log_error("_crailStore->Initialize() failed, ret -> [%d]", ret);
    }

    // free server_list
    return ret;
}

int CrailClient::Disconnect() {
    // invoke disconnect
    // CrailStore object will invoke Close() when distruct, we don't have to handle this

    return 0;
}

int CrailClient::Set(std::string key, const std::string &value) {
    int ret = 0;
    bool enumerable = false;

    // we need to handle duplicate key manually, WTF
    auto lookup_ret = this->_crailStore->Lookup<CrailFile>(key).get();
    if (lookup_ret.valid()) {
        log_info("key [%s] found dup, will remove at first", key.c_str());
        auto remove_ret = this->_crailStore->Remove(key, true);
        if (remove_ret != 0) {
            log_error("Remove() key [%s] failed!, so Set failed", key.c_str());
            ret = -1;
            return ret;
        }
    }

    // then create the file if previous step OK
    auto crailFile = this->_crailStore->Create<CrailFile>(key, 0, 0, enumerable).get();
    if (!crailFile.valid()) {
        log_error("Create() key [%s] failed!, so Set failed", key.c_str());
        ret = -1;
        return ret;
    }

    // save value content to a buffer
    unique_ptr<CrailOutputstream> outputstream = crailFile.outputstream();
    shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(value.length() + 1);
    buf->PutBytes(value.c_str(), value.length());
    buf->Flip();

    // send to remote server
    while(buf->remaining() > 0) {
        if (outputstream->Write(buf).get() < 0) {
            log_error("Write() return failed, so Set failed for key[%s]", key.c_str());
            
            // invoke manually here, avoid leak
            outputstream->Close().get();
            ret = -1;
            return ret;
        }
    }

    // clear buffer
    buf->Clear();

    // close stream
    outputstream->Close().get();

    return ret;
}

int CrailClient::Get(std::string key, std::string &value) {
    int ret = 0;
    
    // lookup file
    auto crailFile = this->_crailStore->Lookup<CrailFile>(key).get();
    if (!crailFile.valid()) {
        log_error("Lookup() key [%s] failed!, so Get failed");
        ret = -1;
        return ret;
    }

    // save stream to string
    unique_ptr<CrailInputstream> inputstream = crailFile.inputstream();
    shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);

    while (inputstream->Read(buf).get() > 0) {
        buf->Flip();
        value.append(reinterpret_cast<const char*>(buf->get_bytes()), buf->remaining());
        buf->Clear();
    }

    // close stream
    inputstream->Close();

    return ret;
}