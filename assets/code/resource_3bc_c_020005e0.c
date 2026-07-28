typedef signed int s32;
typedef unsigned char u8;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0xf];
    u8 mode;
    u8 filler24[0x31];
    u8 state;
};

extern struct Object *Func_02005002(s32);
extern s32 Func_02004f26(s32, s32, s32);
extern void Func_02004f60(s32, s32, s32, s32, s32, s32);
extern void Func_02004f7a(s32, s32, s32, s32, s32, s32);
extern void Func_02004ffa(s32);

void Func_020005e0(void)
{
    struct Object *object;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        object = Func_02005002(slot);
        if (Func_02004f26(0, object->x, object->z) == 0) {
            object->mode = 2;
            object->state = 0;
            x = object->x >> 20;
            z = object->z >> 20;
            Func_02004f60(83, 13, 1, 1, x, z);
            x2 = object->x >> 20;
            z2 = object->z >> 20;
            Func_02004f7a(83, 13, 1, 1, x2, z2 + 52);
            Func_02004ffa(slot + 517);
        }
    }
}
