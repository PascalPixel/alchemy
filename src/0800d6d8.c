typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned char u8;

s32 Func_0800d6d8(void *object, u32 key) {
    u32 *entries;
    s32 index;
    u16 *field = (u16 *)((u8 *)object + 0x5E);
    s32 zero = 0;

    *field = zero;
    if (key == 0) {
        return 0;
    }

    key &= 0xBFFFFFFF;
    entries = *(u32 **)object;
    index = 0;
    do {
        if (*entries++ == key) {
            return index + 1;
        }
        index++;
    } while (index <= 0x3FF);
    return 0;
}
