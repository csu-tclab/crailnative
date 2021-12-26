set(CMAKE_CXX_FLAGS "-O0 -g -D_GLIBCXX_USE_CXX11_ABI=1")

add_executable(iobench 
	iobench.cc
)

target_link_libraries (iobench LINK_PUBLIC crail)
install(TARGETS iobench DESTINATION /usr/bin)
