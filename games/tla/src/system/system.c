/// @file
/// The System object: power-up and the interrupt handlers, the frame wait and
/// VBlank, input, the register write queue, fades and the sprite lists.
///
/// system_internal.h holds what every part shares (its includes); the code
/// is in the files under system/, included here in the order it has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are. Where to start:
///
///   Power-up        start_up.c: Func_0801319c
///   Frames          frame.c: wait(), Func_0801399c (the VBlank handler),
///                   Irq_SetHandler
///   Screen          screen_fade.c: Fade_Start; oam.c: Oam_AddSpriteAtDepth

#include "system_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "system/start_up.c"
#include "system/frame.c"
#include "system/io_write_queue.c"
#include "system/screen_fade.c"
#include "system/oam.c"
