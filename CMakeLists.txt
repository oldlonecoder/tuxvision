cmake_minimum_required(VERSION 3.13..3.30 FATAL_ERROR)
## Project name
set(Work "tuxvision")

project(${Work} VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(main.cmake)
