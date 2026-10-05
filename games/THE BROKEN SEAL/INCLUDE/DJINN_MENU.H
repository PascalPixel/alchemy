#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

#include "TYPES.H"
#include "RENDER_INPUT.H"

/* A packed Djinn entry: index in bits 0-4, element in bits 5-7, owner in
   bits 8-11 and, for a set Djinni, bit 15. */
#define DJINN_ENTRY_INDEX   31
#define DJINN_ENTRY_ELEMENT 0xe0
#define DJINN_ENTRY_OWNER   0xf00
#define DJINN_ENTRY_IS_SET  0x8000

#define DJINN_ELEMENTS      4
#define DJINN_PER_ELEMENT   20
#define DJINN_LIST_OWNERS   8
#define DJINN_LIST_ROWS     10

/* Every party member's Djinn as packed entries, one row per member. */
struct DjinnListTable {
    u16 ids[DJINN_LIST_OWNERS][DJINN_LIST_ROWS];
    s8 counts[DJINN_LIST_OWNERS];
};

/* The buffer the Djinn screen allocates: the list table, then the BG tiles
   and palettes its backdrop replaced. */
struct DjinnMenuBackdrop {
    struct DjinnListTable list;
    u8 tiles[0x2000];
    u16 palette[64];
    /* Frame counter and phase for the interactive Djinn tutorial. */
    s32 tutorial_timer;
    s32 tutorial_step;
};

/* The Djinn screen's view of the menu work cell. */
struct DjinnMenuWork {
    u8 unknown_000[8];
    /* The member the current step acts for. */
    s32 owner;
    u8 unknown_00c[4];
    s32 status_window;
    struct RenderOutput *cursor;
    u8 unknown_018[4];
    /* First selection and the recipient selected for a give/trade. */
    s8 selected_column[2];
    u8 unknown_01e[6];
    s32 window;
    u8 unknown_028[0x08];
    s32 list_window;
    s32 second_window;
    u8 unknown_038[0xd4];
    s32 help_window;
    u8 unknown_110[0x34];
    u16 frames[8];
    u8 unknown_154[0x20];
    u16 selection[2]; /* column + row * DJINN_LIST_ROWS */
    u16 pair_entries[2];
    struct RenderOutput *second_cursor;
    u8 unknown_180[0x04];
    struct DjinnMenuBackdrop *backdrop;
    u8 unknown_188[0x80];
    u16 owners[DJINN_LIST_OWNERS];
    /* Djinn chosen for the pair step; none sends the screen back. */
    u8 pair_count;
    u8 owner_count;
    u8 shown_owner[2];
    u8 unknown_21c[4];
    /* The pair command waiting for its second choice: bit 0 picks again,
       bit 1 confirms. */
    u16 pending;
    u8 unknown_222[0x12];
    u16 slot_x[4];
    u16 slot_y[4];
    u8 unknown_244[0x10];
    u8 pair_index[2];
    u8 pair_element[2];
    u8 pair_owner[2];
};

LAYOUT_OFFSET_GUARD(DjinnMenuWork_StatusWindow, struct DjinnMenuWork, status_window, 0x10);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_SelectedColumn, struct DjinnMenuWork, selected_column, 0x1c);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Selection, struct DjinnMenuWork, selection, 0x174);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_Pending, struct DjinnMenuWork, pending, 0x220);
LAYOUT_OFFSET_GUARD(DjinnMenuWork_PairIndex, struct DjinnMenuWork, pair_index, 0x254);
LAYOUT_OFFSET_GUARD(DjinnMenuBackdrop_TutorialTimer, struct DjinnMenuBackdrop, tutorial_timer, 0x2128);
LAYOUT_OFFSET_GUARD(DjinnMenuBackdrop_TutorialStep, struct DjinnMenuBackdrop, tutorial_step, 0x212c);

#endif
