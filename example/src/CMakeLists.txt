add_executable(fanuc_example main.c)

set(DEPS "fwlib32" "config")
if (UNIX)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set_target_properties(fanuc_example PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
  endif()
  list(APPEND DEPS "m" "pthread")
endif()

target_link_libraries(fanuc_example ${DEPS})

if (WIN32)
  # copy dependency dlls
  get_filename_component(dep_dlls ${CMAKE_SOURCE_DIR} DIRECTORY)
  file(GLOB dep_dlls "${dep_dlls}/fwlib*.dll")
  list(APPEND dep_dlls "${CMAKE_SOURCE_DIR}/extern/libconfig/build/out/Release/libconfig.dll")
  foreach(dep_dll IN LISTS dep_dlls)
    add_custom_command(TARGET fanuc_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dep_dll} $<TARGET_FILE_DIR:fanuc_example>)
  endforeach()
endif()
