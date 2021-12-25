set(CMAKE_CXX_FLAGS "-O0 -g -D_GLIBCXX_USE_CXX11_ABI=0")

add_library (ioutils STATIC
	byte_buffer.cc
)

#install(TARGETS narpc DESTINATION /usr/local/lib)
install (FILES 
	byte_buffer.h
	DESTINATION /usr/local/include/ioutils)

