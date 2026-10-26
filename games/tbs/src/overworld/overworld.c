/// @file
/// The field object: the main loop and map changes, random encounters, a field
/// map's object events, the leader's steps and the requests they raise, the
/// map's events, using items and Psynergy on the field, the map's treasure
/// spots, and the screen transitions and palette fade that the field and
/// battles share. overworld_internal.h holds the includes and the few macros
/// and types more than one part uses; the code is in the files under
/// overworld/, included here in ROM order, each opening with what it holds
/// (head -2 overworld/*.c). The ROM shows one object; the parts are ours, cut
/// where one family ends and the next begins.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "overworld_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "overworld/map.c"
#include "overworld/encounter.c"
#include "overworld/object_events.c"
#include "overworld/step.c"
#include "overworld/run_map.c"
#include "overworld/palette_editor.c"
#include "overworld/events.c"
#include "overworld/targets.c"
#include "overworld/items.c"
#include "overworld/psynergy.c"
#include "overworld/markers.c"
#include "overworld/field_fx.c"
#include "overworld/transition.c"
#include "overworld/color.c"
#include "overworld/fade_overlay.c"
#include "overworld/speakers.c"
