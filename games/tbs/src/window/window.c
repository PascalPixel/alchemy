// The window object: the UI windows drawn into a shared BG tilemap, the messages that type text
// into them, the message decoder and the formatting behind it, the sprites (window sprites) that
// windows carry, icons, the selection screen and the option and pause menus.
//
// Everything lives in one state block, WIN_STATE (heap slot HEAP_SLOT_WINDOW, reached as
// WINDOW_STATE): the tilemap, the window slots, three messages, a pool of 64 window sprites and the
// decoded text.
//
// window_internal.h holds what every part shares (includes, edition values and macros); the code is
// in the files under window/, included here in link order, one group to a part. README.md in this
// folder gives the words, prefixes and spellings the parts use. Where to start:
//
//       Windows         window_windows.c: mnWindowCreate, mnWindowClose;
//                                       window_borders.c: mnWindowDrawBorder,
// mnWindowUpdateZooms Messages        window_borders.c: mnMessageOpenWindow,
//                                       mnFieldMessageShow; window_typing.c:
//                                       mnMessageType; window_decode.c:
//                                       mnMessageDecode
//       Text drawing    window_sprites.c: mnGlyphDraw; window_text_tiles.c:
//                                       mnTextDrawStringAsTiles,
// mnTextDrawString Screens         window_selection.c: mnSelectionMenuRun,
// mnFieldMenuRun; window_settings_menu.c: mnSettingsMenuRun, mnPauseMenuChoose

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
