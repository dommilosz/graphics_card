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
	objects[index] = (Object *)(vga.obj_mem_ptr + (OBJECT_ALLOC * index));
	memset(objects[index], 0, OBJECT_ALLOC);
	objects[index]->type = type;
	objects[index]->ChangeVisibility(true);

	if (objects[index]->type == ObjType_TEXT)
	{
		((ObjectText *)objects[index])->text = NULL;
		((ObjectText *)objects[index])->SetScale(1, 1, 8);
		((ObjectText *)objects[index])->ChangeColor(255);
	}
	ONC = true;

	Objects::OnChange();
}

#include "commands.cpp"
inline void Object::ExecCommands()
{
	if (cmd_delay_left > 0)
	{
		cmd_delay_left--;
		return;
	}

	if (cmd_pointer == 0)
		return;

	if (cmd_pointer_instr >= (cmd_pointer + OBJECT_CODE_SIZE))
	{
		cmd_pointer_instr = cmd_pointer;
		return;
	}

	if (cmd_pointer_instr == 0 || cmd_pointer_instr[0] == 0)
	{
		cmd_pointer_instr = cmd_pointer;
		return;
	}

	cmd_pointer_instr = ExecuteCommand(cmd_pointer_instr, this);
}