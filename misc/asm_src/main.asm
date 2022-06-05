#fn str(v) => v @ 0`8
#fn byte_len(v) => (len(v)/8)
#fn strcolor(v) => 22`8 @ v
#fn strcolorreset() => 23`8
#fn rgb(r,g,b)=> ((r*8/256)`3 @ (g*8/256)`3 @ (b*4/256)`2)`8

#ruledef property_single{
	x => 0x00
	y => 0x01
	w => 0x02
	h => 0x03
	color => 0x04
	visibility => 0x05
	type => 0x06
	code_asset => 0x07
	text_asset => 0x08
	screen_w => 0xA0
	screen_h => 0xA1
	screen_bg => 0xA2
	millis => 0xA3
	micros => 0xA4
	
	mem[{index:u8}] => {
		assert(index < 31)
		(0xD0 + index)`8
	}
	A => (0xD0 + 0)`8
	B => (0xD0 + 1)`8
	C => (0xD0 + 2)`8
	D => (0xD0 + 3)`8
	own_time => (0xD0+25)`8
	PC => (0xD0 + 26)`8
	SP => (0xD0 + 27)`8
}

#ruledef property_number{
	{number:u8} => {0xFF @ number}
	{number:u16} => {0xFE @ number}
}

#ruledef property{
	{prop:property_single}=> prop
	{number:property_number} => number
}

#ruledef bool
{
	true => 0x01
	false => 0x00
	{v:u8}=>{
		assert(v==1)
		0x01
	}
	{v:u8}=>{
		assert(v==0)
		0x00
	}
}

#ruledef asset_nooffset
{
	assets[{asset:property}] => 
	{
		assert(asset > 0);
		asset
	}
}

#ruledef asset
{
	assets[{asset:property}] => 
	{
		assert(asset > 0);
		asset @ 0xFF @0`8
	}
	assets[{asset:property}][{offset:property}] => 
	{
		assert(asset > 0);
		asset @ offset
	}
	
}

#ruledef address{
	{addr} => addr`16
}

#ruledef aligned_data{
	{data} => {
		assert((len(data) % 8) == 0)
		data
	}
	{data} => {
		assert((len(data) % 8) == 4)
		data @ 0`4
	}
}

#ruledef string{
	{string:aligned_data} => {
		0xFE @ byte_len(string)`16 @ string
	}
	0{string:aligned_data}=>0xFE @ byte_len(string)`16 @ 0`8
}

#ruledef
{
    jmp {value:property} => 0xF0 @ value
	jeq {value:property}, {prop1:property}, {prop2:property} => 0xF1 @ value @ prop1 @ prop2
	jnq {value:property}, {prop1:property}, {prop2:property} => 0xF2 @ value @ prop1 @ prop2
	jgt {value:property}, {prop1:property}, {prop2:property} => 0xF3 @ value @ prop1 @ prop2
	jge {value:property}, {prop1:property}, {prop2:property} => 0xF4 @ value @ prop1 @ prop2
	jls {value:property}, {prop1:property}, {prop2:property} => 0xF5 @ value @ prop1 @ prop2
	jle {value:property}, {prop1:property}, {prop2:property} => 0xF6 @ value @ prop1 @ prop2
	
	call {address:property}=>0xF7 @ address
	ret => 0xF8
	
	load {prop:property_single}, {value:property} => 0xE0 @ prop @ value
	asset_load {asset:property}, {offset:property}, {refresh:bool}, {string:string} => 0xA0 @ asset @ offset @ refresh @ string 
	asset_load {asset:property}, {offset:property}, {prop:property_single} => 0xA1 @ asset @ offset @ prop 
	asset_load16 {asset:property}, {offset:property}, {prop:property_single} => 0xA2 @ asset @ offset @ prop 
	asset_load {asset:property}, {offset:property}, {refresh:bool}, {data:property} => 0xA3 @ asset @ offset @ refresh @ data 
	asset_load16 {asset:property}, {offset:property}, {refresh:bool}, {data:property} => 0xA4 @ asset @ offset @ refresh @ data 
	
	load {asset:asset}, {refresh:bool}, {string:string} => 0xA0 @ asset @ refresh @ string 
	load {prop:property_single}, {asset:asset} => 0xA1 @ asset @ prop 
	load16 {prop:property_single}, {asset:asset} => 0xA2 @ asset @ prop 
	load {asset:asset}, {refresh:bool}, {data:property} => 0xA3 @ asset @ refresh @ data 
	load16 {asset:asset}, {refresh:bool}, {data:property} => 0xA4 @ asset @ refresh @ data
	
	load {asset:asset}, {string:string} => 0xA0 @ asset @ 0`8 @ string 
	load {asset:asset}, {data:property} => 0xA3 @ asset @ 0`8 @ data 
	load16 {asset:asset}, {data:property} => 0xA4 @ asset @ 0`8 @ data
	
	loaditos {asset:asset}, {refresh:bool}, {number:property} => 0xA5 @ asset @ refresh @ number 
	loaditos {asset:asset}, {number:property} => 0xA5 @ asset @ 0`8 @ number 
	
	swp => 0xE1
	swp {prop1:property_single}, {prop2:property_single} => 0xE2 @ prop1 @ prop2
	
	sleep {delay:property} => 0xFF @ delay
	
	add {prop:property_single}, {value1:property}, {value2:property} => 0xD0 @ prop @ value1 @ value2
	sub {prop:property_single}, {value1:property}, {value2:property} => 0xD1 @ prop @ value1 @ value2
	mul {prop:property_single}, {value1:property}, {value2:property} => 0xD2 @ prop @ value1 @ value2
	div {prop:property_single}, {value1:property}, {value2:property} => 0xD3 @ prop @ value1 @ value2
	mod {prop:property_single}, {value1:property}, {value2:property} => 0xD4 @ prop @ value1 @ value2
	
	inc {prop:property_single}, {value:property} => 0xD5 @ prop @ value
	dec {prop:property_single}, {value:property} => 0xD6 @ prop @ value
	
	rnd {prop:property_single} => prop @ 0xC000
	rnd {prop:property_single}, {max:property}=> 0xC001 @ prop @ max
	rnd {prop:property_single}, {min:property}, {max:property} => 0xC002 @ prop @ min @ max
	
	append {asset:asset_nooffset}, {string:string} => 0xA6 @ asset @ 0x00 @ string
	
	push {prop:property} => 0xE3 @ prop
	pop {prop:property} => 0xE4 @ prop
	
	debug => 0x10
	hang => 0x11
	
	clear_obj => 0xC100
	draw_obj => 0xC101
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
	debug {v:property} => 0xC102 @ v
}