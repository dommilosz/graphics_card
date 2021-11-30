#include "include.h"
#include "consts.h"
#include "util.h"
#include "vga.cpp"

VGA vga;

#include "objects.cpp"

void CommandRoutine()
{
	char cmd = GetChar();
	if (cmd <= 0)
		return;

	//INIT - resolution, tiles, board_size,
	if (cmd == 1)
	{
		u8 resolution = GetU8();
		u8 tiles = GetU8();
		u32 board_size = GetU32();
		u32 additional_map_size = GetU32();

		vga.Init(resolution, tiles, board_size, additional_map_size);

		Status(STATUS_OK);
	}
	//WRITE - (0-box, 1-add), offset, length, data......
	else if (cmd == 2)
	{
		u8 location = GetU8();
		u32 offset = GetU32();
		u32 length = GetU32();

		u8 *p = NULL;
		if (location == 0)
		{
			u8 *p = vga.box_ptr;
		}
		else if (location == 1)
		{
			u8 *p = vga.additional_ptr;
		}

		ReadBuffer(p + offset, length);
	}
	//CLEAR
	else if (cmd == 3)
	{
		u8 data = GetU8();
		Objects::Init();
		vga.Clear(data);
	}
	//AUTO-INIT - resolution
	else if (cmd == 4)
	{
		u8 resolution = GetU8();
		vga.AutoInit(resolution);
		Status(STATUS_OK);
	}
	//Fill
	else if (cmd == 5)
	{
		u8 location = GetU8();
		u32 offset = GetU32();
		u32 length = GetU32();
		u8 data = GetU8();
		Status(vga.Fill(location, offset, length, data));
	}
	//Draw feature
	else if (cmd == 6)
	{
		u8 feature_type = GetU8();
		if (feature_type == 0)
		{ //Rectangle
			u16 x = GetU16();
			u16 y = GetU16();
			u16 w = GetU16();
			u16 h = GetU16();
			u8 col = GetU8();
			DrawRect(&Canvas, x, y, w, h, col);
			Status(STATUS_OK);
		}
		else if (feature_type == 1)
		{ //Point
			u16 x = GetU16();
			u16 y = GetU16();
			u8 col = GetU8();
			DrawPoint(&Canvas, x, y, col);
			Status(STATUS_OK);
		}
		else if (feature_type == (1 | 64))
		{ //Points
			u16 count = GetU16();
			u8 col = GetU8();
			for (u16 i = 0; i < count; i++)
			{
				u16 x = GetU16();
				u16 y = GetU16();
				DrawPoint(&Canvas, x, y, col);
			}

			Status(STATUS_OK);
		}
		else if (feature_type == 2)
		{ //Line
			u16 x1 = GetU16();
			u16 y1 = GetU16();
			u16 x2 = GetU16();
			u16 y2 = GetU16();
			u8 col = GetU8();
			DrawLine(&Canvas, x1, y1, x2, y2, col);
			Status(STATUS_OK);
		}
		else if (feature_type == 3)
		{ //Circle
			u16 x1 = GetU16();
			u16 y1 = GetU16();
			u16 r = GetU16();
			u8 col = GetU8();
			u8 mask = GetU8();
			DrawCircle(&Canvas, x1, y1, r, col, mask);
			Status(STATUS_OK);
		}
		else if (feature_type == (3 | 128))
		{ //Circle
			u16 x1 = GetU16();
			u16 y1 = GetU16();
			u16 r = GetU16();
			u8 col = GetU8();
			u8 mask = GetU8();
			DrawFillCircle(&Canvas, x1, y1, r, col, mask);
			Status(STATUS_OK);
		}
		else
		{
			Status(STATUS_NOT_FOUND);
		}
	}
	//TEXT
	else if (cmd == 7)
	{
		u8 type = GetU8();
		if (type == 0)
		{ //Normal text 8x8 font
			u16 txt_length = GetU16();
			u16 x = GetU16();
			u16 y = GetU16();
			u8 color = GetU8();
			u8 fh = GetU8();
			u8 sx = GetU8();
			u8 sy = GetU8();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			DrawText(&Canvas, (const char *)c, x, y, color, Font_Copy, fh, sx, sy);
			Status(STATUS_OK);
		}
		else if (type == (0 | 64))
		{ //Normal text 8x8 font but with background
			u16 txt_length = GetU16();
			u16 x = GetU16();
			u16 y = GetU16();
			u8 color = GetU8();
			u8 bgcolor = GetU8();
			u8 fh = GetU8();
			u8 sx = GetU8();
			u8 sy = GetU8();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			DrawTextBg(&Canvas, (const char *)c, x, y, color, bgcolor, Font_Copy, fh, sx, sy);
			Status(STATUS_OK);
		}
		else if (type == 1)
		{ //Cursored text 8x8 font
			u16 txt_length = GetU16();
			u8 color = GetU8();
			u8 fh = GetU8();
			u8 sx = GetU8();
			u8 sy = GetU8();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			vga.DrawTextCursored((char *)c, fh, sx, sy, color);

			Status(STATUS_OK);
		}
		else if (type == (1 | 64))
		{ //Cursored text 8x8 font background
			u16 txt_length = GetU16();
			u8 color = GetU8();
			u8 bgcolor = GetU8();
			u8 fh = GetU8();
			u8 sx = GetU8();
			u8 sy = GetU8();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			vga.DrawTextCursored((char *)c, fh, sx, sy, color, bgcolor);

			Status(STATUS_OK);
		}
		else if (type == (128 | 0))
		{ //Set cursor
			u16 cX = GetU16();
			u16 cY = GetU16();
			vga.SetCursor(cX, cY);
			Status(STATUS_OK);
		}

		else
		{
			Status(STATUS_NOT_FOUND);
		}
	}
	//Objects
	else if (cmd == 8)
	{
		u8 index = GetU8();
		u8 action = GetU8();

		//CREATE:
		if (action == 0)
		{
			u8 type = GetU8();
			if (type == 1)
			{
				Objects::Push(ObjType_RECTANGLE, index);
				Status(STATUS_OK);
			}
			else if (type == 2)
			{
				Objects::Push(ObjType_TEXT, index);
				Status(STATUS_OK);
			}
			else if (type == 3)
			{
				Objects::Push(ObjType_CIRCLE, index);
				Status(STATUS_OK);
			}
			else
			{
				Status(STATUS_NOT_FOUND);
			}
		}
		//DELETE
		else if (action == 1)
		{
			Objects::Free(index);
			Status(STATUS_OK);
		}
		//MOVE:
		else if (action == 2)
		{
			u8 relative = GetU8();
			int16_t x = GetU16();
			int16_t y = GetU16();
			Object *obj = objects[index];
			obj->Move(x, y, relative);
			Status(STATUS_OK);
		}
		//Change Color
		else if (action == 3)
		{
			u8 color = GetU8();
			Object *obj = objects[index];
			obj->ChangeColor(color);
			Status(STATUS_OK);
		}
		//Visibility
		else if (action == 4)
		{
			u8 visibility = GetU8();
			Object *obj = objects[index];
			obj->ChangeVisibility(visibility);
			Status(STATUS_OK);
		}
		//Resize [Asume Rectangle]
		else if (action == 5)
		{
			u8 relative = GetU8();
			Object *_obj = objects[index];
			if (_obj->type != ObjType_RECTANGLE)
			{
				Status(STATUS_WRONG_TYPE);
				return;
			}
			int16_t w = GetU16();
			int16_t h = GetU16();
			ObjectRect *obj = (ObjectRect *)objects[index];
			obj->Resize(w, h, relative);
			Status(STATUS_OK);
		}
		//Change Radius [Assume Circle]
		else if (action == 6)
		{
			u8 relative = GetU8();
			Object *_obj = objects[index];
			if (_obj->type != ObjType_CIRCLE)
			{
				Status(STATUS_WRONG_TYPE);
				return;
			}
			int16_t r = GetU16();
			ObjectCircle *obj = (ObjectCircle *)objects[index];
			obj->SetRadius(r, relative);
			Status(STATUS_OK);
		}
		//Change Mask,Fill [Assume Circle]
		else if (action == 7)
		{
			Object *_obj = objects[index];
			if (_obj->type != ObjType_CIRCLE)
			{
				Status(STATUS_WRONG_TYPE);
				return;
			}
			int16_t mask = GetU8();
			int fill = GetU8();
			ObjectCircle *obj = (ObjectCircle *)objects[index];
			obj->SetMask(mask);
			obj->SetFill(fill);
			Status(STATUS_OK);
		}

		//List All
		else if (action == 255)
		{
			bool showXY = GetU8();
			for (u8 i = 0; i < 128; i++)
			{
				if (objects[i] != 0)
				{
					printf("%c%c", i, objects[i]->type);
					if (showXY)
					{
						u16 x = objects[i]->x;
						u16 y = objects[i]->y;
						u8 x1 = x >> 8;
						u8 x2 = x & 0x00FF;
						u8 y1 = y >> 8;
						u8 y2 = y & 0x00FF;
						printf("%c%c", x1, x2);
						printf("%c%c", y1, y2);
					}
					printf("%c", 0);
				}
			}
			return;
		}
	}
	
	//TEST cmd
	else if (cmd == 254)
	{
	}
	else if (cmd == 255)
	{
	}
	else
	{
		printf("cmd: %i", cmd);
		return;
	}

	Status(STATUS_UNTERMINATED);
}

void draw_no_signal()
{
	vga.Init(RES_EGA, FORM_8BIT, 255, 0);
}

int main()
{
	stdio_init_all();
	memcpy(Font_Copy, FontBold8x8, sizeof(FontBold8x8));
	Objects::Init();
	//draw_no_signal();

	while (true)
	{
		CommandRoutine();
	}
}
