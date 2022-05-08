u8 last_asset_write_finished = 0;
void CommandRoutine(data_source data_source)
{
	UTIL_DATA_SOURCE = data_source;
	int cmd = GetU8();

	if (cmd <= 0)
		return;

	//INIT - resolution, tiles, board_size,
	if (cmd == 1)
	{
		u8 resolution = WaitForByte();
		u8 tiles = WaitForByte();
		u32 board_size = WaitForByte32();

		vga.Init(resolution, tiles, board_size);

		Status(STATUS_OK);
	}
	//WRITE - (0-box, 1-add), offset, length, data......
	else if (cmd == 2)
	{
		u32 offset = WaitForByte32();
		u32 length = WaitForByte32();

		u8 *p = vga.box_ptr;

		ReadBuffer(p + offset, length);
	}
	//CLEAR - color
	else if (cmd == 3)
	{
		u8 data = WaitForByte();
		Objects::Init();
		vga.Clear(data);
		Status(STATUS_OK);
	}
	//AUTO-INIT - resolution
	else if (cmd == 4)
	{
		u8 resolution = WaitForByte();
		vga.AutoInit(resolution);
		Status(STATUS_OK);
	}
	//Fill
	else if (cmd == 5)
	{
		u8 location = WaitForByte();
		u32 offset = WaitForByte32();
		u32 length = WaitForByte32();
		u8 data = WaitForByte();
		Status(vga.Fill(location, offset, length, data));
	}
	//Draw feature
	else if (cmd == 6)
	{
		u8 feature_type = WaitForByte();
		if (feature_type == 0)
		{ //Rectangle
			u16 x = WaitForByte16();
			u16 y = WaitForByte16();
			u16 w = WaitForByte16();
			u16 h = WaitForByte16();
			u8 col = WaitForByte();
			DrawRect(&Canvas, x, y, w, h, col);
			Status(STATUS_OK);
		}
		else if (feature_type == 1)
		{ //Point
			u16 x = WaitForByte16();
			u16 y = WaitForByte16();
			u8 col = WaitForByte();
			DrawPoint(&Canvas, x, y, col);
			Status(STATUS_OK);
		}
		else if (feature_type == (1 | 64))
		{ //Points
			u16 count = WaitForByte16();
			u8 col = WaitForByte();
			for (u16 i = 0; i < count; i++)
			{
				u16 x = WaitForByte16();
				u16 y = WaitForByte16();
				DrawPoint(&Canvas, x, y, col);
			}

			Status(STATUS_OK);
		}
		else if (feature_type == 2)
		{ //Line
			u16 x1 = WaitForByte16();
			u16 y1 = WaitForByte16();
			u16 x2 = WaitForByte16();
			u16 y2 = WaitForByte16();
			u8 col = WaitForByte();
			DrawLine(&Canvas, x1, y1, x2, y2, col);
			Status(STATUS_OK);
		}
		else if (feature_type == 3)
		{ //Circle
			u16 x1 = WaitForByte16();
			u16 y1 = WaitForByte16();
			u16 r = WaitForByte16();
			u8 col = WaitForByte();
			u8 mask = WaitForByte();
			DrawCircle(&Canvas, x1, y1, r, col, mask);
			Status(STATUS_OK);
		}
		else if (feature_type == (3 | 128))
		{ //Circle fill
			u16 x1 = WaitForByte16();
			u16 y1 = WaitForByte16();
			u16 r = WaitForByte16();
			u8 col = WaitForByte();
			u8 mask = WaitForByte();
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
		u8 type = WaitForByte();
		if (type == 0)
		{ //Normal text 8x8 font
			u16 x = WaitForByte16();
			u16 y = WaitForByte16();
			u8 color = WaitForByte();
			u8 fh = WaitForByte();
			u8 sx = WaitForByte();
			u8 sy = WaitForByte();
			u16 txt_length = WaitForByte16();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			DrawText(&Canvas, (const char *)c, x, y, color, Font_Copy, fh, sx, sy);
			Status(STATUS_OK);
		}
		else if (type == (0 | 64))
		{ //Normal text 8x8 font but with background
			u16 x = WaitForByte16();
			u16 y = WaitForByte16();
			u8 color = WaitForByte();
			u8 bgcolor = WaitForByte();
			u8 fh = WaitForByte();
			u8 sx = WaitForByte();
			u8 sy = WaitForByte();
			u16 txt_length = WaitForByte16();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			DrawTextBg(&Canvas, (const char *)c, x, y, color, bgcolor, Font_Copy, fh, sx, sy);
			Status(STATUS_OK);
		}
		else if (type == 1)
		{ //Cursored text 8x8 font
			u8 color = WaitForByte();
			u8 fh = WaitForByte();
			u8 sx = WaitForByte();
			u8 sy = WaitForByte();
			u16 txt_length = WaitForByte16();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			vga.DrawTextCursored((char *)c, fh, sx, sy, color);

			Status(STATUS_OK);
		}
		else if (type == (1 | 64))
		{ //Cursored text 8x8 font background
			u8 color = WaitForByte();
			u8 bgcolor = WaitForByte();
			u8 fh = WaitForByte();
			u8 sx = WaitForByte();
			u8 sy = WaitForByte();
			u16 txt_length = WaitForByte16();
			u8 c[txt_length + 1];
			ReadBuffer(c, txt_length);
			c[txt_length] = 0;
			vga.DrawTextCursored((char *)c, fh, sx, sy, color, bgcolor);

			Status(STATUS_OK);
		}
		else if (type == (128 | 0))
		{ //Set cursor
			u16 cX = WaitForByte16();
			u16 cY = WaitForByte16();
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
		u8 index = WaitForByte();
		u8 action = WaitForByte();

		//CREATE:
		if (action == 0)
		{
			u8 type = WaitForByte();
			if (type == ObjType_RECTANGLE)
			{
				Objects::Push(ObjType_RECTANGLE, index);
				Status(STATUS_OK);
			}
			else if (type == ObjType_TEXT)
			{
				Objects::Push(ObjType_TEXT, index);
				Status(STATUS_OK);
			}
			else if (type == ObjType_CIRCLE)
			{
				Objects::Push(ObjType_CIRCLE, index);
				Status(STATUS_OK);
			}
			else if (type == ObjType_LINE)
			{
				Objects::Push(ObjType_LINE, index);
				Status(STATUS_OK);
			}
			else if (type == ObjType_BLOB)
			{
				Objects::Push(ObjType_BLOB, index);
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
			u8 relative = WaitForByte();
			int16_t x = WaitForByte16();
			int16_t y = WaitForByte16();
			ObjAssertExist();
			Object *obj = objects[index];
			obj->Move(x, y, relative);
			Status(STATUS_OK);
		}
		//Change Color
		else if (action == 3)
		{
			u8 color = WaitForByte();
			ObjAssertExist();
			Object *obj = objects[index];
			obj->ChangeColor(color);
			Status(STATUS_OK);
		}
		//Visibility
		else if (action == 4)
		{
			u8 visibility = WaitForByte();
			ObjAssertExist();
			Object *obj = objects[index];
			obj->ChangeVisibility(visibility);
			Status(STATUS_OK);
		}
		//Resize [Asume Rectangle]
		else if (action == 5)
		{
			u8 relative = WaitForByte();
			int16_t w = WaitForByte16();
			int16_t h = WaitForByte16();
			ObjAssertExist();
			Object *obj = objects[index];
			obj->Resize(w, h, relative);
			Status(STATUS_OK);
		}
		//Change Radius [Assume Circle]
		else if (action == 6)
		{
			u8 relative = WaitForByte();
			int16_t r = WaitForByte16();
			ObjAssert(ObjType_CIRCLE);
			ObjectCircle *obj = (ObjectCircle *)objects[index];
			obj->SetRadius(r, relative);
			Status(STATUS_OK);
		}
		//Change Mask,Fill [Assume Circle]
		else if (action == 7)
		{
			u8 mask = WaitForByte();
			u8 fill = WaitForByte();
			ObjAssert(ObjType_CIRCLE);
			ObjectCircle *obj = (ObjectCircle *)objects[index];
			obj->SetMask(mask);
			obj->SetFill(fill);
			Status(STATUS_OK);
		}
		//Move line [Assume Line]
		else if (action == 8)
		{
			u8 relative = WaitForByte();
			int16_t x = WaitForByte16();
			int16_t y = WaitForByte16();
			int16_t x2 = WaitForByte16();
			int16_t y2 = WaitForByte16();
			ObjAssert(ObjType_LINE);
			ObjectLine *obj = (ObjectLine *)objects[index];
			obj->Move(x, y, x2, y2, relative);
			Status(STATUS_OK);
		}
		//Set text [Assume Text]
		else if (action == 9)
		{
			ObjAssert(ObjType_TEXT);
			ObjectText *obj = (ObjectText *)objects[index];
			u8 asset = WaitForByte();
			obj->SetText(asset);
			Status(STATUS_OK);
		}
		//Set text scale [Assume Text]
		else if (action == 10)
		{
			u16 sX = WaitForByte16();
			u16 sY = WaitForByte16();
			u16 fH = WaitForByte16();
			ObjAssert(ObjType_TEXT);
			ObjectText *obj = (ObjectText *)objects[index];
			obj->SetScale(sX, sY, fH);
			Status(STATUS_OK);
		}
		//set code
		else if (action == 11)
		{
			Object *obj = (Object *)objects[index];
			u8 code_asset = WaitForByte();
			ObjAssertExist();
			obj->SetCode(code_asset);
			Status(STATUS_OK);
		}
		//Set blob asset [Assume blob]
		else if (action == 12)
		{
			u8 code_asset = WaitForByte();
			ObjAssert(ObjType_BLOB);
			ObjectBlob *obj = (ObjectBlob *)objects[index];
			obj->SetImage(code_asset);
			Status(STATUS_OK);
		}
		//read code register
		else if (action == 252)
		{
			u8 reg = WaitForByte();

			if (reg > 31)
			{
				Status(STATUS_ILLEGAL_VALUE);
			}

			ObjAssertExist();
			Object *obj = (Object *)objects[index];
			putu16(obj->code_core.mem[reg]);
			Status(STATUS_OK);
		}
		//Wrtie to code register
		else if (action == 253)
		{
			u8 reg = WaitForByte();
			u8 value = WaitForByte16();

			if (reg > 31)
			{
				Status(STATUS_ILLEGAL_VALUE);
			}

			ObjAssertExist();
			Object *obj = (Object *)objects[index];
			obj->code_core.mem[reg] = value;
			Status(STATUS_OK);
		}
		//Object info
		else if (action == 254)
		{
			Object *obj = (Object *)objects[index];
			uint8_t type;
			uint16_t x, y, w, h;
			uint8_t color;
			uint8_t visibility;
			uint8_t code_asset, text_asset;
			uint16_t code_pc;
			if (obj != 0)
			{
				type = obj->type;
				x = obj->x;
				y = obj->y;
				w = obj->w;
				h = obj->h;
				color = obj->color;
				visibility = obj->visibility;
				code_asset = obj->code_core.asset;
				if (obj->type == ObjType_TEXT)
				{
					text_asset = ((ObjectText *)obj)->text_asset;
				}
				code_pc = obj->code_core.PC();
			}
			putu(type);
			putu16(x);
			putu16(y);
			putu16(w);
			putu16(h);
			putu(color);
			putu(visibility);

			putu(code_asset);
			putu(text_asset);
			putu16(code_pc);

			if(obj == 0){
				Status(STATUS_ILLEGAL_OBJECT);
			}

			Status(STATUS_OK);
		}
		//List All
		else if (action == 255)
		{
			for (u8 i = 0; i < OBJECTS_COUNT; i++)
			{
				if (objects[i] != 0)
				{
					putu(i);
					putu(objects[i]->type);
				}
			}
			Status(STATUS_OK);
		}
	}

	//PLAY SOUND
	else if (cmd == 9)
	{
		u16 freq = WaitForByte16();
		u16 duration = WaitForByte16();
		u8 type = WaitForByte();
		PlaySoundFreq(freq, duration, type);
		Status(STATUS_OK);
	}
	//PLAY SOUND (From asset)
	else if (cmd == (9 | 64))
	{
		u8 asset = WaitForByte();
		u8 repeat = WaitForByte();
		PlaySoundAsset(asset, repeat);
		Status(STATUS_OK);
	}
	//STOP Sound
	else if (cmd == (9 | 128))
	{
		StopSound();
		Status(STATUS_OK);
	}

	//write asset
	else if (cmd == 10)
	{
		u8 asset = WaitForByte();
		u16 length = WaitForByte16();
		Objects::OnChangingAsset(asset);
		WriteAssetFromDataSource(asset, length);
		Objects::OnChangedAsset(asset);
		I2CCom.lastPing = millis();
		Status(STATUS_OK);
	}
	//write asset parted
	else if (cmd == 11)
	{
		u8 asset = WaitForByte();
		u16 offset = WaitForByte16();
		u16 length = WaitForByte16();
		u8 segment_location = WaitForByte();
		if (segment_location == 0 || segment_location == 254)
		{
			Objects::OnChangingAsset(asset);
			last_asset_write_finished = 0;
		}
		WriteAssetFromDataSource(asset, length, offset);
		if (segment_location == 255 || segment_location == 254)
		{
			Objects::OnChangedAsset(asset);
			last_asset_write_finished = 1;
		}
		I2CCom.lastPing = millis();
		Status(STATUS_OK);
	}
	//asset - checksum
	else if (cmd == 12)
	{
		u8 asset = WaitForByte();
		u16 length = WaitForByte16();
		putu32(AssetCRC32(asset, length));
		Status(STATUS_OK);
	}
	//redraw
	else if (cmd == 13)
	{
		vga.Clear();
	}
	//Status:
	else if (cmd == 253)
	{
		putu(vga.initialized);
		putu(last_asset_write_finished);
		Status(STATUS_OK);
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