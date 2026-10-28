/// @file
/// The window object: the UI windows drawn into a shared BG tilemap, the
/// text printers that type text into them, the message decoder and the
/// formatting behind it, the sprites (window sprites) that windows carry,
/// icons, the selection screen and the option and pause menus.
///
/// Everything lives in one work block, struct WindowWork (heap slot
/// HEAP_SLOT_WINDOW, reached as WINDOW_WORK): the tilemap, the window slots,
/// three text printers, a pool of 64 window sprites and the decoded text.
///
/// window_internal.h holds what every part shares (includes, edition values
/// and macros); the code is in the files under window/, included here in the
/// order it has in the ROM. Camelot's own files are not recoverable: the ROM
/// shows one object, and these parts are ours, cut where one family ends and
/// the next begins. README.md in this folder gives the words, prefixes and
/// spellings the parts use. Where to start:
///
///   Windows         windows.c: Window_Create, Window_Close;
///                   borders.c: Window_DrawBorder, Window_UpdateZooms
///   Messages        borders.c: Message_OpenWindow,
///                   FieldMessage_Show; typing.c:
///                   TextPrinter_Step; decode.c:
///                   Message_Decode
///   Text drawing    sprites.c: Glyph_Draw; text_tiles.c:
///                   Text_DrawStringAsTiles, Text_DrawString
///   Screens         selection.c: SelectionMenu_Run, FieldMenu_Run;
///                   options.c: SettingsMenu_Run,
///                   PauseMenu_Choose

#include "window_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "window/windows.c"
#include "window/typing.c"
#include "window/borders.c"
#include "window/glyphs.c"
#include "window/decode.c"
#include "window/measure.c"
#include "window/sprites.c"
#include "window/messages.c"
#include "window/icons.c"
#include "window/selection.c"
#include "window/shortcuts.c"
#include "window/options.c"
#include "window/text_tiles.c"
