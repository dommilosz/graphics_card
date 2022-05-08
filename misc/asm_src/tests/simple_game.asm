
load own_time, 8
load SP, 0

BOARD_SIZE=30
playerX = 4
playerY = 5

fruitX = 12
fruitY = 13

drawX = 6
drawY = 7
drawPtr = 8
dir_asset = 9
fstart = 10
diff = 11

points = 14

load mem[dir_asset], text_asset
inc mem[dir_asset], 1
load C, assets[mem[dir_asset]][0]

load mem[playerX], 5
load mem[playerY], 5
load mem[points], 0

call gen_fruit

start:
call draw_map
call move_player
sleep 1000
jmp start

draw_map:
clear_obj
load assets[text_asset], ""
load mem[drawX], 0
load mem[drawY], 0
load mem[drawPtr], 0

draw_line:
load assets[text_asset][mem[drawPtr]], (repeat(strcolor(rgb(100,100,100))@strcolor(rgb(100,100,100))@".",(BOARD_SIZE)`8)@"  \n")
Offset = 5
Increment = Offset*BOARD_SIZE+3
inc mem[drawPtr], Increment
inc mem[drawY], 1
jle draw_line, mem[drawY], BOARD_SIZE

dec mem[drawPtr], 1
load assets[text_asset][mem[drawPtr]], strcolor(rgb(0,255,0))@strcolor(rgb(0,255,0))@"\n\nPoints: "
inc mem[drawPtr], 14
loaditos assets[text_asset][mem[drawPtr]], mem[points]

load mem[drawPtr], Increment
mul mem[drawPtr], mem[drawPtr], mem[playerY]
load A, mem[playerX]
mul A, A, Offset
dec A, 1
inc mem[drawPtr], A
load assets[text_asset][mem[drawPtr]], strcolor(rgb(255,0,255))@"@"

load mem[drawPtr], Increment
mul mem[drawPtr], mem[drawPtr], mem[fruitY]
load A, mem[fruitX]
mul A, A, Offset
dec A, 1
inc mem[drawPtr], A
load assets[text_asset][mem[drawPtr]], strcolor(rgb(255,255,0))@"%"

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
jge game_over, mem[playerY], BOARD_SIZE
inc mem[playerY], 1
jmp move_after
move_w:
jle game_over, mem[playerY], 0
dec mem[playerY], 1
jmp move_after
move_a:
jle game_over, mem[playerX], 0
dec mem[playerX], 1
jmp move_after
move_d:
jge game_over, mem[playerX], BOARD_SIZE-1
inc mem[playerX], 1

move_after:

jnq fruit_no, mem[fruitX], mem[playerX]
jnq fruit_no, mem[fruitY], mem[playerY]
inc mem[points], 1
call gen_fruit

fruit_no:

ret

gen_fruit:
rnd mem[fruitX], 0, BOARD_SIZE
rnd mem[fruitY], 0, BOARD_SIZE
ret

game_over:
clear_obj
load assets[mem[dir_asset]][0], 0
load assets[text_asset],1, strcolor(rgb(255,0,0))@"Game over. press 6 to restart...\nPress 0 to exit\nScore:..."
loaditos assets[text_asset][56],1, mem[points]
draw_obj
.game_over2:
load C, assets[mem[dir_asset]][0]
jeq 0, C, 6
sleep 500

jmp .game_over2
