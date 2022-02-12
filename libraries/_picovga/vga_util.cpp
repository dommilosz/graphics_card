
// ****************************************************************************
//
//                                 VGA utilities
//
// ****************************************************************************

#include "include.h"

// convert image from 16-color to 8x8 attribute format
void Attr8Conv(u8* dst, u8* attr, const u8* src, int w, int h, const u8* pal)
{
	int x, y, i, j, bestnum, best2num;
	int hist[16];
	u8 b, b2, b3, best, best2, bestcol, best2col;
	const u8 *s;
	u8 *d;
	int ws = w/2;
	int wd = w/8;

	for (y = 0; y < h; y += 8)
	{
		for (x = 0; x < w; x += 8)
		{
			// clear histogram
			memset(hist, 0, 16*sizeof(int));

			// get histogram of this segment
			s = src;
			for (i = 0; i < 8; i++)
			{
				b = s[0]; hist[b >> 4]++; hist[b & 0x0f]++;
				b = s[1]; hist[b >> 4]++; hist[b & 0x0f]++;
				b = s[2]; hist[b >> 4]++; hist[b & 0x0f]++;
				b = s[3]; hist[b >> 4]++; hist[b & 0x0f]++;
				s += ws;
			}

			// find 1st best pixel
			best2 = 0;
			best2num = 0;
			for (i = 0; i < 16; i++)
			{
				if (hist[i] > best2num)
				{
					best2 = (u8)i;
					best2num = hist[i];
				}
			}

			// find 2nd best pixel
			best = best2;
			bestnum = 0;
			for (i = 0; i < 16; i++)
			{
				if ((hist[i] > bestnum) && (i != best2))
				{
					best = (u8)i;
					bestnum = hist[i];
				}
			}

			// sort pixels, 'best' will be brighter (foreground) than 'best2' (background)
			if (ColDist(best, 0) < ColDist(best2, 0))
			{
				i = best;
				best = best2;
				best2 = i;
			}

			// write attributes (best = foreground, best2 = background)
			*attr++ = (best2 << 4) | best;
			bestcol = pal[best];
			best2col = pal[best2];

			// write pixels
			s = src;
			d = dst;
			for (i = 0; i < 8; i++)
			{
				b3 = 0;

				b = s[0];
				b2 = pal[b >> 4];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ?_1LS7 : 0;
				b2 = pal[b & 0x0f];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ? _1LS6 : 0;

				b = s[1];
				b2 = pal[b >> 4];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ?_1LS5 : 0;
				b2 = pal[b & 0x0f];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ?_1LS4 : 0;

				b = s[2];
				b2 = pal[b >> 4];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ? _1LS3 : 0;
				b2 = pal[b & 0x0f];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ? _1LS2 : 0;

				b = s[3];
				b2 = pal[b >> 4];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ? _1LS1 : 0;
				b2 = pal[b & 0x0f];
				b3 |= (ColDist(b2, bestcol) < ColDist(b2, best2col)) ? _1LS0 : 0;

				*d = b3;

				s += ws;
				d += wd;
			}

			// next segment on the row
			src += 4;
			dst++;
		}

		// next row
		src += ws*7;
		dst += wd*7;
	}
}

// convert image from 4-color to 2-plane format (width must be multiply of 8)
void Plane2Conv(u8* plane0, u8* plane1, const u8* src, int w, int h)
{
	int i;
	u8 b1, b2;
	for (i = w/8*h; i > 0; i--)
	{
		b2 = *src++;
		b1 = *src++;
		*plane0++ = ((b2 & _1LS6)<<1) | ((b2 &_1LS4)<<2) | ((b2 & _1LS2)<<3) | ((b2 & _1LS0)<<4) |
				((b1 & _1LS6)>>3) | ((b1 &_1LS4)>>2) | ((b1 & _1LS2)>>1) | (b1 & _1LS0);
		*plane1++ = (b2 &_1LS7) | ((b2 &_1LS5)<<1) | ((b2 & _1LS3)<<2) | ((b2 & _1LS1)<<3) |
				((b1 &_1LS7)>>4) | ((b1 &_1LS5)>>3) | ((b1 & _1LS3)>>2) | ((b1 & _1LS1)>>1);
	}
}

// invert image
void ImgInvert(u8* dst, int num)
{
	for (; num > 0; num--)
	{
		*dst = ~*dst;
		dst++;
	}
}

// decode unsigned number into ASCIIZ text buffer (returns number of digits)
int DecUNum(char* buf, u32 num)
{
	// temporary buffer (max. 10 digits + 1 NUL)
	char b[11];

	// decode number
	char* d = &b[10];
	*d = 0;
	int n = 0;
	u32 k;
	do {
		k = num / 10;
		num -= k*10;
		d--;
		*d = (char)(num + '0');
		num = k;
		n++;
	} while (num != 0);

	// copy number to destination buffer
	memcpy(buf, d, n+1);
	return n;
}

// decode signed number into ASCIIZ text buffer (returns number of digits)
int DecNum(char* buf, s32 num)
{
	if (num >= 0) return DecUNum(buf, num);

	num = -num;
	*buf++ = '-';
	return DecUNum(buf, num)+1;
}

// prepare image with white key transparency (copy and increment pixels)
void CopyWhiteImg(u8* dst, const u8* src, int num)
{
	for (; num > 0; num--) *dst++ = *src++ + 1;
}
