cmake_minimum_required(VERSION 3.5)
project (mips_machine)

set(SOURCES
    main.cc
    ${PROJECT_SOURCE_DIR}/assembler/assembler.cc
    ${PROJECT_SOURCE_DIR}/disassembler/disassembler.cc
    ${PROJECT_SOURCE_DIR}/interface/interface.cc
    ${PROJECT_SOURCE_DIR}/shared/number.cc
    ${PROJECT_SOURCE_DIR}/shared/util.cc
    ${PROJECT_SOURCE_DIR}/simulator/simulator.cc
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Curses REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/assembler
    ${PROJECT_SOURCE_DIR}/disassembler
    ${PROJECT_SOURCE_DIR}/interface
    ${PROJECT_SOURCE_DIR}/shared
    ${PROJECT_SOURCE_DIR}/simulator
)
