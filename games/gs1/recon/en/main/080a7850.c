#include "types.h"
#include "global_cells.h"

extern u8 *Data_03001f2c;
void Func_080a9d84(void);
void Audio_PlayCue(s32);
void UiWindow_Commit(s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Func_08015078(s32, s32, s32, s32, s32);
s32 GameFlag_IsSet(s32);
void WaitFrames(s32);
s32 Func_080770c0(s32);
void UiWindow_Close(s32, s32);

s32 Func_080a7850(void)
{
    u8 *menu;
    s32 window;
    s32 row;
    s32 col;
    s32 x;
    s32 y;
    s32 flags;

    menu = Data_03001f2c;
    Func_080a9d84();
    Audio_PlayCue(112);

    window = *(s32 *)(menu + 0x10c);
    UiWindow_Commit(window);
    UiText_DrawAt(0, window, 0, 16);

    menu[0x14 + 5] = 13;
    menu[0x2f4] = 13;
    WaitFrames(1);

    UiWindow_Commit(*(s32 *)(menu + 0x24));

    y = 0;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 7; col++) {
            x = col * 8 + 48;
            if (Func_080770c0(x)) {
                UiText_DrawAt(0, *(s32 *)(menu + 0x24), col + 3, row + 17);
            }
        }
        y += 56;
    }

    flags = 7;
    for (;;) {
        if (Func_080770c0(0x150)) {
            break;
        }
        WaitFrames(1);
        if (*(volatile u8 *)ADDR_03001C94 & flags) {
            break;
        }
    }

    UiWindow_Commit(*(s32 *)(menu + 0x24));
    UiWindow_Close(*(s32 *)(menu + 0x24), 1);
    Audio_PlayCue(113);
    return 0;
}
