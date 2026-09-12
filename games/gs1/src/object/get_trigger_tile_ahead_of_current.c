#include "types.h"

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

extern s32 Data_02000240[];

void Func_0800447c(s32, s32, struct Vec *);
s32 Func_080091a8(s32, s32, s32);
struct Object *Func_0808ba1c(s32);
s32 Func_0808d48c(s32, s32);

s32 Func_0808ce74(void)
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
    obj = Func_0808ba1c(Data_02000240[125]);
    state = *(u8 **)0x03001ebc;
    map = *(u8 **)0x03001e70;
    if (obj != 0) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        Func_0800447c(0x100000, obj->angle, &pos);
        if (*(s16 *)(state + 0x19e) == 3) {
            cell = (u8 *)0x02020000 + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
        } else {
            base = *(u8 **)(map + 0x130);
            cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
        }
        kind = cell[2];
        if ((u32)(kind - 242) <= 5) {
            height = Func_080091a8(obj->kind, pos.x, pos.z);
            if (height >= obj->pos.y && height <= obj->pos.y + 0x400000) {
                result = kind;
            }
        } else if (Func_0808d48c(3, kind) != 0) {
            result = kind;
        }
    }
    return result;
}
