load own_time, 255

BOARD_SIZE=40
playerX = 4
playerY = 5
drawX = 6
drawY = 7
drawPtr = 8
dir_asset = 9
fstart = 10
diff = 11

load mem[dir_asset], text_asset
inc mem[dir_asset], 1

load mem[playerX], 5
load mem[playerY], 5

start:
load mem[fstart], millis
call draw_map
call move_player
load mem[diff], millis
dec mem[diff], mem[fstart]
sleep 100
debug mem[diff]
jmp start

draw_map:
clear_obj
load assets[text_asset], ""
load mem[drawX], 0
load mem[drawY], 0
load mem[drawPtr], 0

draw_line:
load assets[text_asset][mem[drawPtr]], (repeat(".",BOARD_SIZE`8)@"\n")
inc mem[drawY], 1
inc mem[drawPtr], BOARD_SIZE+1
jle draw_line, mem[drawY], BOARD_SIZE

load mem[drawPtr], BOARD_SIZE+1
mul mem[drawPtr], mem[drawPtr], mem[playerY]
inc mem[drawPtr], mem[playerX]
load assets[text_asset][mem[drawPtr]], "@"

draw_obj
ret

move_player:
load C, assets[mem[dir_asset]][0]
jeq move_w, C, 8
jeq move_s, C, 2
jeq move_a, C, 4
jeq move_d, C, 6

jmp move_after
move_s:
jge move_after, mem[playerY], BOARD_SIZE
inc mem[playerY], 1
jmp move_after
move_w:
jle move_after, mem[playerY], 0
dec mem[playerY], 1
jmp move_after
move_a:
jle move_after, mem[playerX], 0
dec mem[playerX], 1
jmp move_after
move_d:
jge move_after, mem[playerX], BOARD_SIZE-1
inc mem[playerX], 1

move_after:
ret