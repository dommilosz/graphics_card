cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(./arduino-pico/pico-sdk/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

project(vga_graphics_card)

add_executable(vga_graphics_card
    src/main.cpp
)

include_directories(./src/)
include_directories(./libraries/I2CCom/)
include_directories(./libraries/Wire/src/)
include_directories(./arduino-pico/cores/rp2040/)
include_directories(./arduino-pico/variants/rpipico/)

target_sources(vga_graphics_card PUBLIC ./arduino-pico/cores/rp2040/wiring_digital.cpp)
target_sources(vga_graphics_card PUBLIC ./libraries/Wire/src/Wire.cpp)
target_sources(vga_graphics_card PUBLIC ./arduino-pico/cores/rp2040/delay.cpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "./libraries/_picovga/*.cpp")
file(GLOB_RECURSE asm_sources CONFIGURE_DEPENDS "./libraries/_picovga/*.S")
#file(GLOB_RECURSE src_sources CONFIGURE_DEPENDS "./src/*.cpp")
#file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

target_sources(vga_graphics_card PUBLIC ${sources})
target_sources(vga_graphics_card PUBLIC ${asm_sources})
target_sources(vga_graphics_card PUBLIC ${src_sources})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(vga_graphics_card pico_stdlib)
target_link_libraries(vga_graphics_card pico_multicore pico_stdio_usb)
target_link_libraries(vga_graphics_card cmsis_core)
target_link_libraries(vga_graphics_card hardware_pio hardware_adc hardware_dma hardware_flash hardware_i2c hardware_interp hardware_pwm hardware_rtc hardware_spi hardware_exception)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(vga_graphics_card)