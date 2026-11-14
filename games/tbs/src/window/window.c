/// @file
/// The window object: the UI windows drawn into a shared BG tilemap, the
/// messages that type text into them, the message decoder and the
/// formatting behind it, the sprites (window sprites) that windows carry,
/// icons, the selection screen and the option and pause menus.
///
/// Everything lives in one work block, struct WindowWork (heap slot
/// HEAP_SLOT_WINDOW, reached as WINDOW_WORK): the tilemap, the window slots,
/// three messages, a pool of 64 window sprites and the decoded text.
///
/// window_internal.h holds what every part shares (includes, edition values
/// and macros); the code is in the files under window/, included here in link
/// order, one family to a part. README.md in this folder gives the words,
/// prefixes and spellings the parts use. Where to start:
///
///   Windows         window_windows.c: Window_Create, Window_Close;
///                   window_borders.c: Window_DrawBorder, Window_UpdateZooms
///   Messages        window_borders.c: Message_OpenWindow,
///                   FieldMessage_Show; window_typing.c:
///                   Message_Type; window_decode.c:
///                   Message_Decode
///   Text drawing    window_sprites.c: Glyph_Draw; window_text_tiles.c:
///                   Text_DrawStringAsTiles, Text_DrawString
///   Screens         window_selection.c: SelectionMenu_Run, FieldMenu_Run;
///                   window_settings_menu.c: SettingsMenu_Run,
///                   PauseMenu_Choose

#include "window_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "window_windows.c"
#include "window_typing.c"
#include "window_borders.c"
#include "window_glyphs.c"
#include "window_decode.c"
#include "window_measure.c"
#include "window_sprites.c"
#include "window_messages.c"
#include "window_icons.c"
#include "window_selection.c"
#include "window_shortcuts.c"
#include "window_settings_menu.c"
#include "window_text_tiles.c"
