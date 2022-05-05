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

#include "code.h"
inline void Object::ExecCommands()
{
	if (code_core.asset == 0)
		return;

	u8 own_time = code_core.own_time();
	if(own_time<1) own_time = 1;
	
	for (int i = 0; i < own_time; i++)
	{
		if (!time_reached(code_core.delay_to))
			break;
		if (!ExecuteCommand(this))
		{
			code_core.jmp(0);
			break;
		}
	}
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

inline void Object::clear()
{
	ObjectActionWR(clear, this);
}
inline void Object::Draw()
{
	ObjectActionWR(Draw, this);
}

inline void Objects::OnChangingAsset(u8 asset)
{
	for (u8 i = 0; i < OBJECTS_COUNT; i++)
	{
		if (objects[i] != 0)
		{
			Object *obj = objects[i];
			if (obj->code_core.asset == asset)
			{
				//NOTHING TO DO (CODE WILL BE RESET IN OnChangedAsset, code execution is not async)
			}
			else if (obj->type == ObjType_TEXT && ((ObjectText *)obj)->text_asset == asset)
			{
				obj->clear();
			}
		}
	}
}

inline void Objects::OnChangedAsset(u8 asset)
{
	for (u8 i = 0; i < OBJECTS_COUNT; i++)
	{
		if (objects[i] != 0)
		{
			Object *obj = objects[i];
			if (obj->code_core.asset == asset)
			{
				obj->SetCode(asset);
			}
			else if (obj->type == ObjType_TEXT && ((ObjectText *)obj)->text_asset == asset)
			{
				((ObjectText *)obj)->Draw();
			}
			else if (obj->type == ObjType_BLOB && ((ObjectBlob *)obj)->img_asset == asset)
			{
				((ObjectBlob *)obj)->Draw();
			}
		}
	}
}