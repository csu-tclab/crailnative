set(CMAKE_CXX_FLAGS "-O0 -g -D_GLIBCXX_USE_CXX11_ABI=0")

add_library (narpc STATIC
	rpc_client.cc
	network_utils.cc
	narpc_staging.cc
)

target_link_libraries (narpc LINK_PUBLIC ioutils)

#install(TARGETS narpc DESTINATION /usr/local/lib)
install (FILES 
	rpc_client.h
	rpc_message.h
	DESTINATION /usr/local/include/narpc)

