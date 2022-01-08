set(CMAKE_CXX_FLAGS "-O3 -g -D_GLIBCXX_USE_CXX11_ABI=1")

add_library (crail STATIC 
	client/crail_store.cc
	client/crail_node.cc
	client/crail_file.cc
	client/crail_directory.cc
	client/crail_table.cc
	client/crail_keyvalue.cc
	client/crail_outputstream.cc
	client/crail_inputstream.cc
	client/directory_record.cc
	client/common/block_cache.cc
	client/namenode/namenode_request.cc
	client/namenode/namenode_response.cc
	client/namenode/create_request.cc
	client/namenode/create_response.cc
	client/namenode/lookup_request.cc
	client/namenode/lookup_response.cc
	client/namenode/getblock_request.cc
	client/namenode/getblock_response.cc
	client/namenode/setfile_request.cc
	client/namenode/void_response.cc
	client/namenode/remove_request.cc
	client/namenode/remove_response.cc
	client/namenode/ioctl_request.cc
	client/namenode/ioctl_response.cc
	client/namenode/narpc/narpc_namenode_client.cc
	client/storage/storage_cache.cc
	client/storage/narpc/narpc_storage_client.cc
	client/storage/narpc/narpc_storage_request.cc
	client/storage/narpc/narpc_storage_response.cc
	client/storage/narpc/narpc_write_request.cc
	client/storage/narpc/narpc_write_response.cc
	client/storage/narpc/narpc_read_request.cc
	client/storage/narpc/narpc_read_response.cc
	client/metadata/filename.cc
	client/metadata/file_info.cc
	client/metadata/datanode_info.cc
	client/metadata/block_info.cc
	client/utils/micro_clock.cc
	client/utils/crail_hash.cc
	client/utils/crail_networking.cc
	client/ioutils/byte_buffer.cc
	narpc/narpc_staging.cc
	narpc/network_utils.cc
	narpc/rpc_client.cc
	client/utils/log.cc
	client/api/crail_client.cc
)

install(TARGETS crail DESTINATION /usr/local/lib)

install (FILES 
	client/crail_store.h
	client/crail_node.h
	client/crail_file.h
	client/crail_outputstream.h
	client/crail_inputstream.h
	client/directory_record.h
	client/crail_table.h
	client/crail_keyvalue.h
	DESTINATION /usr/local/include/crail/client)
install (FILES
	client/common/file_type.h
	client/common/crail_constants.h
	client/common/future.h
	client/common/serializable.h
	client/common/block_cache.h
	DESTINATION /usr/local/include/crail/client/common)
install (FILES
	client/metadata/block_info.h
	client/metadata/datanode_info.h
	client/metadata/file_info.h
	client/metadata/filename.h 
	DESTINATION /usr/local/include/crail/client/metadata)
install (FILES
	client/namenode/namenode_client.h
	client/namenode/create_response.h
	client/namenode/lookup_response.h
	client/namenode/getblock_response.h
	client/namenode/remove_response.h
	client/namenode/void_response.h
	client/namenode/namenode_response.h
	client/namenode/ioctl_response.h
	DESTINATION /usr/local/include/crail/client/namenode)
install (FILES
	client/storage/storage_client.h
	client/storage/storage_cache.h
	DESTINATION /usr/local/include/crail/client/storage)
install (FILES
	client/utils/crail_networking.h
	DESTINATION /usr/local/include/crail/client/utils)
install (FILES
	client/ioutils/byte_buffer.h
	DESTINATION /usr/local/include/crail/client/ioutils)
install (FILES
	narpc/narpc_staging.h
	narpc/network_utils.h
	narpc/rpc_client.h
	narpc/rpc_message.h
	DESTINATION /usr/local/include/crail/narpc)
install (FILES
	client/api/crail_client.h
	DESTINATION /usr/local/include/crail/client/api)
install (FILES
	client/utils/log.h
	DESTINATION /usr/local/include/crail/client/utils)

install (FILES
	libcrail.pc
	DESTINATION /usr/local/lib/pkgconfig/)
