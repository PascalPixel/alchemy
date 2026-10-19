/// @file
/// The field object: the main loop and scene changes, random encounters, a
/// field scene's actors, the leader's steps and the requests they raise, the
/// scene's events, using items and Psynergy on the field, the map's markers,
/// and the window transitions and colour buffer that the field and battles
/// share. object_internal.h holds the includes and the few macros and types
/// more than one part uses; the code is in the files under object/, included
/// here in ROM order, each opening with what it holds (head -2 object/*.c).
/// The ROM shows one object; the parts are ours, cut where one family ends and
/// the next begins.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "object_internal.h"

// The object's code, in ROM order. Each part is compiled here, as part of
// this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "object/scene.c"
#include "object/encounter.c"
#include "object/actors.c"
#include "object/step.c"
#include "object/run_scene.c"
#include "object/palette_editor.c"
#include "object/events.c"
#include "object/targets.c"
#include "object/items.c"
#include "object/psynergy.c"
#include "object/markers.c"
#include "object/field_fx.c"
#include "object/transition.c"
#include "object/color.c"
#include "object/fade_overlay.c"
#include "object/speakers.c"
