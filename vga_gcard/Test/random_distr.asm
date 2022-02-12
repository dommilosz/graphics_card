load B, screen_w
rnd B
load draw_x, A
load B, screen_h
rnd B
load draw_y, A
read
inc draw_color, 1
draw

jmp 0