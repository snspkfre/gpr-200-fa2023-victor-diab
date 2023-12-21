#Transformations example

file(
 GLOB_RECURSE PERSONAL_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE PERSONAL_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this personal_project's asset folder to bin when it is built
add_custom_target(copyAssetsPersonal ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${ASSIGNMENT7_INC} DESTINATION include/personal_project)
add_executable(personal_project ${PERSONAL_SRC} ${PERSONAL_INC} ${PERSONAL_ASSETS})
target_link_libraries(personal_project PUBLIC core IMGUI)
target_include_directories(personal_project PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when personal_project is built
add_dependencies(personal_project copyAssetsPersonal)