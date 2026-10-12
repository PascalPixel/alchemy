#include "types.h"

struct Orbiter {
    u8 unknown_00[85];
    u8 motion_flags;
    u8 unknown_56[12];
    u8 phase;
    u8 slot;
    s16 center_x;
    s16 center_z;
    struct Orbiter *link;
};

extern const u8 Data_0200e88c[];

struct Orbiter *Object_GetById(s32 id);
void Func_02005624(struct Orbiter *object, s32 mode);
void Func_02005634(struct Orbiter *object, const u8 *script);
void ObjectDispatch_SetSingleChildFlags(struct Orbiter *object, s32 flags);
void Animation_ApplyChildValues(struct Orbiter *object, s32 value);

void Func_020022d0(s32 slot, s32 value, s32 target_id, s32 angle)
{
    struct Orbiter *object = Object_GetById(slot);
    struct Orbiter *target = Object_GetById(target_id);

    Func_02005624(object, 0);
    Func_02005634(object, Data_0200e88c);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motion_flags = 0;
    object->link = target;
    object->phase = angle / 256;
    object->slot = slot;
    Animation_ApplyChildValues(object, value);
}
