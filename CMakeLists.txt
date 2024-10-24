cmake_minimum_required(VERSION 3.13..3.29 FATAL_ERROR)
project(tuxvision VERSION 0.1.0 LANGUAGES CXX)
set(BaseTarget ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

message("Entering ${PROJECT_NAME} project configuration:")
include(GNUInstallDirs)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${BaseTarget} STATIC  #SHARED
            include/${BaseTarget}/exports
            include/${BaseTarget}/tools/colors.h src/tools/colors.cc
            include/${BaseTarget}/tools/glyphes.h src/tools/glyphes.cc
            include/${BaseTarget}/tools/cadres.h src/tools/cadres.cc
            include/${BaseTarget}/tools/string.h src/tools/string.cc
            include/${BaseTarget}/tools/geometry.h src/tools/geometry.cc
            include/${BaseTarget}/journal/book.h src/journal/book.cc
            include/${BaseTarget}/tools/cmdargs.h src/tools/cmdargs.cc
            include/${BaseTarget}/tools/sscan.h src/tools/sscan.cc
            include/${BaseTarget}/tools/object.h src/tools/object.cc
            include/${BaseTarget}/tools/signals.h
            # include/${BaseTarget}/ui/input_track.h src/tui/input_track.cc
            # include/${BaseTarget}/ui/window.h src/tui/window.cc
            # include/${BaseTarget}/ui/desktop.h src/tui/desktop.cc
            # include/${BaseTarget}/tui/ansi_to_chr.h  src/tui/ansi_to_chr.cc
            # include/${BaseTarget}/ui/statusbar.h src/tui/statusbar.cc

            include/${BaseTarget}/lexer/lexer_types.h src/lexer/lexer_types.cc
            include/${BaseTarget}/lexer/token_data.h src/lexer/token_data.cc
            include/${BaseTarget}/lexer/tokens_table.h src/lexer/tokens_table.cc
            include/${BaseTarget}/lexer/lexer.h src/lexer/lexer.cc
            include/${BaseTarget}/est/alu.h src/est/alu.cc
            include/${BaseTarget}/est/node.h src/est/node.cc
            include/${BaseTarget}/est/variable.h src/est/variable.cc
            include/${BaseTarget}/est/bloc.h src/est/bloc.cc
            include/${BaseTarget}/est/expression.h src/est/expression.cc
            include/${BaseTarget}/ui/globals.h src/ui/globals.cc
            include/${BaseTarget}/ui/terminal.h src/ui/terminal.cc  src/ui/terminal.vchar.cc
            include/${BaseTarget}/ui/events.h src/ui/events.cc
            include/${BaseTarget}/ui/application.h src/ui/application.cc
            include/${BaseTarget}/ui/widget.h src/ui/widget.cc src/ui/widget_painter.cc
            include/${BaseTarget}/ui/widget/label.h src/ui/widget/label.cc
)

target_compile_definitions(${BaseTarget} PUBLIC "${BaseTarget}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header(${BaseTarget} EXPORT_FILE_NAME ${${BaseTarget}_export})

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()

target_include_directories(${BaseTarget} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           $<INSTALL_INTERFACE:include>
)

target_link_libraries(${BaseTarget} ${CMAKE_DL_LIBS}) # and other system dependencies...








install(DIRECTORY
        include/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)


add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
    install_manifest.txt to be uninstalled!
)
# Enable packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME ${BaseTarget})
set(CPACK_GENERATOR "TGZ")

# Include CPack
#include(CPack) # Scrap!! No way to include header files!

add_subdirectory(tddv)
