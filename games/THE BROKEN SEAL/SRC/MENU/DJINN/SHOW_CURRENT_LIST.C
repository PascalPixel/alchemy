/* Show the acquired Djinn of each element, then restore the party selector. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"

struct DjinnListCursor {
    u8 unknown_00[5];
    u8 state;
};

struct DjinnListMenu {
    u8 unknown_000[0x14];
    struct DjinnListCursor *cursor;
    u8 unknown_018[0x0c];
    s32 window;
    u8 unknown_028[0xe4];
    s32 help_window;
    u8 unknown_110[0x34];
    u16 frames[8];
    u8 unknown_154[0x28];
    struct DjinnListCursor *second_cursor;
    u8 unknown_180[0xb4];
    u16 slot_x[4];
    u16 slot_y[4];
};

extern struct DjinnListMenu *gMenuWork;
extern u8 *gWindowWork;
extern volatile u32 gKeyState;
extern u8 MsgReturnHelp[];
extern u8 MsgCurrentDjinn[];
extern u8 MsgDjinnName[];

void ItemMenu_ResetCategory(void);
void Audio_PlayCue(s32 cue);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, u32 x1, u32 y1, u32 x2, u32 y2);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, u32 mode);
s32 GameFlag_TestFar(s32 flag);

s32 DjinnMenu_ShowCurrentList(void)
{
    struct DjinnListMenu *menu;
    s32 window;
    s32 row;
    s32 col;

    menu = gMenuWork;
    ItemMenu_ResetCategory();
    Audio_PlayCue(112);
    RenderOutput_RedrawSavedRectFar(menu->help_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgReturnHelp, menu->help_window, 0, 16);
    menu->cursor->state = 13;
    menu->second_cursor->state = 13;
    WaitFrames(1);
    window = menu->window;
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 32 + i * 56;
            menu->slot_y[i] = 70;
            menu->frames[i] = 30;
        }
    }
    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
    Func_08015078((s32)MsgCurrentDjinn, menu->help_window, -96, 132);
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 7; col++) {
            s32 id = row * 20 + col;

            if (GameFlag_TestFar(48 + id)) {
                /* Tilemap entry: tile 1 + row in palette 1. */
                UiWindow_SetTilemapEntryFar(window, 0x1001 + row,
                    row * 7 + 1, col + 3, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName + id,
                    window, row * 56 + 16, col * 8 + 24);
            }
        }
    }
    gWindowWork[RENDER_DIRTY_OFS] = 1;
    for (;;) {
        if (GameFlag_TestFar(0x150))
            break;
        WaitFrames(1);
        if (gKeyState & 7)
            break;
    }
    RenderOutput_RedrawSavedRectFar(menu->window);
    RenderOutput_ClearListFar(menu->help_window);
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 130 + i * 32;
            menu->slot_y[i] = 128;
        }
    }
    menu->cursor->state = 1;
    menu->second_cursor->state = 1;
    Audio_PlayCue(113);
    /* FAKEMATCH: the caller ignores the result, but the reference retains
       a value-returning epilogue after this void audio call. */
}
