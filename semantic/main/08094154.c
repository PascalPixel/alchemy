typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_0808ba1c(u32);
void *Func_08185000(s16);

s32 Func_08094154(u32 arg0, s32 *out)
{
    s32 y;
    u8 *object;
    u8 *world;
    s32 x;
    void *target;
    s32 adjustment;
    s32 *first;

    object = Func_0808ba1c(arg0);
    if (object == 0)
        return -1;

    world = *(u8 **)0x03001E70;
    y = FIELD(object, s32, 0x10) -
        (FIELD(world, s32, 0xE8) & 0xFFFF0000) - FIELD(object, s32, 0xC);
    x = FIELD(object, s32, 8) - (FIELD(world, s32, 0xE4) & 0xFFFF0000);
    first = out;
    out++;
    if (x < 0)
        x += 0xFFFF;
    *first = x >> 16;
    if (y < 0)
        y += 0xFFFF;
    *out = y >> 16;

    if ((FIELD(object, u8, 0x54) & 0xF) == 1) {
        target = FIELD(object, void *, 0x50);
        adjustment = FIELD(Func_08185000(FIELD(FIELD(target, void *, 0x28), s16, 0)), s8, 8);
        *out -= adjustment;
    }

    return 0;
}
