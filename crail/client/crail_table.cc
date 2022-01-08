#include "crail/client/crail_table.h"

#include <iostream>
#include <memory>

#include "crail/client/crail_inputstream.h"

using std::shared_ptr;

CrailTable::CrailTable(FileInfo file_info,
                        shared_ptr<NamenodeClient> namenode_client,
                        shared_ptr<StorageCache> storage_cache,
                        shared_ptr<BlockCache> block_cache)
    : CrailNode(file_info, namenode_client, storage_cache, block_cache) {}

CrailTable::~CrailTable() {}