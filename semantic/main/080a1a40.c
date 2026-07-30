typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080a1a40(s32 x, s32 y) {
    void *state = *(void **)0x03001F2C;
    void *source;
    void *target;
    u32 xValue;
    u32 yValue;

    xValue = *(u8 *)(0x080AF294
        + ((*(volatile u32 *)0x03001E40 >> 1) & 7)) + x;
    source = FIELD(state, void *, 0x10);
    xValue += FIELD(source, u16, 0xC) * 8 + 8;
    target = FIELD(state, void *, 0x14);

    FIELD(target, u16, 6) = xValue;
    FIELD(target, u16, 0x16) =
        (FIELD(target, u16, 0x16) & 0xFE00) | (xValue & 0x1FF);

    yValue = *(u8 *)(0x080AF29D
        + ((*(volatile u32 *)0x03001E40 >> 1) & 7)) + y
        + FIELD(source, u16, 0xE) * 8 + 8;
    FIELD(target, u16, 8) = yValue;
    FIELD(target, u8, 0x14) = yValue;
}
