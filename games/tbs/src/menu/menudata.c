#include "menu.h"
#include "field.h"
#include "edition.h"
#include "types.h"

//--------------------------------------------------------------------------
// メニューの表
// Menu tables: cursor bobbing, list orders, cursor widths and resource ids.
//--------------------------------------------------------------------------

// 読む側が const なしで持つので const にしない。

// カーソルの揺れ ((gFrameCount >> 1) & 7)
u8 gUiMenu_CursorBobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
u8 gUiMenu_CursorBobY[9] = {0,    0xff, 0xff, 0xfe, 0xfe,
                            0xff, 0xff, 0,    0}; // 0, -1, -1, -2 ...

// 一覧の並び (0xff で終わり)
u8 gMenu_ListOrderDefault[11] = {0x00, 0x01, 0x04, 0x03, 0x02, 0x05,
                                 0x06, 0x07, 0x08, 0x09, 0xff};
u8 gMenu_ListOrderMode2[11] = {0x01, 0x04, 0x03, 0x02, 0x05, 0x06,
                               0x07, 0x08, 0x09, 0x00, 0xff};
// clang-format off
u8 gMenu_ListOrderMode1[20] = {
    0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x01,
    0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0xff,
};
u8 gMenu_ListOrderMode0[20] = {
    0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x00,
    0x01, 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff,
};

#if EDITION_INTERNATIONAL
// 道具の命令のカーソルの x (行ごとに 3 列)
s32 gItemMenu_CommandColumnXTable[6] = {
    86, 117, 166,
    86, 117, 166,
};
// clang-format on
#endif

// 命令名の幅
#if defined(TBS_EDITION_JA)
u8 gCharacterMenu_CursorWidths[5] = {9, 4, 6, 5, 7};
#elif defined(TBS_EDITION_DE)
u8 gCharacterMenu_CursorWidths[5] = {7, 5, 7, 6, 5};
#else
u8 gCharacterMenu_CursorWidths[5] = {6, 7, 7, 6, 6};
#endif

s32 gFourObjectMotion_ResourceIds[4] = {0x5a, 0x5b, 0x5c, 0x5d};
