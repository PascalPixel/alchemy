/// @file
/// The field object: the sequence parent and scene changes, random encounters,
/// a field scene's actors, the leader's walking and the requests they raise,
/// the scene's triggers, using items and Psynergy on the field, the map's
/// treasure spots, and the screen transitions and palette fade that the field
/// and battles share. overworld_internal.h holds the includes and the few
/// macros and types more than one part uses; the code is in the files under
/// overworld/, included here in ROM order, each opening with what it holds
/// (head -2 overworld/*.c). The ROM shows one object; the parts are ours, cut
/// where one family ends and the next begins.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "overworld_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "overworld/sequence_parent.c"
#include "overworld/encounter.c"
#include "overworld/actors.c"
#include "overworld/walking.c"
#include "overworld/field_sequence.c"
#include "overworld/palette_editor.c"
#include "overworld/triggers.c"
#include "overworld/targets.c"
#include "overworld/items.c"
#include "overworld/psynergy.c"
#include "overworld/markers.c"
#include "overworld/field_fx.c"
#include "overworld/transition.c"
#include "overworld/color.c"
#include "overworld/fade_overlay.c"
#include "overworld/speakers.c"
