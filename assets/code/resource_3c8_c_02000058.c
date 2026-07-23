typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_020000b2(void *, s32);
void *Func_02004e4e(s32, s32, s32, s32);
void Func_02004ed6(void *, s32);

void *Func_02000058(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *object;
    u8 *entry;
    s32 mask;
    u8 flags;

    object = Func_02004e4e(arg3, arg0, arg1, arg2);
    if (object != 0) {
        entry = *(u8 **)((u8 *)object + 0x50);
        mask = 13;
        flags = entry[9];
        mask = -mask;
        mask &= flags;
        entry[9] = mask;
        Func_020000b2(object, 0xE);
        Func_02004ed6(object, 1);
        return object;
    }
    return 0;
}
