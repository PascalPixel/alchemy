/* NONMATCHING: 2026-10-01 equal-size menu cursor struct attempt.
 * PartyTalkMenu_Choose: add r3, sp, #12 becomes mov r3, #12 (218/219 assembly lines in EN).
 * All six editions change; the production union preserves the measured order.
 * Existing approved TBS options; complete function and literal pool remain to match.
 */
#include "DMA.H"
#include "RENDER_INPUT.H"
#ifndef ALCHEMY_WORKSPACE_OPTIONS_H
#define ALCHEMY_WORKSPACE_OPTIONS_H

#include "TYPES.H"
#include "RENDER_INPUT.H"

/* A menu cursor record (object pointer and cursor state). The union keeps
   pointer stores ordered against the window reads that follow them;
   the two affected callers record their measured steering reason. */
struct MenuCursor {
    struct RenderOutput *output;
    u8 data[0xc];
};

/* The options page of the workspace: five settings, each a value in
   0..count-1, and the menu objects that show them. */
struct WorkspaceWork {
    u8 unknown_000[0x574];
    u16 page;
    u8 unknown_576[8];
    u16 preset;
    u8 unknown_580[0x14];
    s8 option[5];
    s8 option_count[5];
    u8 unknown_59e[6];
    struct MenuCursor cursor;
    struct MenuCursor marker[2];
    u8 unknown_5d4[0x18];
    struct RenderOutput *frame[3][3];
};

extern struct WorkspaceWork *gSelectionWork;

struct RenderInput *Menu_OpenWorkspaceOptions(void);

#endif


struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiWork_Finalize(struct RenderInput *window, s32 mode);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Resource_ResetEntry(s32 slot);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

void ShopCursor_AdvanceFar(struct MenuCursor *cursor);
void Shop_SetCursorFar(struct MenuCursor *cursor, s32 x, s32 y, s32 speed);
void ShopCursor_SetPositionImmediateFar(struct MenuCursor *cursor, s32 x, s32 y);

extern u8 MsgTalkChoice[];
extern u8 Resource_FixedBlockBTiles[];

extern volatile u32 gKeysRepeat;

/* The Spanish and Japanese choices are shorter: a narrower window further
   right. */
#if defined(TBS_EDITION_ES)
#define TALK_WINDOW_X     9
#define TALK_WINDOW_WIDTH 12
#elif defined(TBS_EDITION_JA)
#define TALK_WINDOW_X     8
#define TALK_WINDOW_WIDTH 16
#else
#define TALK_WINDOW_X     7
#define TALK_WINDOW_WIDTH 18
#endif

/* "Descriptions", "Cheer" or "Nothing": returns the row chosen with A, or -1
   when B cancels. */
s32 PartyTalkMenu_Choose(void)
{
    
    struct RenderInput *window;
    struct MenuCursor cursor;
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
