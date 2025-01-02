set(Target "core")
set(CMAKE_CXX_STANDARD 20)

message("Entering tools lib configuration:")


message("Current SourceDir:" ${CMAKE_CURRENT_SOURCE_DIR})


set(CORE_HFILES
           src/include/tuxvision/_start_.h
           src/include/tuxvision/rem.h
           src/include/tuxvision/colors.h
           src/include/tuxvision/glyphes.h
           src/include/tuxvision/cadres.h
           src/include/tuxvision/string.h
           src/include/tuxvision/journal/logger.h
           src/include/tuxvision/object.h
           src/include/tuxvision/geometry.h
           src/include/tuxvision/delegator.h
           src/include/tuxvision/cmdargs.h
           src/include/tuxvision/expect.h
           #src/include/tuxvision/io/iofd.h
           #src/include/tuxvision/io/ipoll.h
           #src/include/tuxvision/io/tcpsocket.h
)


add_library("${Work}.${Target}" SHARED

            #-----------TOOLS:
            ${CORE_HFILES}
            src/include/tuxvision/rem.cc
            src/include/tuxvision/colors.cc
            src/include/tuxvision/glyphes.cc
            src/include/tuxvision/cadres.cc
            src/include/tuxvision/string.cc
            src/include/tuxvision/journal/logger.cc
            src/include/tuxvision/object.cc
            src/include/tuxvision/geometry.cc
            src/include/tuxvision/cmdargs.cc
            #src/include/tuxvision/io/iofd.cc
            #src/include/tuxvision/io/ipoll.cc
            #src/include/tuxvision/io/tcpsocket.cc

            # # --------- LEXER:
        #     include/${Work}/${Target}/lexer/lexer_types.h src/lexer/lexer_types.cc
        #     include/${Work}/${Target}/lexer/token_data.h src/lexer/token_data.cc
        #     include/${Work}/${Target}/lexer/tokens_table.h src/lexer/tokens_table.cc
        #     include/${Work}/${Target}/lexer/lexer.h src/lexer/lexer.cc
        #     # # --------- EST : { Expression Syntax Tree }
        #     include/${Work}/${Target}/est/alu.h src/est/alu.cc
        #     include/${Work}/${Target}/est/node.h src/est/node.cc
        #     include/${Work}/${Target}/est/variable.h src/est/variable.cc
        #     include/${Work}/${Target}/est/bloc.h src/est/bloc.cc
        #     include/${Work}/${Target}/est/expression.h src/est/expression.cc
            # # --------- TUI :
            #     include/${Work}/${Target}/tui/terminal.h src/tui/terminal.cc
            #     include/${Work}/${Target}/tui/vchar.h src/tui/terminal.vchar.cc
            #     include/${Work}/${Target}/tui/events.h src/tui/events.cc
            #     include/${Work}/${Target}/tui/widgets/threads_pool.h src/tui/widgets/threads_pool.cc
            #     include/${Work}/${Target}/tui/application.h src/tui/application.cc
            #     include/${Work}/${Target}/tui/widget.h src/tui/widget.cc src/tui/widget_painter.cc
            #     include/${Work}/${Target}/tui/widgets/label.h src/tui/widgets/label.cc
            #     include/${Work}/${Target}/tui/widgets/icon.h src/tui/widgets/icon.cc
            #     include/${Work}/${Target}/tui/widgets/window.h src/tui/widgets/window.cc
            #     include/${Work}/${Target}/tui/widgets/screen.h src/tui/widgets/screen.cc
            #     include/${Work}/${Target}/tui/widgets/status_bar.h src/tui/widgets/status_bar.cc
            #     include/${Work}/${Target}/tui/widgets/input_track.h src/tui/widgets/input_track.cc
            #     include/${Work}/${Target}/tui/io/ansi_colours_parser.h src/tui/io/ansi_colours_parser.cc
            #     include/${Work}/${Target}/tui/io/ansi_parser.h src/tui/io/ansi_parser.cc
            #     #include/${Work}/${Target}/tui/io/polling.h src/tui/io/polling.cc
            #     include/${Work}/${Target}/tui/tuxns.h src/tui/tuxns.cc
            #     include/${Work}/${Target}/tui/io/iobloc.h src/tui/io/iobloc.cc
            #     include/${Work}/${Target}/tui/io/ipoll.h      src/tui/io/ipoll.cc

)


target_include_directories("${Work}.${Target}" PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>"
                           $<INSTALL_INTERFACE:include/${Work}>
)

target_compile_definitions("${Work}.${Target}" PUBLIC "${Work}.${Target}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header("${Work}.${Target}" EXPORT_FILE_NAME "${Work}.${Target}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()


target_link_libraries("${Work}.${Target}" ${CMAKE_DL_LIBS} ) # and other system dependencies...


# add_executable(lus++.app
#     tddv.app/tddv.h tddv.app/tddv.cc
# )

# target_link_libraries(lus++.app lus)


install(TARGETS "${Work}.${Target}"
        EXPORT "${Work}.${Target}"
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        )

INSTALL(FILES ${CORE_HFILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${Work}")


# install(DIRECTORY
#         include/
#         DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
# )

#add_subdirectory(tests)


#add_custom_target("uninstall" COMMENT "Uninstall installed files")
#add_custom_command(
#   TARGET "uninstall"
#   POST_BUILD
#   COMMENT "Uninstall files with install_manifest.txt"
#   COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
#   install_manifest.txt to be uninstalled!
#)

## Enable packaging
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PACKAGE_NAME ${Target})
#set(CPACK_GENERATOR "TGZ")

# Include CPack
#include(CPack) # Scrap!! No way to include header files!
