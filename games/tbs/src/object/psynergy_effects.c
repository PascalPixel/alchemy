/// @file
/// The field Psynergy and event object: the commands scene scripts run (waits,
/// items, questions, destinations, actors, messages and the camera), moving
/// on the field (views, jumps and climbing), the field's weather and wavy
/// backgrounds, Djinn joining the party, each field Psynergy's effect, the
/// effect slots its particles use, and the world map screen.
/// psynergy_effects_internal.h holds the includes and the macros, types and
/// inline helpers more than one part uses; the code is in the files under
/// psynergy_effects/, included here in ROM order, each opening with what it
/// holds (head -2 psynergy_effects/*.c). The ROM shows one object; the parts
/// are ours, cut where one family ends and the next begins.
///
/// A field Psynergy reaches its effect through its field kind, the type column
/// of games/tbs/src/game/actions.tsv; dispatch.c runs the kind's handlers.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "psynergy_effects_internal.h"

// The object's code, in ROM order. Each part is compiled here, as part of
// this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "psynergy_effects/events.c"
#include "psynergy_effects/requests.c"
#include "psynergy_effects/actor_commands.c"
#include "psynergy_effects/messages.c"
#include "psynergy_effects/camera.c"
#include "psynergy_effects/view.c"
#include "psynergy_effects/actor_motion.c"
#include "psynergy_effects/climbing.c"
#include "psynergy_effects/context.c"
#include "psynergy_effects/wave_scroll.c"
#include "psynergy_effects/weather.c"
#include "psynergy_effects/djinni.c"
#include "psynergy_effects/jupiter.c"
#include "psynergy_effects/mars.c"
#include "psynergy_effects/venus.c"
#include "psynergy_effects/mercury.c"
#include "psynergy_effects/dispatch.c"
#include "psynergy_effects/arcs.c"
#include "psynergy_effects/setup.c"
#include "psynergy_effects/mind_read.c"
#include "psynergy_effects/move.c"
#include "psynergy_effects/reveal.c"
#include "psynergy_effects/growth.c"
#include "psynergy_effects/lift.c"
#include "psynergy_effects/force.c"
#include "psynergy_effects/frost.c"
#include "psynergy_effects/ply.c"
#include "psynergy_effects/cloak.c"
#include "psynergy_effects/douse.c"
#include "psynergy_effects/carry.c"
#include "psynergy_effects/whirlwind.c"
#include "psynergy_effects/halt.c"
#include "psynergy_effects/catch.c"
#include "psynergy_effects/retreat.c"
#include "psynergy_effects/avoid.c"
#include "psynergy_effects/effect_slot.c"
#include "psynergy_effects/world_map.c"
