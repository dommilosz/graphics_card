#include "objects.h"

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

inline size_t Object::GetSize()
{
	if (this->type == ObjType_RECTANGLE)
	{
		return sizeof(ObjectRect);
	}
	else if (this->type == ObjType_CIRCLE)
	{
		return sizeof(ObjectCircle);
	}
	else if (this->type == ObjType_LINE)
	{
		return sizeof(ObjectLine);
	}
	else if (this->type == ObjType_TEXT)
	{
		return sizeof(ObjectText);
	}
	return sizeof(Object);
}

inline void Objects::Push(u8 type, u8 index)
{
	if (objects[index] != 0)
	{
		Free(index);
	}
	size_t size = objects[index]->GetSize();
	objects[index] = (Object *)malloc(size);
	memset(objects[index], 0, size);
	objects[index]->type = type;
	objects[index]->visibility = true;

	if(objects[index]->type == ObjType_TEXT){
		((ObjectText *)objects[index])->text = NULL;
	}

	Objects::OnChange();
}