typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
#define NULL ((void *)0)

void *Func_020013ca(s32, s32, s32, s32);
u8 Func_02001410(void *, s32);
u8 Func_02001428(void *, s32);
u8 Func_020014e0(void *, s32);

void *Func_02000048(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Func_020013ca(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Func_02001410(result, 0);
        Func_020014e0(result, 14);
        Func_02001428(result, 1);
        return result;
    }
    return NULL;
}
