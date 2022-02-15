IF NOT EXIST build/ (
   echo build/ doesn't exist. Creating...
   mkdir build
)
cd build && cmake -G "Unix Makefiles" .. && make vga_graphics_card 