#ruledef property{
	x => 0x00
	y => 0x01
	w => 0x02
	h => 0x03
	color => 0x04
	visibility => 0x05
	type => 0x06
	cmd_asset => 0x07
	txt_asset => 0x08
	screen_w => 0xD0
	screen_h => 0xD1
	screen_bg => 0xD2
	millis => 0xD3
	micros => 0xD4
	draw_color => 0xF0
	draw_x => 0xF1
	draw_y => 0xF2
	draw_x2 => 0xF3
	draw_y2 => 0xF4
}

#ruledef address{
	{addr} => addr`16
}

#ruledef string{
	{string} => ((len(string)/8)+1)`16 @ string  @ 0x00
}

#ruledef
{
    jmp {value:address} => 0xF0 @ value
	jeq {value:address} => 0xF1 @ value
	jnq {value:address} => 0xF2 @ value
	jgt {value:address} => 0xF3 @ value
	jge {value:address} => 0xF4 @ value
	jls {value:address} => 0xF5 @ value
	jle {value:address} => 0xF6 @ value
	
	load A, {value:u16} => 0xE0 @ value`16
	load B, {value:u16} => 0xE1 @ value`16
	
	load A, {value:u8} => 0xE6 @ value`8
	load B, {value:u8} => 0xE7 @ value`8
	
	load A, {prop:property} => 0xE2 @ prop
	load B, {prop:property} => 0xE3 @ prop

	load {prop:property}, A => 0xE4 @ prop
	load {prop:property}, B => 0xE5 @ prop
	
	load A, B => 0xE8
	load B, A => 0xE9
	swp => 0xEA

	load {prop:property}, {value:u16} => 0xEB @ prop @ value
	load {prop:property}, {value:u8} => 0xEC @ prop @ value
	
	sleep {delay:u16} => 0xFF @ delay`16
	sleep {delay:u8} => 0xFC @ delay`8
	sleep A => 0xFD 
	sleep B => 0xFE 
	
	add => 0xD0
	sub => 0xD1
	mul => 0xD2
	div => 0xD3
	mod => 0xD4
	
	inc {value:u16} => 0xD5 @ value`16
	dec {value:u16} => 0xD6 @ value`16
	
	inc {value:u8} => 0xD9 @ value`8
	dec {value:u8} => 0xDA @ value`8
	
	inc {prop:property}, {value:u16} => 0xD7 @ prop @ value`16
	dec {prop:property}, {value:u16} => 0xD8 @ prop @ value`16
	
	inc {prop:property}, {value:u8} => 0xDB @ prop @ value`8
	dec {prop:property}, {value:u8} => 0xDC @ prop @ value`8
	
	rnd => 0xC0
	rnd {value:u16} => 0xC1 @ value
	rnd {value:u8} => 0xC2 @ value
	
	rnd {min:u16}, {max:u16} => 0xC3 @ min @ max
	rnd {min:u8}, {max:u8} => 0xC4 @ min @ max
	
	rnd B => 0xC5
	rnd A, B => 0xC6
	
	debug => 0x10

	hang => 0x11
}

#ruledef
{
	draw A, B, draw_color => 0xB1
	draw => 0xB4
	draw {x:u16}, {y:u16}, {color:u8} => 0xB0 @ x @ y @ color
	
	line => 0xB6
	line {x:u16}, {y:u16}, {x2:u16}, {y2:u16}, {color:u8} => 0xB7 @ x @ y @ x2 @ y2 @ color 

	rect => 0xB8
	rect {x:u16}, {y:u16}, {w:u16}, {h:u16}, {color:u8} => 0xB9 @ x @ y @ w @ h @ color 
	
	read A, B => 0xB3
	read {x:u16}, {y:u16} => 0xB2 @ x @ y
	read => 0xB5	
}

#ruledef 
{
	write_asset {asset}, {str:string} => 0xA0 @ asset`8 @ str
}