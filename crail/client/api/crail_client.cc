#include "crail/client/api/crail_client.h"
#include "crail/client/utils/log.h"
#include "crail/client/crail_file.h"
#include "crail/client/crail_table.h"
#include "crail/client/crail_keyvalue.h"

using namespace std;

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

    // realKey
    string realKey = "/gg/" + key;

    // then create the file if previous step OK
    CrailKeyValue keyValue = this->_crailStore->Create<CrailKeyValue>(realKey, 0, 0, enumerable).get();

    if (!keyValue.valid()) {
        log_error("Create() key [%s] failed!, so Set failed", key.c_str());
        log_error("realKey -> [%s]", realKey.c_str());
        ret = -1;
        return ret;
    }

    // save value content to a buffer
    unique_ptr<CrailOutputstream> outputstream = keyValue.outputstream();
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

    // realKey
    string realKey = "/gg/" + key;
    
    // lookup file
    CrailKeyValue keyValue = this->_crailStore->Lookup<CrailKeyValue>(realKey).get();
    if (!keyValue.valid()) {
        log_error("Lookup() key [%s] failed!, so Get failed");
        log_error("realKey -> [%s]", realKey.c_str());
        ret = -1;
        return ret;
    }

    // save stream to string
    unique_ptr<CrailInputstream> inputstream = keyValue.inputstream();
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

// need to invoke before Get or Set, only need to invoke one time
int CrailClient::InitCrailTable(void){
    int ret = 0;

    string table_path = "/gg";

    CrailTable tb = this->_crailStore->Lookup<CrailTable>(table_path).get();

    if (!tb.valid()) {
        CrailTable crailTable = this->_crailStore->Create<CrailTable>(table_path, 0, 0, false).get();
        if (!crailTable.valid()) {
            log_error("invoke CreateTable() to create table [%s] failed!", table_path.c_str());
            return -1;
        }
    }else{
        log_trace("table [%s] already exist", table_path.c_str());
    }

    return ret;
}