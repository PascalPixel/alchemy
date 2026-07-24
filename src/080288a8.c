typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

s32 Func_080162d4(s32, s32, s32, s32, s32);
extern u8 Data_03001f38;

void Func_080288a8(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u8 *state;
    s32 i;

    state = *(u8 **)&Data_03001f38;

    *(u16 *)(state + 144) = (u16)(a2 + 2);
    *(u16 *)(state + 146) = (u16)a3;
    *(u16 *)(state + 148) = (u16)a1;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)(a1 << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)(a0 << 3);
        a0 += 3;
    }

    *(s32 *)(state + 120) = Func_080162d4(a0, a1, *(s16 *)(state + 144), 3, 2);
}
