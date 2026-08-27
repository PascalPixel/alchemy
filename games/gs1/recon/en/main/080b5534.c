#include "types.h"
#include "global_cells.h"

void Func_08015020(s32, u16 *);
void Func_08015000(void);
void Func_08015118(void);
void UiText_DrawQuantity(s32, s32);
s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void Func_08015148(s32);
void UiWindow_Close(s32, s32);
u8 *Runtime_GetObject(s32);

extern u8 Value_000026fa[];
extern u8 Value_00000ad0[];
extern u8 Value_00002850[];

void Func_080b5534(void)
{
    u16 text[64];
    u8 *name;
    s32 state;
    u16 ch;
    s32 cursor;
    s32 window;
    s32 range;
    s32 i;
    u8 *flag_ptr;
    s32 flag_val;

    state = 0;
    name = Runtime_GetObject(0);
    Func_08015020(0x903, text);
    ch = text[state];
    name[0] = (u8)ch;
    if (ch != 0) {
        i = 0;
        do {
            i++;
            if (i > 13)
                break;
            name[i] = (u8)text[i];
        } while (text[i] != 0);
    }
    name[14] = 0;

    Func_08015000();
    Audio_PlayCue(71);
    cursor = 0;
    *(volatile u16 *)0x04000000 = 0x1341;
    flag_ptr = (u8 *)(0x02000240 + 0x20c);
    flag_val = 2;

    for (;;) {
        *flag_ptr = flag_val;
        Func_08015118();
        UiText_DrawQuantity(0x3e7, 5);
        UiText_DrawQuantity(0, 3);
        UiText_DrawQuantity(1, 1);
        UiText_DrawQuantity(1, 2);
        UiText_DrawQuantity(2, 4);

        if (state == 0)
            window = UiWork_Create(cursor + (s32)Value_000026fa, 2, 10, 4);
        else
            window = UiWork_Create(cursor + (s32)Value_00000ad0, 2, 2, 4);

        WaitFrames(10);
        range = (s32)Value_00002850 - (s32)Value_000026fa;

        for (;;) {
            if (*(volatile u32 *)ADDR_03001B04 & 2) {
                if (state != 0) {
                    state = 0;
                } else {
                    cursor++;
                    state = 1;
                }
            }
            if (*(volatile u32 *)ADDR_03001B04 & 0x10)
                cursor++;
            if (*(volatile u32 *)ADDR_03001B04 & 0x20)
                cursor -= 2;
            if (*(volatile u32 *)ADDR_03001B04 & 0x40)
                state = 1;
            if (*(volatile u32 *)ADDR_03001B04 & 0x80)
                state = 0;
            if (*(volatile u32 *)ADDR_03001B04 & 0x100)
                cursor += 10;
            if (*(volatile u32 *)ADDR_03001B04 & 0x200)
                cursor -= 10;
            if (cursor < 0)
                cursor = 0;
            if ((u32)cursor >= (u32)(range + 5))
                cursor = range + 5;

            if (*(volatile u32 *)ADDR_03001B04 & 0x3f2)
                break;
            if (UiWork_IsCompleteFar() != 0 && (*(volatile u32 *)ADDR_03001B04 & 1))
                break;
            WaitFrames(1);
        }

        Func_08015148(1);
        UiWindow_Close(window, 1);
        flag_val = 0;
        flag_ptr = (u8 *)(*(s32 *)ADDR_03001E8C + 0x12f8);
    }
}
