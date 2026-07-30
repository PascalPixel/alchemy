typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

void *Func_080a1814(void *);
void Func_080a1870(void *, s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);

void Func_080a3354(void)
{
    u8 *state = *(u8 **)0x03001F2C;
    s32 index;
    s16 *cursor;

    Func_080a1870(Func_080a1814(state), 2, 2, 8, 0);
    index = 3;
    cursor = (s16 *)(state + 0x14A);
    do {
        index--;
        *cursor-- = 0x1E;
    } while (index >= 0);

    *(u32 *)(state + 40) = 0;
    *(u32 *)(state + 36) = 0;
    *(u32 *)(state + 44) = Func_08015010(0, 17, 30, 3, 2);
    *(u32 *)(state + 32) = 0;
    state[0x110] = 0;
    state[0x111] = 0;
    state[0x112] = 8;
    state[0x113] = 2;
}
