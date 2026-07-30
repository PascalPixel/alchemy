typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Slot02000338 {
    u8 head[6];
    u16 angle;
};

extern struct Slot02000338 *Func_02001206(s32);

s32 Func_02000338(void)
{
    struct Slot02000338 *slot = Func_02001206(0);

    if ((u32) ((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}
