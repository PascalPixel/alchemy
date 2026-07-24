typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

void Func_080f9010(s32);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08009080(void *, s32);
void Func_080030f8(s32);

void *Func_0809a3c4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *obj;
    s32 v;
    s32 zero;

    Func_080f9010(138);
    obj = Func_08096c80(215, arg0, arg1, arg2);
    if (obj != NULL) {
        *(s32 *)(obj + 0x1C) = 0x4000;
        *(s32 *)(obj + 0x18) = 0x4000;
        *(s32 *)(obj + 0x30) = 0x30000;
        *(s32 *)(obj + 0x34) = 0x30000;
        zero = 0;
        *(s8 *)(obj + 0x5A) = zero;
        Func_08009080(obj, 1);
        v = *(s32 *)(obj + 0x18);
        if (v < 0x10000) {
            do {
                v += 0x800;
                *(s32 *)(obj + 0x1C) = v;
                *(s32 *)(obj + 0x18) = v;
                *(u16 *)(obj + 6) += 0x2000;
                Func_080030f8(1);
                v = *(s32 *)(obj + 0x18);
            } while (v <= 0xFFFF);
        }
        *(u16 *)(obj + 6) = (u16) arg3;
    }
    return obj;
}
