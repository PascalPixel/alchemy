/// @file
/// The window object: the UI windows drawn into a shared BG tilemap, the
/// message channels that type text into them, the message decoder and the
/// formatting behind it, the sprites (render outputs) that windows carry,
/// icons, the selection screen and the option and system menus.
///
/// Everything lives in one work block, struct WindowWork (heap slot
/// HEAP_SLOT_WINDOW, reached as WINDOW_WORK): the tilemap, the window slots,
/// three message channels, a pool of 64 render outputs and the decoded text.
///
/// window_internal.h holds what every part shares (includes, edition values
/// and macros); the code is in the files under window/, included here in the
/// order it has in the ROM. Camelot's own files are not recoverable: the ROM
/// shows one object, and these parts are ours, cut where one family ends and
/// the next begins. README.md in this folder gives the words, prefixes and
/// spellings the parts use. Where to start:
///
///   Windows         windows.c: Window_Create, Window_Close;
///                   frames.c: Window_DrawFrame, UiWork_ProcessDirectWork
///   Messages        frames.c: Message_OpenWindow,
///                   FieldMessage_Show; typing.c:
///                   TextPrinter_Step; decode.c:
///                   Message_Decode
///   Text drawing    sprites.c: Glyph_Draw; text_tiles.c:
///                   Text_DrawStringAsTiles, Text_DrawString
///   Screens         selection.c: SelectionScreen_Run, FieldMenu_Run;
///                   options.c: OptionMenu_Run,
///                   PauseMenu_Choose

#include "window_internal.h"

// The window object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "window/windows.c"
#include "window/typing.c"
#include "window/frames.c"
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
