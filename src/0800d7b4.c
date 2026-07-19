typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u32 Func_0800d6d8(void *object, u32 value);

u32 Func_0800d7b4(u8 *object) {
    s32 index = *(s16 *)(object + 4);
    u8 *table = *(u8 **)object;
    u32 value = *(u32 *)(table + index * 4 + 4);

    if (object[87] == 0) {
        *(u16 *)(object + 4) = Func_0800d6d8(object, value);
    } else {
        *(u16 *)(object + 4) = *(u16 *)(object + 4) + 2;
    }
    return 1;
}
