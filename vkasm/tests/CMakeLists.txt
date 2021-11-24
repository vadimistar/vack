cmake_minimum_required(VERSION 3.10)

add_test(NAME vkasm_add COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/add.vkasm)
add_test(NAME vkasm_sub COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/sub.vkasm)
add_test(NAME vkasm_goto COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/goto.vkasm)
add_test(NAME vkasm_if_eq COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/if_eq.vkasm)
add_test(NAME vkasm_swp COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/swp.vkasm)
add_test(NAME vkasm_call COMMAND vkasm ${PROJECT_SOURCE_DIR}/examples/call.vkasm)