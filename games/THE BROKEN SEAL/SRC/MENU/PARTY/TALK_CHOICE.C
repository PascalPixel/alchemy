#include "DMA.H"
#include "RENDER_INPUT.H"
#include "WORKSPACE_OPTIONS.H"

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiWork_Finalize(struct RenderInput *window, s32 mode);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Resource_ResetEntry(s32 slot);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

void ShopCursor_AdvanceFar(union MenuCursor *cursor);
void Shop_SetCursorFar(union MenuCursor *cursor, s32 x, s32 y, s32 speed);
void ShopCursor_SetPositionImmediateFar(union MenuCursor *cursor, s32 x, s32 y);

extern u8 MsgTalkChoice[];
extern u8 Resource_FixedBlockBTiles[];

extern volatile u32 gKeysRepeat;

/* FAKEMATCH: the shared MenuCursor union preserves pointer-store ordering. */
/* "Descriptions", "Cheer" or "Nothing": returns the row chosen with A, or -1
   when B cancels. */
s32 PartyTalkMenu_Choose(void)
{
    struct RenderInput *window;
    union MenuCursor cursor;
    s32 slot;
    s32 row;
    s32 moved;
    u16 *palette;

    moved = 1;
    window = UiWindow_Create(7, 13, 18, 7, 2);
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
        UiWindow_SetTileAttributeRect(window, 1, row * 2, 14, 1, 14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window, 1, row * 2, 14, 1, 15);
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
