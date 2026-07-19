typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_0801ce48(void *base) {
    s32 original;
    u16 test;
    s32 current;

    base = (u8 *)base + 0x574;
    original = M2C_FIELD(base, u16 *, 0);
    test = original;
    current = test;

    if (current == 0) {
        current = 2;
    } else {
        current = original + 0xFFFF;
    }
    M2C_FIELD(base, u16 *, 0) = current;
}
