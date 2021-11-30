#include "include.h"

// format: 1-bit pixel graphics
// image width: 2048 pixels
// image height: 8 lines
// image pitch: 256 bytes
const u8 FontGame8x8[2048] __attribute__ ((aligned(4))) = {
	0x00, 0xAA, 0x00, 0xFF, 0x6C, 0x10, 0x10, 0x10, 0x3C, 0x3C, 0x30, 0x38, 0x10, 0x38, 0x44, 0x44, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x10, 0x36, 0x24, 0x10, 0x60, 0x00, 0x18, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0x7C, 0x18, 0x7C, 0x7E, 0x60, 0xFC, 0x3C, 0x7E, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 
	0x3C, 0x3C, 0xD8, 0x38, 0xDC, 0xDC, 0xEE, 0x3C, 0xCC, 0x7E, 0x3E, 0xC0, 0xC0, 0x66, 0xCC, 0x38, 
	0xDC, 0x38, 0xDC, 0x3C, 0xFE, 0xE6, 0xCE, 0xCC, 0xEE, 0xCC, 0x76, 0x1C, 0xC0, 0x38, 0x38, 0x00, 
	0x00, 0x00, 0x60, 0x00, 0x06, 0x00, 0x18, 0x00, 0x70, 0x18, 0x0C, 0x60, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x08, 0x78, 0x36, 0x3C, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xEF, 0xC9, 0xDB, 0xEF, 0x9F, 0xFF, 0xE7, 0xF1, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 
	0x83, 0xE7, 0x83, 0x81, 0x9F, 0x03, 0xC3, 0x81, 0x83, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x83, 
	0xC3, 0xC3, 0x27, 0xC7, 0x23, 0x23, 0x11, 0xC3, 0x33, 0x81, 0xC1, 0x3F, 0x3F, 0x99, 0x33, 0xC7, 
	0x23, 0xC7, 0x23, 0xC3, 0x01, 0x19, 0x31, 0x33, 0x11, 0x33, 0x89, 0xE3, 0x3F, 0xC7, 0xC7, 0xFF, 
	0xFF, 0xFF, 0x9F, 0xFF, 0xF9, 0xFF, 0xE7, 0xFF, 0x8F, 0xE7, 0xF3, 0x9F, 0xF7, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xF7, 0x87, 0xC9, 0xC3, 
	0x00, 0x55, 0x18, 0xE7, 0xFE, 0x38, 0x38, 0x38, 0x42, 0x7E, 0x28, 0x38, 0x54, 0x7C, 0x28, 0x38, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x38, 0x36, 0x6C, 0x7E, 0x66, 0x30, 0x18, 0x18, 0x18, 0x6C, 0x08, 0x00, 0x00, 0x00, 0x06, 
	0xEE, 0x38, 0xCE, 0xCC, 0xC8, 0x66, 0x60, 0xE6, 0xEE, 0xCE, 0x00, 0x00, 0x18, 0x00, 0x30, 0xC6, 
	0x6E, 0x66, 0xEC, 0x6C, 0xE6, 0xE6, 0x72, 0x66, 0xC6, 0x98, 0x0C, 0x66, 0xE0, 0xFE, 0xE6, 0x6C, 
	0xE6, 0x6C, 0xE6, 0x66, 0xBA, 0x66, 0xCC, 0xCE, 0xC6, 0xC6, 0xEC, 0x30, 0x60, 0x0C, 0x7C, 0x00, 
	0x1C, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x34, 0x00, 0x70, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x08, 0x0C, 0x36, 0x42, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xC7, 0xC9, 0x93, 0x81, 0x99, 0xCF, 0xE7, 0xE7, 0xE7, 0x93, 0xF7, 0xFF, 0xFF, 0xFF, 0xF9, 
	0x11, 0xC7, 0x31, 0x33, 0x37, 0x99, 0x9F, 0x19, 0x11, 0x31, 0xFF, 0xFF, 0xE7, 0xFF, 0xCF, 0x39, 
	0x91, 0x99, 0x13, 0x93, 0x19, 0x19, 0x8D, 0x99, 0x39, 0x67, 0xF3, 0x99, 0x1F, 0x01, 0x19, 0x93, 
	0x19, 0x93, 0x19, 0x99, 0x45, 0x99, 0x33, 0x31, 0x39, 0x39, 0x13, 0xCF, 0x9F, 0xF3, 0x83, 0xFF, 
	0xE3, 0xFF, 0xCF, 0xFF, 0xF3, 0xFF, 0xCB, 0xFF, 0x8F, 0xFF, 0xFF, 0x9F, 0xE7, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xF7, 0xF3, 0xC9, 0xBD, 
	0x00, 0xAA, 0x24, 0xDB, 0xFE, 0x7C, 0x54, 0x7C, 0xA5, 0xDB, 0x24, 0x10, 0x38, 0xD6, 0x7C, 0xFE, 
	0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 
	0x00, 0x38, 0x12, 0xFE, 0xD8, 0x0C, 0x38, 0x30, 0x30, 0x0C, 0x38, 0x18, 0x00, 0x3C, 0x00, 0x0C, 
	0xC6, 0x38, 0x9C, 0x98, 0xD8, 0x60, 0xDC, 0x0E, 0xC6, 0x86, 0x18, 0x18, 0x30, 0x3E, 0x18, 0x46, 
	0x6E, 0xC6, 0xCC, 0xC6, 0xC6, 0x60, 0x60, 0xC0, 0xC6, 0x30, 0x0C, 0x6C, 0x60, 0xD6, 0xE6, 0xC6, 
	0xC6, 0xC6, 0xC6, 0x62, 0x18, 0xC6, 0xC6, 0xC6, 0x6C, 0x66, 0x98, 0x30, 0x30, 0x0C, 0x54, 0x00, 
	0x36, 0x38, 0x3C, 0x38, 0x3C, 0x3C, 0x70, 0x3E, 0x3C, 0x08, 0x04, 0x6C, 0x18, 0x6C, 0x3C, 0x1C, 
	0x5C, 0x3A, 0x2C, 0x3C, 0x78, 0x36, 0xC6, 0xC6, 0x46, 0x26, 0x7E, 0x30, 0x18, 0x0C, 0x24, 0x99, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 0xFF, 0xFF, 0xEF, 0xEF, 
	0xFF, 0xC7, 0xED, 0x01, 0x27, 0xF3, 0xC7, 0xCF, 0xCF, 0xF3, 0xC7, 0xE7, 0xFF, 0xC3, 0xFF, 0xF3, 
	0x39, 0xC7, 0x63, 0x67, 0x27, 0x9F, 0x23, 0xF1, 0x39, 0x79, 0xE7, 0xE7, 0xCF, 0xC1, 0xE7, 0xB9, 
	0x91, 0x39, 0x33, 0x39, 0x39, 0x9F, 0x9F, 0x3F, 0x39, 0xCF, 0xF3, 0x93, 0x9F, 0x29, 0x19, 0x39, 
	0x39, 0x39, 0x39, 0x9D, 0xE7, 0x39, 0x39, 0x39, 0x93, 0x99, 0x67, 0xCF, 0xCF, 0xF3, 0xAB, 0xFF, 
	0xC9, 0xC7, 0xC3, 0xC7, 0xC3, 0xC3, 0x8F, 0xC1, 0xC3, 0xF7, 0xFB, 0x93, 0xE7, 0x93, 0xC3, 0xE3, 
	0xA3, 0xC5, 0xD3, 0xC3, 0x87, 0xC9, 0x39, 0x39, 0xB9, 0xD9, 0x81, 0xCF, 0xE7, 0xF3, 0xDB, 0x66, 
	0x00, 0x55, 0x42, 0xBD, 0xFE, 0xFE, 0xFE, 0xFE, 0x81, 0xFF, 0x24, 0x7C, 0xEE, 0xD6, 0xD6, 0x54, 
	0x18, 0xF0, 0x10, 0xF0, 0x1F, 0xFF, 0x1F, 0xFF, 0x10, 0xF0, 0x10, 0xF0, 0x1F, 0xFF, 0x1F, 0xFF, 
	0x00, 0x10, 0x24, 0x6C, 0x7C, 0x18, 0x76, 0x00, 0x30, 0x0C, 0xFE, 0x7E, 0x00, 0x78, 0x00, 0x18, 
	0xC6, 0x18, 0x38, 0x3C, 0x7E, 0x7C, 0xF6, 0x1C, 0x7C, 0xDE, 0x18, 0x18, 0x60, 0x00, 0x0C, 0x1C, 
	0x6C, 0xFE, 0xFC, 0xC0, 0xC6, 0x7C, 0x7C, 0xCE, 0xFE, 0x30, 0xE6, 0x78, 0x60, 0xD6, 0xD6, 0xC6, 
	0xCE, 0xD6, 0xEC, 0x3C, 0x18, 0xC6, 0xC6, 0xD6, 0x38, 0x3C, 0x30, 0x30, 0x18, 0x0C, 0xD6, 0x00, 
	0x70, 0x6E, 0x32, 0x74, 0x4C, 0x66, 0x38, 0x76, 0x36, 0x18, 0x0C, 0x38, 0x18, 0x7E, 0x2E, 0x3E, 
	0x26, 0x6E, 0x3E, 0x70, 0x30, 0x66, 0x46, 0xC6, 0x6C, 0x66, 0x5C, 0x60, 0x18, 0x06, 0x12, 0xA1, 
	0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 0x00, 0x0F, 0xF0, 0xFF, 
	0xE7, 0x0F, 0xEF, 0x0F, 0xE0, 0x00, 0xE0, 0x00, 0xEF, 0x0F, 0xEF, 0x0F, 0xE0, 0x00, 0xE0, 0x00, 
	0xFF, 0xEF, 0xDB, 0x93, 0x83, 0xE7, 0x89, 0xFF, 0xCF, 0xF3, 0x01, 0x81, 0xFF, 0x87, 0xFF, 0xE7, 
	0x39, 0xE7, 0xC7, 0xC3, 0x81, 0x83, 0x09, 0xE3, 0x83, 0x21, 0xE7, 0xE7, 0x9F, 0xFF, 0xF3, 0xE3, 
	0x93, 0x01, 0x03, 0x3F, 0x39, 0x83, 0x83, 0x31, 0x01, 0xCF, 0x19, 0x87, 0x9F, 0x29, 0x29, 0x39, 
	0x31, 0x29, 0x13, 0xC3, 0xE7, 0x39, 0x39, 0x29, 0xC7, 0xC3, 0xCF, 0xCF, 0xE7, 0xF3, 0x29, 0xFF, 
	0x8F, 0x91, 0xCD, 0x8B, 0xB3, 0x99, 0xC7, 0x89, 0xC9, 0xE7, 0xF3, 0xC7, 0xE7, 0x81, 0xD1, 0xC1, 
	0xD9, 0x91, 0xC1, 0x8F, 0xCF, 0x99, 0xB9, 0x39, 0x93, 0x99, 0xA3, 0x9F, 0xE7, 0xF9, 0xED, 0x5E, 
	0x00, 0xAA, 0x42, 0xBD, 0x7C, 0x7C, 0x54, 0xFE, 0xA5, 0xFF, 0x28, 0x10, 0x38, 0xFE, 0xFE, 0x7C, 
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x10, 0x00, 0xFE, 0x36, 0x30, 0xFC, 0x00, 0x30, 0x0C, 0x38, 0x18, 0x18, 0x00, 0x00, 0x30, 
	0xC6, 0x18, 0x70, 0x0E, 0x18, 0x0E, 0xC2, 0x38, 0xC6, 0x76, 0x00, 0x00, 0x30, 0x7C, 0x18, 0x30, 
	0x60, 0xC6, 0xC6, 0xC2, 0xC6, 0x60, 0x60, 0xC4, 0xC6, 0x30, 0x66, 0x78, 0x60, 0xC6, 0xCE, 0xC6, 
	0xF8, 0xCC, 0xD8, 0x46, 0x18, 0xC6, 0xC6, 0xD6, 0x6C, 0x18, 0x62, 0x30, 0x0C, 0x0C, 0x10, 0x00, 
	0x30, 0x66, 0x36, 0x70, 0x6C, 0x6C, 0x30, 0x64, 0x32, 0x18, 0x0C, 0x30, 0x18, 0x56, 0x26, 0x66, 
	0x2E, 0x64, 0x30, 0x3C, 0x30, 0x66, 0x7E, 0x54, 0x38, 0x66, 0x18, 0x30, 0x18, 0x0C, 0x00, 0xA1, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xEF, 0xFF, 0x01, 0xC9, 0xCF, 0x03, 0xFF, 0xCF, 0xF3, 0xC7, 0xE7, 0xE7, 0xFF, 0xFF, 0xCF, 
	0x39, 0xE7, 0x8F, 0xF1, 0xE7, 0xF1, 0x3D, 0xC7, 0x39, 0x89, 0xFF, 0xFF, 0xCF, 0x83, 0xE7, 0xCF, 
	0x9F, 0x39, 0x39, 0x3D, 0x39, 0x9F, 0x9F, 0x3B, 0x39, 0xCF, 0x99, 0x87, 0x9F, 0x39, 0x31, 0x39, 
	0x07, 0x33, 0x27, 0xB9, 0xE7, 0x39, 0x39, 0x29, 0x93, 0xE7, 0x9D, 0xCF, 0xF3, 0xF3, 0xEF, 0xFF, 
	0xCF, 0x99, 0xC9, 0x8F, 0x93, 0x93, 0xCF, 0x9B, 0xCD, 0xE7, 0xF3, 0xCF, 0xE7, 0xA9, 0xD9, 0x99, 
	0xD1, 0x9B, 0xCF, 0xC3, 0xCF, 0x99, 0x81, 0xAB, 0xC7, 0x99, 0xE7, 0xCF, 0xE7, 0xF3, 0xFF, 0x5E, 
	0x00, 0x55, 0x24, 0xDB, 0x38, 0x38, 0x10, 0x10, 0x99, 0xDB, 0xE0, 0x28, 0x54, 0xFE, 0xC6, 0x7C, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0x6C, 0xFC, 0x66, 0xCC, 0x00, 0x18, 0x18, 0x6C, 0x18, 0x18, 0x00, 0x18, 0x60, 
	0xEE, 0x3E, 0xE6, 0xCE, 0x18, 0xCE, 0xE6, 0x30, 0xEE, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x30, 0x00, 
	0x66, 0xE6, 0xE6, 0xEE, 0xCC, 0xE6, 0xE0, 0xEC, 0xC6, 0x1A, 0xC6, 0xEC, 0xFC, 0xE6, 0xCE, 0xEE, 
	0xC0, 0xEC, 0xCC, 0xC6, 0x38, 0xC6, 0x6C, 0xFE, 0xC6, 0x30, 0xDE, 0x30, 0x06, 0x0C, 0x10, 0x00, 
	0x60, 0x66, 0x66, 0x60, 0x66, 0x70, 0x60, 0x3C, 0x36, 0x18, 0x0C, 0x7C, 0x18, 0xC6, 0x6C, 0x6E, 
	0x7C, 0x3E, 0x30, 0x1E, 0x3C, 0x6C, 0x3C, 0x7C, 0x6C, 0x3C, 0x32, 0x30, 0x18, 0x1C, 0x00, 0x99, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xFF, 0xFF, 0x93, 0x03, 0x99, 0x33, 0xFF, 0xE7, 0xE7, 0x93, 0xE7, 0xE7, 0xFF, 0xE7, 0x9F, 
	0x11, 0xC1, 0x19, 0x31, 0xE7, 0x31, 0x19, 0xCF, 0x11, 0xF3, 0xE7, 0xE7, 0xE7, 0xFF, 0xCF, 0xFF, 
	0x99, 0x19, 0x19, 0x11, 0x33, 0x19, 0x1F, 0x13, 0x39, 0xE5, 0x39, 0x13, 0x03, 0x19, 0x31, 0x11, 
	0x3F, 0x13, 0x33, 0x39, 0xC7, 0x39, 0x93, 0x01, 0x39, 0xCF, 0x21, 0xCF, 0xF9, 0xF3, 0xEF, 0xFF, 
	0x9F, 0x99, 0x99, 0x9F, 0x99, 0x8F, 0x9F, 0xC3, 0xC9, 0xE7, 0xF3, 0x83, 0xE7, 0x39, 0x93, 0x91, 
	0x83, 0xC1, 0xCF, 0xE1, 0xC3, 0x93, 0xC3, 0x83, 0x93, 0xC3, 0xCD, 0xCF, 0xE7, 0xE3, 0xFF, 0x66, 
	0x00, 0xAA, 0x18, 0xE7, 0x10, 0x10, 0x38, 0x38, 0x42, 0x66, 0xC0, 0x44, 0x10, 0xAA, 0x6C, 0xAA, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x10, 0x00, 0x48, 0x10, 0x06, 0x76, 0x00, 0x0E, 0x70, 0x00, 0x10, 0x30, 0x00, 0x18, 0xC0, 
	0x7C, 0x7C, 0xFE, 0x7C, 0x10, 0x7C, 0x7C, 0x30, 0x7C, 0x78, 0x18, 0x18, 0x00, 0x00, 0x00, 0x30, 
	0x3C, 0x66, 0xDC, 0x7C, 0xF8, 0xDC, 0xC0, 0x78, 0x66, 0xFC, 0x7C, 0xC6, 0xC2, 0x66, 0x66, 0x7C, 
	0xC0, 0x76, 0xC6, 0x7C, 0x30, 0x7C, 0x38, 0x6C, 0xEE, 0x60, 0xBC, 0x1C, 0x03, 0x38, 0x10, 0x00, 
	0x7C, 0x3A, 0x7C, 0x3C, 0x3E, 0x3C, 0x60, 0x0C, 0x66, 0x0C, 0x1C, 0x66, 0x0C, 0xCC, 0x6C, 0x38, 
	0x60, 0x06, 0x60, 0x38, 0x18, 0x3C, 0x18, 0x6C, 0xC4, 0x0C, 0x7E, 0x1E, 0x10, 0x78, 0x00, 0x42, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xEF, 0xFF, 0xB7, 0xEF, 0xF9, 0x89, 0xFF, 0xF1, 0x8F, 0xFF, 0xEF, 0xCF, 0xFF, 0xE7, 0x3F, 
	0x83, 0x83, 0x01, 0x83, 0xEF, 0x83, 0x83, 0xCF, 0x83, 0x87, 0xE7, 0xE7, 0xFF, 0xFF, 0xFF, 0xCF, 
	0xC3, 0x99, 0x23, 0x83, 0x07, 0x23, 0x3F, 0x87, 0x99, 0x03, 0x83, 0x39, 0x3D, 0x99, 0x99, 0x83, 
	0x3F, 0x89, 0x39, 0x83, 0xCF, 0x83, 0xC7, 0x93, 0x11, 0x9F, 0x43, 0xE3, 0xFC, 0xC7, 0xEF, 0xFF, 
	0x83, 0xC5, 0x83, 0xC3, 0xC1, 0xC3, 0x9F, 0xF3, 0x99, 0xF3, 0xE3, 0x99, 0xF3, 0x33, 0x93, 0xC7, 
	0x9F, 0xF9, 0x9F, 0xC7, 0xE7, 0xC3, 0xE7, 0x93, 0x3B, 0xF3, 0x81, 0xE1, 0xEF, 0x87, 0xFF, 0xBD, 
	0x00, 0x55, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 
	0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 0xEF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0x9F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 
};
