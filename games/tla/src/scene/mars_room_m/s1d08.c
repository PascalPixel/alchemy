#include "mars_room_m.h"

void Func_02001d08(struct MarsRoomMVector *position, s32 value, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

    if (object != 0) {
        object->bounce = 0;
        object->gravity = 0;
        object->unk4c = -value;
        Func_02001cc4(object, 3);
        object->update = Func_02001988;
        if (flag != 0)
            object->unk5c[0] = 2;
        Func_02003118(object, 2);
    }
}

void Func_02001d50(struct MarsRoomMVector *position, s32 value, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

    if (object != 0) {
        object->bounce = 0;
        object->gravity = 0;
        object->unk4c = value;
        Func_02001cc4(object, 3);
        object->update = Func_02001b64;
        if (flag != 0)
            object->unk5c[0] = 2;
        Func_02003118(object, 2);
    }
}

void Func_02001d98(struct MarsRoomMVector *position, s32 value, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

    if (object != 0) {
        object->bounce = 0;
        object->gravity = 0;
        object->unk4c = -value;
        Func_02001cc4(object, 3);
        object->update = Func_02001b64;
        if (flag != 0)
            object->unk5c[0] = 2;
        Func_02003118(object, 2);
    }
}

s32 Func_02001de0(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 height;

    if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
        height = a->y - b->y;
        if (height > 0 && height < 0x100000) {
            if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
                return 1;
        }
    }
    return 0;
}
