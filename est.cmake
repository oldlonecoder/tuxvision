set(Target "est")
set(CMAKE_CXX_STANDARD 20)

message("Entering tools lib configuration:")


message("Current SourceDir:" ${CMAKE_CURRENT_SOURCE_DIR})


set(EST_HFILES
    src/include/${Work}/${Target}/alu.h
    src/include/${Work}/${Target}/node.h
    src/include/${Work}/${Target}/variable.h
    src/include/${Work}/${Target}/bloc.h
    src/include/${Work}/${Target}/expression.h
)


add_library("${Work}.${Target}" SHARED

            #-----------TOOLS:
            ${LEXER_HFILES}
            src/include/${Work}/${Target}/alu.cc
            src/include/${Work}/${Target}/node.cc
            src/include/${Work}/${Target}/variable.cc
            src/include/${Work}/${Target}/bloc.cc
            src/include/${Work}/${Target}/expression.cc

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


target_link_libraries("${Work}.${Target}" ${CMAKE_DL_LIBS} tuxvision.lexer  ) # and other system dependencies...


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

INSTALL(FILES ${EST_HFILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${Work}/${Target}")
