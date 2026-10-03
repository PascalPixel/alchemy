#include "RESOURCE.H"
#include "EDITION.H"
#include "DMA.H"
#include "RENDER_INPUT.H"
#include "WORKSPACE_OPTIONS.H"

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiWork_Finalize(struct RenderInput *window, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

void ShopCursor_AdvanceFar(union MenuCursor *cursor);
void Shop_SetCursorFar(union MenuCursor *cursor, s32 x, s32 y, s32 speed);
void ShopCursor_SetPositionImmediateFar(union MenuCursor *cursor, s32 x, s32 y);

extern u8 MsgTalkChoice[];
extern u8 Resource_FixedBlockBTiles[];

extern volatile u32 gKeysRepeat;

/* The Spanish and Japanese choices are shorter: a narrower window further
   right. */
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

/* "Descriptions", "Cheer" or "Nothing": returns the row chosen with A, or -1
   when B cancels. */
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
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, (volatile u32 *)0x040000d4);
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
        if (gKeysRepeat & 0x40) {
            Audio_PlayCue(111);
            row--;
            moved = 1;
            if (row == -1)
                row = 2;
        }
        if (gKeysRepeat & 0x80) {
            Audio_PlayCue(111);
            row++;
            moved = 1;
            if (row == 3)
                row = 0;
        }
        if (gKeysRepeat & 2) {
            Audio_PlayCue(113);
            row = -1;
            goto close;
        }
    } while (!(gKeysRepeat & 1));
    Audio_PlayCue(112);
close:
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(slot);
    return row;
}
