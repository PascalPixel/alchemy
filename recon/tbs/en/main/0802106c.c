/* Draft, not exact: 156 differing halfwords, 436-byte candidate for the
   444-byte owner (2026-09-23). Residual: loop structure, B-cancel placement
   and palette pointer match; the key-state address is not hoisted into a
   register for the loop as in the reference (which spills the resource slot
   to the stack), so registers r9-r11 and the slot differ. A local key
   pointer spills the slot but permutes r9-r11. */

#include "DMA.H"
#include "RENDER_INPUT.H"

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiWork_Finalize(struct RenderInput *window, s32 mode);
s32 find_free_slot(void);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void Resource_ResetEntry(s32 slot);
s32 WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

/* A pointing-hand sprite that glides between menu rows. */
struct UiCursor {
    struct RenderOutput *output;
    u8 unknown_04[12];
};

void Func_080b0020(struct UiCursor *cursor);
void Func_080b0030(struct UiCursor *cursor, s32 x, s32 y, s32 speed);
void Func_080b0038(struct UiCursor *cursor, s32 x, s32 y);

extern u8 Data_00002080[];
extern u8 Data_080310a4[];

#define KEYS_PRESSED (*(volatile u32 *)0x03001b04)

/* "Descriptions", "Cheer" or "Nothing": returns the row chosen with A, or -1
   when B cancels. */
s32 PartyTalkMenu_Choose(void)
{
    struct RenderInput *window;
    struct UiCursor cursor;
    s32 slot;
    s32 row;
    s32 moved;
    u16 *palette;

    moved = 1;
    window = UiWindow_Create(7, 13, 18, 7, 2);
    UiText_DrawCharacterAtOffset((s32)Data_00002080, window, 8, 0);
    UiText_DrawCharacterAtOffset((s32)Data_00002080 + 1, window, 8, 16);
    UiText_DrawCharacterAtOffset((s32)Data_00002080 + 2, window, 8, 32);
    slot = find_free_slot();
    row = 0;
    if (slot < 96) {
        VramBlock_LoadCached(slot, 128, Data_080310a4);
        cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
        Func_080b0038(&cursor, window->x * 8 - 3, window->y * 8 + 9);
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
            Func_080b0030(&cursor, window->x * 8 - 3, (window->y + row * 2) * 8 + 9, 3);
        }
        Func_080b0020(&cursor);
        if (KEYS_PRESSED & 0x40) {
            Audio_PlayCue(111);
            row--;
            moved = 1;
            if (row == -1)
                row = 2;
        }
        if (KEYS_PRESSED & 0x80) {
            Audio_PlayCue(111);
            row++;
            moved = 1;
            if (row == 3)
                row = 0;
        }
        if (KEYS_PRESSED & 2) {
            Audio_PlayCue(113);
            row = -1;
            goto close;
        }
    } while (!(KEYS_PRESSED & 1));
    Audio_PlayCue(112);
close:
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(slot);
    return row;
}
