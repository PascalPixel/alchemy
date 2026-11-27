// The window object: the windows drawn into a shared BG tilemap, the messages typed into them and
// the decoder behind them, the sprites windows carry, icons, the selection screen, the settings and
// Pause menus, the debug menus and text drawn as tiles. The screens built on them are the next
// object, screens.c, as in The Broken Seal.
//
// window_internal.h holds what every part shares (its includes and the declarations the parts
// disagreed on); the code is in the window_*.c files beside this one, included here in the order it
// has in the ROM, cut where The Broken Seal's parts are. window_debug.c holds the debug menus,
// which The Lost Age keeps after the settings and Pause menus, where The Broken Seal keeps them in
// its screens object. Where to start:
//
// - windows: mnWindowCreate and mnWindowClose (window_windows.c), mnWindowDrawBorder and
//   mnWindowUpdateZooms (window_borders.c);
// - messages: mnMessageOpenWindow and mnFieldMessageShow (window_borders.c), mnMessageUpdateAll
//   (window_typing.c), mnMessageDecode (window_decode.c);
// - text drawing: mnWindowSpriteUpdateAll (window_sprites.c), mnTextDrawStringAsTiles and
//   mnTextDrawString (window_text_tiles.c);
// - screens: mnFieldMenuRun (window_selection.c), mnDebugWarpRun (window_debug.c).

#include "window_internal.h"

// Called before their definitions; the screens object calls them with no declaration in scope.
void mnWindowMapTextCanvasTiles(Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 whole);
MESSAGE *
mnMessageStart(WINDOW *window, Sint32 entry, Sint32 x, Sint32 y, Uint16 *spacing, Sint32 flags);

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and parts").
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
