#include "objects.h"

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
	objects[index] = 0;
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
	ONC = false;
	size_t size = objects[index]->GetSize();
	objects[index] = (Object *)(obj_mem + (OBJECT_ALLOC * index));
	memset(objects[index], 0, OBJECT_ALLOC);
	objects[index]->type = type;
	objects[index]->ChangeVisibility(true);

	if (objects[index]->type == ObjType_TEXT)
	{
		((ObjectText *)objects[index])->SetScale(1, 1, 8);
		((ObjectText *)objects[index])->ChangeColor(255);
	}
	ONC = true;

	Objects::OnChange();
}

#include "commands.cpp"
inline void Object::ExecCommands()
{
	if (!time_reached(cmd_delay_to))return;

	if (cmd_asset == 0)
		return;

	if (ReadAsset(cmd_asset, cmd_pointer) == 0)
	{
		cmd_pointer = 0;
		return;
	}

	ExecuteCommand(this);
}

inline void Objects::ExecCode()
{	
	for (u8 i = 0; i < OBJECTS_COUNT; i++)
	{
		if (objects[i] != 0)
		{
			objects[i]->ExecCommands();
		}
	}
	if (cmd_changed)
	{
		Objects::DrawAll();
		cmd_changed = false;
	}
}

inline void Object::clear(){
	ObjectActionWR(clear,this);
}