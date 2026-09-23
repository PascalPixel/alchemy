#include "TYPES.H"

extern void *Data_03001f2c;
extern u8 Value_0000053a;

s32 Func_080770c0(s32 flag);
void Func_080770d0(s32 flag);
void Func_08015270(s32);
void Func_080030f8(s32);
#define WaitFrames Func_080030f8
void Func_08015080(s32, s32, s32, s32);
s32 Func_080a2268(s32, s32, s32, s32, s32, s32);

/* The Psynergy counterpart of ItemMenu_DrawItemDetailPage: while flag
   0x151 is clear it names the selected entry, otherwise it clears flag
   0x2ff, then redraws the five row highlights. */
s32 PsynergyMenu_DrawDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 combined;
    s32 off;
    s32 row;

    menu = Data_03001f2c;
    combined = *(s32 *)(state + 8) * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    if (Func_080770c0(0x151) == 0) {
        Func_08015270(*(s32 *)(menu + 44));
        WaitFrames(1);

        combined = *(s32 *)(state + 24);
        off = combined * 2 + 456;
        if (*(u16 *)((char *)menu + off) != 0) {
            s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&Value_0000053a;
            Func_08015080(masked, *(s32 *)(menu + 44), 0, 0);
        }
    } else {
        Func_080770d0(0x2ff);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Func_080a2268(*(s32 *)(menu + 32), 0, row * 2 + 1, 15, 1, 14);
        } else {
            Func_080a2268(*(s32 *)(menu + 32), 0, row * 2 + 1, 15, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
