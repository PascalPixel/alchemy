#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 field18;
    s32 field1C;
    u8 filler20[0x1c];
    s32 field3C;
    u8 filler40[0x15];
    u8 state;
    u8 filler56[0xe];
    s16 speedX;
    s16 speedY;
};

extern struct Object *Func_02005de2(s32);
extern s32 Func_02005c1a(s32, s32);
extern struct Object *Func_02005e1a(s32);
extern struct Object *Func_02005e26(s32);
extern void Func_02005e84(s32, s32, s32);
extern s32 Func_02005e3a(s32);
extern void Func_02005d80(s32, s32);
extern void Func_02005e8a(s32, s32);

void Func_020013c0(void)
{
    struct Object *object;
    struct Object *other;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    object = Func_02005de2(0);
    count = *(s32 *)0x0200DB80 + 1;
    kind = 41;
    x = object->x;
    y = object->y;
    *(s32 *)0x0200DB80 = count;
    switch (Func_02005c1a(count, 180)) {
    case 10:
        break;
    case 20:
        kind = 42;
        break;
    case 30:
        kind = 43;
        break;
    default:
        return;
    }
    object = Func_02005e1a(kind);
    if (object == 0) {
        return;
    }
    other = Func_02005e26(0);
    if (other != 0) {
        Func_02005e84(kind, other->x, other->z);
    }
    Func_02005d80(Func_02005e3a(kind), 0);
    object->state = 0;
    object->field18 = 0x6666;
    object->field1C = 0x6666;
    {
        s32 t = 0x40000;
        object->x = x + t;
        t += y;
        object->y = t;
        object->field3C = t;
    }
    object->speedX = 25;
    object->speedY = 128;
    Func_02005e8a(kind, 0x0200D96C);
}
