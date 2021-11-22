cmake_minimum_required(VERSION 3.10)

add_test(NAME vkasm_add COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/add.vkasm)
add_test(NAME vkasm_sub COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/sub.vkasm)
add_test(NAME vkasm_goto COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/goto.vkasm)