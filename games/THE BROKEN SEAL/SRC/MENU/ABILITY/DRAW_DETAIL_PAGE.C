#include "TYPES.H"
#include "SYSTEM.H"

extern void *gMenuWork;
extern u8 Value_0000053a;

s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void RenderOutput_RedrawSavedRectFar(s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

/* The Psynergy counterpart of ItemMenu_DrawItemDetailPage: while flag
   0x151 is clear it names the selected entry, otherwise it clears flag
   0x2ff, then redraws the five row highlights. */
s32 PsynergyMenu_DrawDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 combined;
    s32 off;
    s32 row;

    menu = gMenuWork;
    combined = *(s32 *)(state + 8) * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    if (GameFlag_TestFar(0x151) == 0) {
        RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 44));
        WaitFrames(1);

        combined = *(s32 *)(state + 24);
        off = combined * 2 + 456;
        if (*(u16 *)((char *)menu + off) != 0) {
            s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&Value_0000053a;
            UiText_DrawCharacterAtOffsetFar(masked, *(s32 *)(menu + 44), 0, 0);
        }
    } else {
        GameFlag_ClearBitFar(0x2ff);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 0, row * 2 + 1, 15, 1, 14);
        } else {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 0, row * 2 + 1, 15, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
