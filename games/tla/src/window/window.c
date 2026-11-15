/// @file
/// The window object: the windows drawn into a shared BG tilemap, the messages
/// typed into them and the decoder behind them, the sprites windows carry,
/// icons, the selection screen, the settings and Pause menus, the debug menus
/// and text drawn as tiles. The screens built on them are the next object,
/// screens.c, as in The Broken Seal.
///
/// window_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the files under
/// window/, included here in the order it has in the ROM. Camelot's own files
/// are not recoverable: these parts are ours, cut where The Broken Seal's are.
/// debug.c holds the debug menus, which The Lost Age keeps after the settings
/// and Pause menus, where The Broken Seal keeps them in its screens object
/// (screens/debug.c). Where to start:
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
#include "graphics/output.h"

// Called before their definitions; the screens object calls them with no
// declaration in scope.
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 whole);
struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags);

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
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
#include "window_debug.c"
#include "window_text_tiles.c"
