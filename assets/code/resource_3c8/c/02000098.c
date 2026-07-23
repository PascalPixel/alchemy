typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_020000f6(void *, s32);
void *Func_02004e8e(s32, s32, s32, s32);

void *Func_02000098(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *object;
    u8 *entry;
    s32 flags;
    s32 result;

    object = Func_02004e8e(arg3, arg0, arg1, arg2);
    if (object != 0) {
        entry = *(u8 **)((u8 *)object + 0x50);
        flags = entry[9];
        flags = (flags & -13) | 4;
        entry[9] = flags;
        Func_020000f6(object, 0xF);
        flags = *((u8 *)object + 0x23);
        result = 2;
        result |= flags;
        *((u8 *)object + 0x23) = result;
        return object;
    }
    return 0;
}
