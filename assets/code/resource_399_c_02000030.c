typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

extern s32 Func_02002222(void);
extern s32 Func_02002210(s32, s32);
extern s32 Func_02002230(void);
extern s32 Func_0200221e(s32, s32);

s32 Func_02000030(struct Object *object)
{
    switch (object->counter) {
    case 6:
        object->x += 0xffffc000;
        object->z += 0x2000;
        break;
    case 4:
        object->x += 0x2000;
        object->z += 0xf868f001;
        break;
    case 2:
        object->x += 0x1000;
        object->z += 0xfffff800;
        break;
    case 0:
        object->x += 0x1000;
        object->z += 0xfffff800;
        if (object->mode != 0) {
            object->counter = Func_02002210(Func_02002222(), 40) + 40;
        } else {
            object->counter = Func_0200221e(Func_02002230(), 20) + 20;
        }
        break;
    }
    object->counter--;
    return 1;
}
