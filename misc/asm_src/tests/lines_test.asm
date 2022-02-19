#include "../main.asm"

load A, 0
load draw_x, A
load draw_y, A
next:
read
inc draw_color, 1
draw
inc draw_x, 1
load A, draw_x
load B, screen_w
jls next
load A, 0
load draw_x, A
inc draw_y, 1

load A, draw_y
load B, screen_h
jge 0

jmp next