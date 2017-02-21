# Add uninstall target cmake module
# Usage: sudo make uninstall
# Unix users can run this uninstall as well:
# sudo xargs rm < install_manifest.txt

function(add_uninstall_target)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endfunction()