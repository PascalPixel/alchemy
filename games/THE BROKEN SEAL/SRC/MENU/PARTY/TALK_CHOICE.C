#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "RESOURCE.H"
#include "EDITION.H"
#include "DMA.H"
#include "RENDER_INPUT.H"
#include "WORKSPACE_OPTIONS.H"
#include "UIFLOW.H"
#include "text/MSG_IDS.H"

#define ShopCursor_AdvanceFar ((void (*)(union MenuCursor*))ShopCursor_AdvanceFar)
#define Shop_SetCursorFar ((void (*)(union MenuCursor*,s32,s32,s32))Shop_SetCursorFar)
#define ShopCursor_SetPositionImmediateFar ((void (*)(union MenuCursor*,s32,s32))ShopCursor_SetPositionImmediateFar)

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window, s32 x, s32 y);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);


extern u8 Resource_FixedBlockBTiles[];

extern volatile u32 gKeysRepeat;

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define TALK_WINDOW_X     7
#define TALK_WINDOW_WIDTH 18
#elif defined(TBS_EDITION_ES)
#define TALK_WINDOW_X     9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X     8
#define TALK_WINDOW_WIDTH 16
#endif

s32 PartyTalkMenu_Choose(void)
{
    /* FAKEMATCH: the shared cursor union preserves pointer-store ordering; an equal-size struct changes stack-address construction in all six editions. */
    struct RenderInput *window;
    union MenuCursor cursor;
    s32 slot;
    s32 row;
    s32 moved;
    u16 *palette;

    moved = 1;
    window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
    UiText_DrawCharacterAtOffset((s32)MsgTalkChoice, window, 8, 0);
    UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 1, window, 8, 16);
    UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 2, window, 8, 32);
    slot = Resource_FindFreeEntry();
    row = 0;
    if (slot < 96) {
        VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
        cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 - 3, window->y * 8 + 9);
    }
    palette = (u16 *)0x050001c0;
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
    palette[4] = 0x6318;
    do {
        UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 15);
        if (moved) {
            moved = 0;
            Shop_SetCursorFar(&cursor, window->x * 8 - 3, (window->y + row * 2) * 8 + 9, 3);
        }
        ShopCursor_AdvanceFar(&cursor);
        if (gKeysRepeat & KEY_UP) {
            Audio_PlayCue(111);
            row--;
            moved = 1;
            if (row == -1)
                row = 2;
        }
        if (gKeysRepeat & KEY_DOWN) {
            Audio_PlayCue(111);
            row++;
            moved = 1;
            if (row == 3)
                row = 0;
        }
        if (gKeysRepeat & KEY_B) {
            Audio_PlayCue(113);
            row = -1;
            goto close;
        }
    } while (!(gKeysRepeat & KEY_A));
    Audio_PlayCue(112);
close:
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(slot);
    return row;
}
