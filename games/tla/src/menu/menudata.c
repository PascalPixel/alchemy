#include "menu.h"
#include "field.h"
#include "edition.h"
#include "types.h"

//--------------------------------------------------------------------------
// Menu tables: cursor bobbing, list orders, cursor widths and resource ids.
// The menus declare them without const, so they are not const here either.
//--------------------------------------------------------------------------

// The cursor's bob, indexed by (gFrameCount >> 1) & 7
u8 gUiMenu_CursorBobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
u8 gUiMenu_CursorBobY[9] = {0, 0xff, 0xff, 0xfe, 0xfe, 0xff, 0xff, 0, 0}; // 0, -1, -1, -2 ...

// List orders, each ending in 0xff; ⚓️ adds entries 10 and 11
u8 gMenu_ListOrderDefault[13] = {0, 1, 4, 3, 2, 5, 10, 6, 7, 8, 9, 11, 0xff};
u8 gMenu_ListOrderMode2[13] = {1, 4, 3, 2, 5, 10, 6, 7, 8, 9, 11, 0, 0xff};
u8 gMenu_ListOrderMode1[24] =
{
    0x81, 0x84, 0x83, 0x82, 0x85, 0x8a, 0x86, 0x87, 0x88, 0x89, 0x8b, 1,
    4, 3, 2, 5, 10, 6, 7, 8, 9, 11, 0, 0xff,
};
u8 gMenu_ListOrderMode0[24] =
{
    0x81, 0x84, 0x83, 0x82, 0x85, 0x8a, 0x86, 0x87, 0x88, 0x89, 0x8b, 0,
    1, 4, 3, 2, 5, 10, 6, 7, 8, 9, 11, 0xff,
};

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR)
// The item commands' cursor x, three columns a row
s32 gItemMenu_CommandColumnXTable[6] =
{
    86, 117, 166,
    86, 117, 166,
};
#endif

// The command names' widths
#if defined(TLA_EDITION_FR)
u8 gCharacterMenu_CursorWidths[5] = {6, 7, 7, 6, 6};
#else
u8 gCharacterMenu_CursorWidths[5] = {9, 4, 6, 5, 7};
#endif

s32 gFourObjectMotion_ResourceIds[4] = {0xf3, 0xf4, 0xf5, 0xf6};

u16 gData_08105a50[5] = {16, 1, 2, 15, 7};
