cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(serprog_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(serprog
  main.c
  led.c
  pio_jtag.c
)

target_include_directories(serprog PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(serprog PRIVATE
  pico_stdlib
  hardware_dma
  hardware_pio
)

pico_generate_pio_header(serprog ${CMAKE_CURRENT_LIST_DIR}/jtag.pio)

pico_add_extra_outputs(serprog)

pico_enable_stdio_usb(serprog 1)
pico_enable_stdio_uart(serprog 0)