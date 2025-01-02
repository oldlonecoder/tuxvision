set(Target "io")
set(CMAKE_CXX_STANDARD 20)

message("Entering tools lib configuration:")


message("Current SourceDir:" ${CMAKE_CURRENT_SOURCE_DIR})


set(IO_HFILES
    src/include/${Work}/${Target}/sscan.h

)


add_library("${Work}.${Target}" SHARED

            #-----------TOOLS:
            ${IO_HFILES}
            src/include/${Work}/${Target}/sscan.cc
)


target_include_directories("${Work}.${Target}" PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>"
                           $<INSTALL_INTERFACE:include/${Work}/${Target}>
)

target_compile_definitions("${Work}.${Target}" PUBLIC "${Work}.${Target}_DEBUG=$<CONFIG:Debug>")
include(GenerateExportHeader)
generate_export_header("${Work}.${Target}" EXPORT_FILE_NAME "${Work}.${Target}_export")

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    )
ENDIF ()


target_link_libraries("${Work}.${Target}" ${CMAKE_DL_LIBS} tuxvision.core) # and other system dependencies...


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

INSTALL(FILES ${IO_HFILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${Work}/${Target}")
