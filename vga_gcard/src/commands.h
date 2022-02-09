u16 GetProperty(u8 prop, Object *obj);
void SetProperty(u8 prop,u16 value, Object *obj);
u8 *ExecuteCommand(u8 *ptr, Object *obj);
u8 *HandleJumps(u8 *ptr, Object *obj);
u8 *HandleLoads(u8 *ptr, Object *obj);
u8 *HandleALU(u8 *ptr, Object *obj);