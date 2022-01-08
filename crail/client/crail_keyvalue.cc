#include "crail/client/crail_keyvalue.h"

CrailKeyValue::CrailKeyValue(FileInfo file_info,
                            shared_ptr<NamenodeClient> namenode_client,
                            shared_ptr<StorageCache> storage_cache,
                            shared_ptr<BlockCache> block_cache)
    : CrailNode(file_info, namenode_client, storage_cache, block_cache) {}

unique_ptr<CrailOutputstream> CrailKeyValue::outputstream() {
    return make_unique<CrailOutputstream>(namenode_client_, storage_cache_, block_cache_, file_info_, 0);
}

unique_ptr<CrailInputstream> CrailKeyValue::inputstream() {
    return make_unique<CrailInputstream>(namenode_client_, storage_cache_, block_cache_, file_info_, 0);
}
