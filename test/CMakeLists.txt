set(CMAKE_CXX_FLAGS "-O0 -g -D_GLIBCXX_USE_CXX11_ABI=1")

add_executable(crailnative_test 
	crailnative_test.cc
    crailnative_test.hh
)

target_link_libraries (crailnative_test LINK_PUBLIC crail)
install(TARGETS crailnative_test DESTINATION /usr/bin)
