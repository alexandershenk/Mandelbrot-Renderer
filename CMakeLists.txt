cmake_minimum_required (VERSION 2.6)
project (mandel)

find_package(OpenGL REQUIRED)

add_executable(your_exe_name
    main.cpp
    init.cpp
	init.h
)

