#ifndef __CONSTS_H_VGA
#define __CONSTS_H_VGA

#include "include.h"
#include "config.h"

#define RES_ZX_S 256 * 192
#define RES_CGA_S 320 * 200
#define RES_QVGA_S 320 * 240
#define RES_EGA_S 512 * 400
#define RES_VGA_S 640 * 480
#define RES_SVGA_S 800 * 600
#define RES_XGA_S 1024 * 768
#define RES_HD_S 1280 * 960

#define RES_ZX_S_W 256
#define RES_CGA_S_W 320
#define RES_QVGA_S_W 320
#define RES_EGA_S_W 512
#define RES_VGA_S_W 640
#define RES_SVGA_S_W 800
#define RES_XGA_S_W 1024
#define RES_HD_S_W 1280

#define RES_ZX_S_H 192
#define RES_CGA_S_H 200
#define RES_QVGA_S_H 240
#define RES_EGA_S_H 400
#define RES_VGA_S_H 480
#define RES_SVGA_S_H 600
#define RES_XGA_S_H 768
#define RES_HD_S_H 960

const u32 RES_LOOKUP[] = {RES_ZX_S, RES_CGA_S, RES_QVGA_S, RES_EGA_S, RES_VGA_S, RES_SVGA_S, RES_XGA_S, RES_HD_S};
const u32 RES_LOOKUP_W[] = {RES_ZX_S_W, RES_CGA_S_W, RES_QVGA_S_W, RES_EGA_S_W, RES_VGA_S_W, RES_SVGA_S_W, RES_XGA_S_W, RES_HD_S_W};
const u32 RES_LOOKUP_H[] = {RES_ZX_S_H, RES_CGA_S_H, RES_QVGA_S_H, RES_EGA_S_H, RES_VGA_S_H, RES_SVGA_S_H, RES_XGA_S_H, RES_HD_S_H};

#define ObjType_RECTANGLE 1
#define ObjType_TEXT 2
#define ObjType_CIRCLE 3 
#define ObjType_LINE 4
#define ObjType_BLOB 5

#define STATUS_OK 0
#define STATUS_NOT_FOUND 1
#define STATUS_UNKNOWN_ERROR 2
#define STATUS_UNTERMINATED 3
#define STATUS_WRONG_TYPE 4
#define STATUS_WRONG_VALUE 5
#define STATUS_ILLEGAL_OBJECT 6
#define STATUS_ILLEGAL_VALUE 7
#define STATUS_UNDEFINED 255

#define OBJECT_ALLOC (OBJECT_SIZE)

u8 tmp_buff[TMP_BUFFER_SIZE];

#if CODE_ADDRESS_SIZE == 8
typedef u8 CODE_ADDRESS_TYPE;
#elif CODE_ADDRESS_SIZE == 16
typedef u16 CODE_ADDRESS_TYPE;
#endif
#endif