#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"

extern void *gMenuWork;
extern u8 MsgItemPlainName;

void RenderOutput_RedrawSavedRectFar(s32);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = gMenuWork;
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 44));
    WaitFrames(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 456;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&MsgItemPlainName;
        UiText_DrawCharacterAtOffsetFar(masked, *(s32 *)(menu + 44), 0, 0);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
