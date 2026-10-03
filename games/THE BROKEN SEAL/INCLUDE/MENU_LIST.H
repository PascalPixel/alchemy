#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

#include "TYPES.H"
#include "TBS_EDITION.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "WINDOW.H"
#include "LAYOUT_GUARD.H"

/* What the paged list screens share: the battle ability, item and summon
   lists, the target and status screens. Each opens a list window and a message
   window, draws one page of rows, queues one sprite per row and a blinking
   cursor, and keeps its page and row in the shared menu cell so that the list
   reopens where it was left. A page holds PAGE_ROWS rows (TBS_EDITION.H). */

/* A queued sprite: the list link Runtime_PushSlotEntry fills, then the three
   hardware attributes as two words or as fields. */
struct MenuSprite {
    struct MenuSprite *next;
    union {
        struct {
            u32 attr01;
            u32 attr23;
        } word;
        struct {
            u16 y : 8;
            u16 affine : 2;
            u16 mode : 2;
            u16 mosaic : 1;
            u16 colors : 1;
            u16 shape : 2;
            u16 x : 9;
            u16 affine_index : 5;
            u16 size : 2;
            u16 tile : 10;
            u16 priority : 2;
            u16 palette : 4;
            u16 unused;
        } f;
    } oam;
};

/* The attribute words of a blank 16x16 sprite. */
#define MENU_SPRITE_SIZE_16 0x40000000

/* A pixel position. As a local it is one two-word value, so it never lives in
   registers across a call. */
struct MenuPoint {
    s32 x;
    s32 y;
};

/* The menu cell behind gWindowWork[42], which the link countdown shares
   (gLinkCountdownWork names the same pointer). A list keeps its position
   here between openings; `active` cleared from outside closes the list. */
struct MenuCell {
    u8 unknown_00[0x28];
    s32 slide_offset;           /* 0x28: target marker slide */
    u8 unknown_2c[4];
    s32 row;                    /* 0x30: row within the page */
    s32 page;                   /* 0x34: index of the page's first entry */
    s32 preferred_row;          /* 0x38: row to return to when paging */
    u8 unknown_3c[8];
    void *work;                 /* 0x44: screen workspace */
    s32 busy;                   /* 0x48: screen redraw in progress */
    s32 active;                 /* 0x4c */
    u8 unknown_50[0x88];
    s32 auto_enabled;           /* 0xd8: automatic target selection */
    s32 auto_delay;             /* 0xdc: frames until automatic input */
};

LAYOUT_OFFSET_GUARD(MenuCell_Slide, struct MenuCell, slide_offset, 0x28);
LAYOUT_OFFSET_GUARD(MenuCell_Row, struct MenuCell, row, 0x30);
LAYOUT_OFFSET_GUARD(MenuCell_Work, struct MenuCell, work, 0x44);
LAYOUT_OFFSET_GUARD(MenuCell_Busy, struct MenuCell, busy, 0x48);
LAYOUT_OFFSET_GUARD(MenuCell_Active, struct MenuCell, active, 0x4c);
LAYOUT_OFFSET_GUARD(MenuCell_AutoEnabled, struct MenuCell, auto_enabled, 0xd8);
LAYOUT_OFFSET_GUARD(MenuCell_AutoDelay, struct MenuCell, auto_delay, 0xdc);

extern struct MenuCell *gLinkCountdownWork;
extern volatile s32 gKeyState;
extern volatile s32 gKeysRepeat;
extern volatile u32 gFrameCount;
extern u8 Resource_FixedBlockBTiles[];
extern const u8 Menu_AnimatedCursorTiles[];

void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Runtime_PushSlotEntry(s32 *entry, s32 slot);
void Resource_ResetEntry(s32 handle);
s32 Resource_LoadIntoFreeSlot(s32 kind);
s32 Resource_GetBuffer(s32 handle, s32 source);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
s32 UiText_CopyMessageString(s32 message, u16 *buffer, u32 count);
void UiText_RenderWideStringAtOffset(
    u16 *buffer, struct TextWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntry(
    struct UiWindow *window, s32 tile, s32 x, s32 y, u32 layer);
void UiWork_SetParamNibble(s32 param);
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 value);
void Ui_FillVramBlockPattern(void);
void Vram_CopyTile(s32 source, s32 destination);
void AudioCommand_PlayFar(s32 cue);

#endif
