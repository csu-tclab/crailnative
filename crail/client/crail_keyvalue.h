#ifndef CRAIL_KEYVALUE_H
#define CRAIL_KEYVALUE_H

#include "crail/client/common/file_type.h"
#include "crail/client/crail_node.h"
#include "crail/client/metadata/file_info.h"
#include "crail/client/namenode/namenode_client.h"
#include "crail/client/storage/storage_cache.h"
#include "crail/client/crail_inputstream.h"

class CrailKeyValue : public CrailNode {
public:
    constexpr static FileType type = FileType::KeyValue;

    CrailKeyValue() = default;
    CrailKeyValue(FileInfo file_info, shared_ptr<NamenodeClient> namenode_client,
                    shared_ptr<StorageCache> storage_cache,
                    shared_ptr<BlockCache> block_cache);

    virtual ~CrailKeyValue() = default;

    unique_ptr<CrailOutputstream> outputstream();
    unique_ptr<CrailInputstream> inputstream();
};

#endif /* CRAIL_KEYVALUE_H */