@echo off

IF NOT EXIST build/ (
   echo build/ doesn't exist. Creating...
   mkdir build
)
echo Building...
cd build && cmake -G "Unix Makefiles" .. && make vga_graphics_card 