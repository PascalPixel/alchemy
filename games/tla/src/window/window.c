/// @file
/// The window object: the windows drawn into a shared BG tilemap, the messages
/// typed into them and the decoder behind them, the sprites windows carry,
/// icons, the selection screen, the settings and Pause menus, the debug menus
/// and text drawn as tiles. The screens built on them are the next object,
/// screens.c, as in The Broken Seal.
///
/// window_internal.h holds what every part shares (its includes and the
/// declarations the parts disagreed on); the code is in the window_*.c files
/// beside this one, included here in the order it has in the ROM, cut where
/// The Broken Seal's parts are. window_debug.c holds the debug menus, which The
/// Lost Age keeps after the settings and Pause menus, where The Broken Seal
/// keeps them in its screens object. Where to start:
///
///   Windows         window_windows.c: Window_Create, Window_Close;
///                   window_borders.c: Window_DrawBorder, Window_UpdateZooms
///   Messages        window_borders.c: Message_OpenWindow, FieldMessage_Show;
///                   window_typing.c: Message_UpdateAll;
///                   window_decode.c: Message_Decode
///   Text drawing    window_sprites.c: WindowSprite_UpdateAll;
///                   window_text_tiles.c: Text_DrawStringAsTiles,
///                   Text_DrawString
///   Screens         window_selection.c: FieldMenu_Run;
///                   window_debug.c: DebugWarp_Run

#include "window_internal.h"
#include "window/screens.h"

#ifdef MODERN
// The window object, like the screens object, calls Oam_AddSpriteAtDepth with
// no declaration in scope, as its bytes need; a current GCC gets it here.
#include "graphics/window_sprite_list.h"
#endif

// Called before their definitions; the screens object calls them with no
// declaration in scope.
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 whole);
struct Message *Message_Start(struct Window *window, s32 entry, s32 x, s32 y,
                              u16 *spacing, s32 flags);

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
