#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

#include "TYPES.H"

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
};

struct DjinnMenuCursor {
    u8 unknown_00[5];
    u8 state;
};

/* The Djinn screen's view of the menu work cell. */
struct DjinnMenuWork {
    u8 unknown_000[0x14];
    struct DjinnMenuCursor *cursor;
    u8 unknown_018[0x0c];
    s32 window;
    u8 unknown_028[0x08];
    s32 list_window;
    s32 second_window;
    u8 unknown_038[0xd4];
    s32 help_window;
    u8 unknown_110[0x34];
    u16 frames[8];
    u8 unknown_154[0x24];
    u16 pair_entries[2];
    struct DjinnMenuCursor *second_cursor;
    u8 unknown_180[0x04];
    struct DjinnMenuBackdrop *backdrop;
    u8 unknown_188[0x80];
    u16 owners[DJINN_LIST_OWNERS];
    u8 unknown_218;
    u8 owner_count;
    u8 shown_owner[2];
    u8 unknown_21c[0x18];
    u16 slot_x[4];
    u16 slot_y[4];
    u8 unknown_244[0x10];
    u8 pair_index[2];
    u8 pair_element[2];
    u8 pair_owner[2];
};

#endif
