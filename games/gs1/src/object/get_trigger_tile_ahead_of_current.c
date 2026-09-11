#include "types.h"
#include "scene.h"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Object {
    u8 pad0[6];
    u16 angle;
    struct Vec pos;
    u8 pad1[14];
    u8 kind;
};

extern s32 gCell[];

struct Object *Obj_Run(s32);

s32 Obj_unk2_2(void)
{
    u8 *state;
    u8 *map;
    struct Object *obj;
    struct Vec pos;
    u8 *cell;
    u8 *base;
    s32 kind;
    s32 height;
    s32 result;

    result = 0;
    obj = Obj_Run(gCell[125]);
    state = *(u8 **)0x03001ebc;
    map = *(u8 **)0x03001e70;
    if (obj != 0) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        Obj_Place(0x100000, obj->angle, &pos);
        if (*(s16 *)(state + 0x19e) == 3) {
            cell = (u8 *)0x02020000 + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
        } else {
            base = *(u8 **)(map + 0x130);
            cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
        }
        kind = cell[2];
        if ((u32)(kind - 242) <= 5) {
            height = Obj_unk2(obj->kind, pos.x, pos.z);
            if (height >= obj->pos.y && height <= obj->pos.y + 0x400000) {
                result = kind;
            }
        } else if (Obj_Apply(3, kind) != 0) {
            result = kind;
        }
    }
    return result;
}
