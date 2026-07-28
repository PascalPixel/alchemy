typedef signed int s32;
typedef unsigned char u8;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

extern s32 Func_02004fea(s32, s32, s32);
extern struct Object *Func_020050da(s32);
extern struct Object *Func_020050f4(s32);
extern struct Object *Func_0200510a(s32);

s32 Func_020006a4(s32 x, s32 z)
{
    struct Object *object;

    if (Func_02004fea(0, x, z) == 255) {
        return -2;
    }
    object = Func_020050da(15);
    x = x >> 20;
    z = z >> 20;
    if (object->x >> 20 == x && object->z >> 20 == z) {
        return -1;
    }
    object = Func_020050f4(16);
    if (object->x >> 20 == x && object->z >> 20 == z) {
        return -1;
    }
    object = Func_0200510a(17);
    if (object->x >> 20 == x && object->z >> 20 == z) {
        return -1;
    }
    return 0;
}
