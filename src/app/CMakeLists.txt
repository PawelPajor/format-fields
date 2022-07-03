file(GLOB_RECURSE SRC *.cpp *.hpp)
set(APP_NAME ${PROJECT_NAME}-app)
add_executable(${APP_NAME} ${SRC})
target_link_libraries(${APP_NAME})
