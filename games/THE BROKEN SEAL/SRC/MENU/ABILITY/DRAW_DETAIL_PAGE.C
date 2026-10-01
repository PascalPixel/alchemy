#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"

extern void *gMenuWork;
extern u8 MsgAbilityDescription;

s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void RenderOutput_RedrawSavedRectFar(s32);
void RenderOutput_ClearListFar(s32);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

/* The Japanese rows' highlight starts a tile in and is a tile narrower. */
#if defined(TBS_EDITION_JA)
#define DETAIL_ROW_X     1
#define DETAIL_ROW_WIDTH 14
#else
#define DETAIL_ROW_X     0
#define DETAIL_ROW_WIDTH 15
#endif

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
#if defined(TBS_EDITION_JA)
        RenderOutput_ClearListFar(*(s32 *)(menu + 44));
#else
        RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 44));
#endif
        WaitFrames(1);

        combined = *(s32 *)(state + 24);
        off = combined * 2 + 456;
        if (*(u16 *)((char *)menu + off) != 0) {
            s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&MsgAbilityDescription;
#if defined(TBS_EDITION_JA)
            UiText_DrawMessageAt(masked, *(s32 *)(menu + 44), 0, 0);
#else
            UiText_DrawCharacterAtOffsetFar(masked, *(s32 *)(menu + 44), 0, 0);
#endif
        }
    } else {
        GameFlag_ClearBitFar(0x2ff);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 14);
        } else {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
