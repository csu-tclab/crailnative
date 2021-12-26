set(CMAKE_CXX_FLAGS "-O0 -g -D_GLIBCXX_USE_CXX11_ABI=1")

add_library (disni STATIC
	rdma_endpoint.cc
	)

target_link_libraries (disni LINK_PUBLIC ibverbs)

install (FILES 
	rdma_endpoint.h
	DESTINATION /usr/local/include/disni)

