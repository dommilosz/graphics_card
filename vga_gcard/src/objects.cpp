#include "objects.h"

inline void Object::Move(int16_t x, int16_t y, u8 relative)
{
	if (relative == 0)
	{
		this->x = x;
		this->y = y;
	}
	else
	{
		this->x += x;
		this->y += y;
	}
	Objects::OnChange();
}

inline void Object::ChangeColor(u8 c)
{
	this->color = c;
	Objects::OnChange();
}

inline void Object::ChangeVisibility(u8 v)
{
	this->visibility = v;
	Objects::OnChange();
}

inline u8 Object::Rotate(u16 deg)
{
	if (deg > 360)
	{
		return 1;
	}

	Objects::OnChange();
}

inline void ObjectRect::Resize(int16_t w, int16_t h, u8 relative)
{
	if (relative == 0)
	{
		this->w = w;
		this->h = h;
	}
	else
	{
		this->w += w;
		this->h += h;
	}
	Objects::OnChange();
}

inline void ObjectRect::Draw()
{
	DrawRect(&Canvas, x, y, w, h, color);
}

inline void ObjectText::SetText(char *c)
{
	free(text);
	int len = strlen(c);
	text = (char *)malloc(len + 1);
	text[len] = 0;
	Objects::OnChange();
}

inline void ObjectText::Draw()
{
	DrawText(&Canvas, (const char *)text, x, y, color, Font_Copy, fH, sX, sY);
}

inline ObjectText::~ObjectText()
{
	free(text);
}

inline void ObjectCircle::SetRadius(int16_t r, bool relative)
{
	if (!relative)
	{
		this->r = r;
	}
	else
	{
		this->r += r;
	}
	Objects::OnChange();
}

inline void ObjectCircle::SetMask(u8 mask)
{
	this->mask = mask;
	Objects::OnChange();
}

inline void ObjectCircle::SetFill(bool fill)
{
	this->full = fill;
	Objects::OnChange();
}

inline void ObjectCircle::Draw()
{
	if (this->full)
	{
		DrawFillCircle(&Canvas, this->x, this->y, this->r, this->color, this->mask);
	}
	else
	{
		DrawCircle(&Canvas, this->x, this->y, this->r, this->color, this->mask);
	}
}

inline void Objects::PushObject(Object *obj, u8 index)
{
	if (objects[index] != 0)
	{
		Free(index);
	}
	objects[index] = (Object *)malloc(sizeof(Object));
	memcpy(objects[index], obj, sizeof(Object));
	Objects::OnChange();
}

inline void Objects::DrawAll()
{
	vga.Clear();
	for (u8 i = 0; i < 128; i++)
	{
		if (objects[i] != 0)
		{
			Draw(i);
		}
	}
}

inline void Objects::Draw(int index)
{
	if (objects[index]->visibility == 0)
	{
		return;
	}
	ObjectAction(Draw, index);
}

inline void Objects::Free(int index)
{
	ObjectAction(Free, index);
}
inline void Objects::OnChange()
{
	Objects::DrawAll();
}

inline void Objects::Push(u8 type, u8 index)
{
	if (objects[index] != 0)
	{
		Free(index);
	}
	objects[index] = (Object *)malloc(sizeof(Object));
	memset(objects[index], 0, sizeof(Object));
	objects[index]->type = type;
	objects[index]->visibility = true;
	Objects::OnChange();
}