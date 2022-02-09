#ruledef property{
	x => 0x00
	y => 0x01
	w => 0x02
	h => 0x03
	color => 0x04
	visibility => 0x05
	type => 0x06
}

#ruledef
{
    jmp {value} => 0xF0 @ value`8
	jeq {value} => 0xF1 @ value`8
	jnq {value} => 0xF2 @ value`8
	jgt {value} => 0xF3 @ value`8
	jge {value} => 0xF4 @ value`8
	jls {value} => 0xF5 @ value`8
	jle {value} => 0xF6 @ value`8
	
	load A, {value} => 0xE0 @ value`16
	load B, {value} => 0xE1 @ value`16
	
	load A, {prop:property} => 0xE2 @ prop
	load B, {prop:property} => 0xE3 @ prop
	
	load {prop:property}, A => 0xE4 @ prop
	load {prop:property}, B => 0xE5 @ prop
	
	sleep {delay} => 0xFF @ delay`16
	
	add => 0xD0
	sub => 0xD1
	mul => 0xD2
	div => 0xD3
	mod => 0xD4
}


start:

right:
sleep 100
load A, x
load B, 300
jgt left
load B, 5
add
load x, A
jmp right
left:
sleep 100
load A, x
load B, 10
jls right
load B, 5
sub
load x, A

jmp left