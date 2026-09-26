/* Complete owner [080a7850, 080a7a34), 484 bytes including interior pools.
   2026-09-26 H1: 482-byte function, all instructions and literals exact;
   score against 484 reports only the final zero alignment halfword absent
   from the function-sized extraction. Reference and candidate frame 28.
   Normal production integration must validate the owner extent before credit.
   Old template baseline: 200/484, 240 differing halfwords, 210 aligned edits.
   The caller is CharacterSelector_RunRearrange. Messages 0xb17 and 0xb18
   are Return and Current Djinn; 0x45f starts the Djinn names. H1 restores
   the pointer-owned cursors, four element positions, both draws per owned
   Djinn, the window refresh flag and the selector's original layout. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"

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
extern u8 Value_00000b17[];
extern u8 Value_00000b18[];
extern u8 Value_00001001[];
extern u8 Value_0000045f[];

void ItemMenu_ResetCategory(void);
void Audio_PlayCue(s32 cue);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, u32 x1, u32 y1, u32 x2, u32 y2);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, u32 mode);
s32 GameFlag_TestFar(s32 flag);

s32 Func_080a7850(void)
{
    struct DjinnListMenu *menu;
    s32 window;
    s32 row;
    s32 col;

    menu = gMenuWork;
    ItemMenu_ResetCategory();
    Audio_PlayCue(112);
    RenderOutput_RedrawSavedRectFar(menu->help_window);
    UiText_DrawCharacterAtOffsetFar((s32)Value_00000b17, menu->help_window, 0, 16);
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
    Func_08015078((s32)Value_00000b18, menu->help_window, -96, 132);
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 7; col++) {
            s32 id = row * 20 + col;

            if (GameFlag_TestFar(48 + id)) {
                UiWindow_SetTilemapEntryFar(window, (s32)Value_00001001 + row,
                    row * 7 + 1, col + 3, 0);
                UiText_DrawCharacterAtOffsetFar((s32)Value_0000045f + id,
                    window, row * 56 + 16, col * 8 + 24);
            }
        }
    }
    gWindowWork[0xea3] = 1;
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
