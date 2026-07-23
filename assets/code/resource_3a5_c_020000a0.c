typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
#define NULL ((void *)0)

void *Func_02001dae(s32, s32, s32, s32);
u8 Func_02001e10(void *, s32);
s8 Func_02001eb8(void *, s32);

void *Func_020000a0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Func_02001dae(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Func_02001e10(result, 0);
        Func_02001eb8(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}
