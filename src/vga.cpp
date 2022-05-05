
ALIGNED u8 Font_Copy[sizeof(FontBold8x8)];
u8 vga_mem[MALLOC_SIZE];
#include "hardware/structs/rosc.h"

class VGA
{
public:
    u8 *main_mem_ptr = 0;
    u8 *main_mem_end = 0;

    u8 *box_ptr = NULL;
    int box_len = 0;
    u16 Width;
    u16 Heigth;
    u16 cX = 0;
    u16 cY = 0;
    u8 bgcolor;
    sCanvas TmpCanvas;
    bool double_buffer = false;
    bool initialized = false;

    VGA()
    {
    }

    void Clear(u8 color)
    {
        memset(box_ptr, color, box_len);
        bgcolor = color;
    }

    void Clear()
    {
        memset(box_ptr, bgcolor, box_len);
    }

    void Init(u8 resolution, u8 tiles, u32 bs)
    {
        box_len = bs;

        box_ptr = main_mem_ptr;
        TmpCanvas.img = main_mem_ptr + bs;
        if (TmpCanvas.img > main_mem_end)
        {
            TmpCanvas.img = 0;
        }

        double_buffer = true;
        if (TmpCanvas.img == 0)
        {
            TmpCanvas.img = box_ptr;
            double_buffer = false;
        }
        Clear();

        Width = RES_LOOKUP_W[resolution];
        Heigth = RES_LOOKUP_H[resolution];
        cX = 0;
        cY = 0;

        Video(DEV_VGA, resolution, tiles, box_ptr);

        TmpCanvas.w = Canvas.w;
        TmpCanvas.h = Canvas.h;
        TmpCanvas.wb = Canvas.wb;
        TmpCanvas.format = Canvas.format;

        initialized = true;
    }

    void MergeCanvas()
    {
        if (!double_buffer)
        {
            return;
        }
        memcpy(Canvas.img, TmpCanvas.img, box_len);
    }

    void ClearDouble()
    {
        if (!double_buffer)
        {
            return;
        }
        memset(TmpCanvas.img, bgcolor, box_len);
    }

    u8 Fill(u8 mode, u32 offset, u32 length, u8 data)
    {
        u8 location = mode & 0x0F;
        if (location == 0)
        {
            if (offset + length > box_len)
            {
                if (GetBit(mode, 7))
                {
                    return Fill(mode, offset, box_len - offset, data);
                }
                return STATUS_WRONG_VALUE;
            }
            memset(box_ptr + offset, data, length);
        }
        else
        {
            return STATUS_NOT_FOUND;
        }
        return STATUS_OK;
    }

    void AutoInit(u8 res)
    {
        Init(res, 0, RES_LOOKUP[res]);
    }

    void DrawTextCursored(char *c, u8 fh, u8 sx, u8 sy, u8 color)
    {
        for (u16 i = 0; true; i++)
        {
            char cbuf[2];
            cbuf[0] = c[i];
            cbuf[1] = 0;

            if (cbuf[0] == 0)
                break;

            if (cbuf[0] == 10)
            {
                cY += 8;
                cX = 0;
                continue;
            }

            DrawText(&Canvas, (const char *)cbuf, cX, cY, color, Font_Copy, fh, sx, sy);
            cX += 8;

            if (cX + 8 > Width)
            {
                cX = 0;
                cY += 8;
            }
        }
    }

    void DrawTextCursored(char *c, u8 fh, u8 sx, u8 sy, u8 color, u8 bgcolor)
    {
        for (u16 i = 0; true; i++)
        {
            char cbuf[2];
            cbuf[0] = c[i];
            cbuf[1] = 0;

            if (cbuf[0] == 0)
                break;

            if (cbuf[0] == 10)
            {
                cY += 8;
                cX = 0;
                continue;
            }

            DrawTextBg(&Canvas, (const char *)cbuf, cX, cY, color, bgcolor, Font_Copy, fh, sx, sy);
            cX += 8;

            if (cX + 8 > Width)
            {
                cX = 0;
                cY += 8;
            }
        }
    }

    void SetCursor(u16 x, u16 y)
    {
        cX = x;
        cY = y;
    }

    void Allocate()
    {
        main_mem_ptr = vga_mem;
        main_mem_end = main_mem_ptr + MALLOC_SIZE - 1;
        if (main_mem_ptr == 0)
        {
            while (1)
            {
                gpio_put(LED_PIN, 1);
                sleep_ms(500);
                gpio_put(LED_PIN, 0);
                sleep_ms(500);
            };
        }
    }
};