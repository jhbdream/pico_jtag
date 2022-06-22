cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(jtag_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(jtag
  main.c
  unique.c
  dap_jtag_pio.c
  DAP/Firmware/Source/DAP_vendor.c
  DAP/Firmware/Source/DAP.c
  DAP/Firmware/Source/JTAG_DP.c
  DAP/Firmware/Source/SW_DP.c
  usb/dev_lowlevel.c
)

target_include_directories(jtag PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	DAP/Firmware/Include
	DAP/Firmware/Config
  usb
)

target_link_libraries(jtag PRIVATE
  pico_stdlib
  cmsis_core
  hardware_dma
  hardware_gpio
  pico_unique_id
  hardware_pio
  pico_time
)

#pico_generate_pio_header(jtag ${CMAKE_CURRENT_LIST_DIR}/jtag.pio)
pico_generate_pio_header(jtag ${CMAKE_CURRENT_LIST_DIR}/dap_jtag.pio)

pico_add_extra_outputs(jtag)

pico_enable_stdio_usb(jtag 0)
pico_enable_stdio_uart(jtag 1)
