cmake_minimum_required(VERSION 3.10)

add_test(NAME vkasm_add COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/add.vkasm)