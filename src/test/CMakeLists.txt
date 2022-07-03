file(GLOB_RECURSE SRC *.cpp *.hpp)
set(TEST_APP ${PROJECT_NAME}-test)
add_executable(${TEST_APP} ${SRC})
target_link_libraries(${TEST_APP} Catch2 Catch2Main)
