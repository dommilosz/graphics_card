
// ****************************************************************************
//
//                           VGA colors and palettes
//
// ****************************************************************************

#ifndef _VGA_PAL_H
#define _VGA_PAL_H

#define MULTICOL(a,b,c,d) ((a)|((b)<<8)|((c)<<16)|((d)<<24)) // multiply color pattern (used in mode GF_COLOR)

// CGA colors
#define CGACOL_0	0	// 0x000000 black
#define CGACOL_1	2	// 0x0000C3 dark blue
#define CGACOL_2	20	// 0x00C300 dark green
#define CGACOL_3	22	// 0x00C3C3 dark cyan
#define CGACOL_4	160	// 0xC30000 dark red
#define CGACOL_5	162	// 0xC300C3 dark magenta
#define CGACOL_6	168	// 0xC35400 brown
#define CGACOL_7	182	// 0xC3C3C3 light gray
#define CGACOL_8	73	// 0x545454 dark gray
#define CGACOL_9	75	// 0x5454FF light blue
#define CGACOL_10	93	// 0x54FF54 light green
#define CGACOL_11	95	// 0x54FFFF light cyan
#define CGACOL_12	233	// 0xFF5454 light red
#define CGACOL_13	235	// 0xFF54FF light magenta
#define CGACOL_14	253	// 0xFFFF54 yellow
#define CGACOL_15	255	// 0xFFFFFF white

// ZX Spectrum color
#define ZXCOL_0		0	// 0x000000 black
#define ZXCOL_1		2	// 0x0000C3 dark blue
#define ZXCOL_2		160	// 0xC30000 dark red
#define ZXCOL_3		162	// 0xC300C3 dark magenta
#define ZXCOL_4		20	// 0x00C300 dark green
#define ZXCOL_5		22	// 0x00C3C3 dark cyan
#define ZXCOL_6		180	// 0xC3C300 dark yellow
#define ZXCOL_7		182	// 0xC3C3C3 light gray
#define ZXCOL_8		73	// 0x545454 dark gray
#define ZXCOL_9		3	// 0x0000FF light blue
#define ZXCOL_10	224	// 0xFF0000 light red
#define ZXCOL_11	227	// 0xFF00FF light magenta
#define ZXCOL_12	28	// 0x00FF00 light green
#define ZXCOL_13	31	// 0x00FFFF light cyan
#define ZXCOL_14	252	// 0xFFFF00 yellow
#define ZXCOL_15	255	// 0xFFFFFF white

// Colors
//	GP0 ... _1LS0 ... VGA _1LS0 blue
//	GP1 ... _1LS1 ... VGA _1LS1
//	GP2 ... _1LS2 ... VGA G0 green
//	GP3 ... _1LS3 ... VGA G1
//	GP4 ... _1LS4 ... VGA G2
//	GP5 ... _1LS5 ... VGA R0 red
//	GP6 ... _1LS6 ... VGA R1
//	GP7 ... _1LS7 ... VGA R2

#define COL_BLACK	0

#define COL_DARKBLUE	_1LS0
#define COL_SEMIBLUE	_1LS1
#define COL_BLUE	(_1LS0+_1LS1)
#define COL_MOREBLUE	(COL_BLUE+_1LS3+_1LS6)
#define COL_LIGHTBLUE	(COL_BLUE+_1LS4+_1LS7)

#define COL_DARKGREEN	_1LS3
#define COL_SEMIGREEN	_1LS4
#define COL_GREEN	(_1LS2+_1LS3+_1LS4)
#define COL_MOREGREEN	(COL_GREEN+_1LS0+_1LS6)
#define COL_LIGHTGREEN	(COL_GREEN+_1LS1+_1LS7)

#define COL_DARKRED	_1LS6
#define COL_SEMIRED	_1LS7
#define COL_RED		(_1LS5+_1LS6+_1LS7)
#define COL_MORERED	(COL_RED+_1LS0+_1LS3)
#define COL_LIGHTRED	(COL_RED+_1LS1+_1LS4)

#define COL_DARKCYAN	(_1LS0+_1LS3)
#define COL_SEMICYAN	(_1LS1+_1LS4)
#define COL_CYAN	(_1LS0+_1LS1+_1LS2+_1LS3+_1LS4)

#define COL_DARKMAGENTA	(_1LS0+_1LS6)
#define COL_SEMIMAGENTA	(_1LS1+_1LS7)
#define COL_MAGENTA	(_1LS0+_1LS1+_1LS5+_1LS6+_1LS7)

#define COL_DARKYELLOW	(_1LS3+_1LS6)
#define COL_SEMIYELLOW	(_1LS4+_1LS7)
#define COL_YELLOW	(_1LS2+_1LS3+_1LS4+_1LS5+_1LS6+_1LS7)

#define COL_GRAY0	0
#define COL_GRAY1	(_1LS2+_1LS5)
#define COL_GRAY2	(_1LS0+_1LS3+_1LS6)
#define COL_GRAY3	(_1LS0+_1LS2+_1LS3+_1LS5+_1LS6)
#define COL_GRAY4	(_1LS1+_1LS4+_1LS7)
#define COL_GRAY5	(_1LS1+_1LS2+_1LS4+_1LS5+_1LS7)
#define COL_GRAY6	(_1LS0+_1LS1+_1LS3+_1LS4+_1LS6+_1LS7)
#define COL_GRAY7	(_1LS0+_1LS1+_1LS2+_1LS3+_1LS4+_1LS5+_1LS6+_1LS7)

#define COL_WHITE	COL_GRAY7

// compose color from RGB
#define COLRGB(r,g,b) ((u8)(((r)&0xe0)|(((g)&0xe0)>>3)|((b)>>6)))

// default 16-color palettes (CGA colors)
// - do not set "const", to stay in faster RAM
extern u8 DefPal16[16];

// ZX Spectrum color palettes
// - do not set "const", to stay in faster RAM
extern u8 ZXPal16[16];

// 4-color palettes (CGA colors) 
// - do not set "const", to stay in faster RAM
extern u8 PalCGA1[4]; // palette 0, low intensity (black, dark green, brown)
extern u8 PalCGA2[4]; // palette 0, high intensity (black, light green, light red, yellow)
extern u8 PalCGA3[4]; // palette 1, low intensity (black, dark cyan, dark magenta, light gray)
extern u8 PalCGA4[4]; // palette 1, high intensity (black, light cyan, light magenta, white)
extern u8 PalCGA5[4]; // palette 1, low intensity (black, dark cyan, dark red, light gray)
extern u8 PalCGA6[4]; // palette 1, high intensity (black, light cyan, light red, white)

// 4-color palette translation table
//extern u32 Pal4Trans[256];
//extern u32 Pal4Plane[256];

// 16-color palette translation table
//extern u16 Pal16Trans[256];
//extern u16 ZX16Trans[256];

// values of color components
extern u8 RGVal[8]; // values of Red and Green components
extern u8 BVal[4]; // values of Blue components

// distance of 2 colors in 332 format (R3G3_1LS2)
// - returns value 0..195075
int ColDist(u8 col1, u8 col2);

// generate gradient
void GenGrad(u8* dst, int w);

#endif // _VGA_PAL_H
