
message (${Work} ":  ** Building the framework ")


OPTION(CORE "Build the tuxvision's core library" ON)
OPTION(LEXER "Build the tuxvision's lexer library" OFF)
OPTION(EST "Build the tuxvision's EST (Expression [abstract] Syntax Tree) library" OFF)
OPTION(TUXVISION "Build the tuxvision's TUI (Text User Interface) library" OFF)



IF(CORE)
    message ("******* Building the core library *******")
    include(core.cmake)
ENDIF()

IF(OOMIO)
    message ("******* Building the oom::io basic support library *******")
    include(oomio.cmake)
ENDIF()

IF(LEXER)
    message ("******* Building the lexer library *******")
    include(lexer.cmake)
ENDIF()


IF(EST)
    message ("******* Building the est library *******")
    include(est.cmake)
ENDIF()

IF(TUXVISION)
    message ("******* Building the tuxvision library *******")
    include(tuxvision.cmake)
ENDIF()

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files under tuxvision framework"
    COMMAND rm -vrf ${CMAKE_INSTALL_PREFIX}/include/${Work} ${CMAKE_INSTALL_PREFIX}/lib/lib${Work}* || echo Nothing in install_manifest.txt to be uninstalled!
)