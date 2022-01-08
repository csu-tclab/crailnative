#ifndef CRAIL_TABLE_H
#define CRAIL_TABLE_H

#include "crail/client/common/file_type.h"
#include "crail/client/crail_node.h"
#include "crail/client/metadata/file_info.h"
#include "crail/client/namenode/namenode_client.h"
#include "crail/client/storage/storage_cache.h"

using std::shared_ptr;

class CrailTable : public CrailNode {
public:
  constexpr static FileType type = FileType::Table;

  CrailTable() = default;
  CrailTable(FileInfo file_info, shared_ptr<NamenodeClient> namenode_client, shared_ptr<StorageCache> storage_cache, shared_ptr<BlockCache> block_cache);

  virtual ~CrailTable();
};

#endif /* CRAIL_TABLE_H */