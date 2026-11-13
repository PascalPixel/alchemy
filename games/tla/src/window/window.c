/// @file
/// The window object: the windows drawn into a shared BG tilemap, the messages
/// typed into them and the decoder behind them, the sprites windows carry,
/// icons, the selection screen, the settings and Pause menus, the debug menus
/// and text drawn as tiles.
///
/// window_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// window/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours,
/// cut where The Broken Seal's are. debug.c holds the debug menus, which The
/// Lost Age keeps here, between the Pause menu and the text tiles, where The
/// Broken Seal keeps them in its screens object (screens/debug.c). The object
/// ends at Text_DrawPrefixedNumberAtOffset, as The Broken Seal's does. Where to
/// start:
///
///   Windows         windows.c: Window_Create, Window_Close;
///                   borders.c: Window_DrawBorder, Window_UpdateZooms
///   Messages        borders.c: Message_OpenWindow, FieldMessage_Show;
///                   typing.c: Message_UpdateAll; decode.c: Message_Decode
///   Text drawing    sprites.c: WindowSprite_UpdateAll; text_tiles.c:
///                   Text_DrawStringAsTiles, Text_DrawString
///   Screens         selection.c: FieldMenu_Run; debug.c:
///                   DebugWarp_Run

#include "window_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
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
#include "window/settings_menu.c"
#include "window/debug.c"
#include "window/text_tiles.c"
