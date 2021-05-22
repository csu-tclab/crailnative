set(CMAKE_CXX_FLAGS "-fPIC -O3 -g -D_GLIBCXX_USE_CXX11_ABI=0")
add_executable(crailnative_test 
	crailnative_test.cc
    crailnative_test.hh
	)
target_link_libraries (crailnative_test LINK_PUBLIC crail)
install(TARGETS crailnative_test DESTINATION /bin)
