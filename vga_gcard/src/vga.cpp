
ALIGNED u8 Font_Copy[sizeof(FontBold8x8)];
class VGA
{
public:
    u8 *box_ptr = NULL;
    int box_len = 0;
    u8 *additional_ptr = NULL;
    int additional_len = 0;
    u16 Width;
    u16 Heigth;
    u16 cX = 0;
    u16 cY = 0;
    u8 bgcolor;

    VGA(){

    }

    void Clear(u8 color)
    {
        memset(box_ptr, color, box_len);
        memset(additional_ptr, 0, additional_len);
        bgcolor = color;
    }

    void Clear()
    {
        memset(box_ptr, bgcolor, box_len);
        memset(additional_ptr, 0, additional_len);
    }

    void Init(u8 resolution, u8 tiles, u32 bs, u32 ams)
    {
        box_len = bs;
        additional_len = ams;

        free(box_ptr);
        free(additional_ptr);

        box_ptr = (u8 *)malloc(bs);
        additional_ptr = (u8 *)malloc(ams);

        Width = RES_LOOKUP_W[resolution];
        Heigth = RES_LOOKUP_H[resolution];
        cX = 0;
        cY = 0;

        if (additional_len < 1)
        {
            Video(DEV_VGA, resolution, tiles, box_ptr, Font_Copy);
        }
        else
        {
            Video(DEV_VGA, resolution, tiles, box_ptr, additional_ptr);
        }
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
        else if (location == 1)
        {
            if (offset + length > additional_len)
            {
                if (GetBit(mode, 7))
                {
                    return Fill(mode, offset, additional_len - offset, data);
                }
                return STATUS_WRONG_VALUE;
            }
            memset(additional_ptr + offset, data, length);
        }
        else
        {
            return STATUS_NOT_FOUND;
        }
        return STATUS_OK;
    }

    void AutoInit(u8 res)
    {
        Init(res, 0, RES_LOOKUP[res], 0);
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

            DrawTextBg(&Canvas, (const char *)cbuf, cX, cY, color, bgcolor, Font_Copy, fh, sx, sy);
            cX += 8;

            if (cX + 8 > Width)
            {
                cX = 0;
                cY += 8;
            }
        }
    }

    void SetCursor(u16 x, u16 y){
        cX = x;
        cY = y;
    }
};