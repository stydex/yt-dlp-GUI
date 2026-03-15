cmake_minimum_required(VERSION 3.16)
project(yt-dlp_project VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(yt-dlp_projects 
    main.cpp
    MainWindow.cpp
    MainWindow.h
    )

    
target_link_libraries(yt-dlp_projects PRIVATE Qt6::Widgets)
