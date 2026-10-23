/// @file
/// The field Psynergy and script object: the commands map scripts run (waits,
/// items, questions, destinations, object events, messages and the camera),
/// moving on the field (views, jumps and climbing), the field's weather and
/// scanline effects, Djinn joining the party, each field Psynergy's effect, the
/// field effect sprites its particles use, and the world map screen.
/// field_services_internal.h holds the includes and the macros, types and
/// inline helpers more than one part uses; the code is in the files under
/// field_services/, included here in ROM order, each opening with what it
/// holds (head -2 field_services/*.c). The ROM shows one object; the parts
/// are ours, cut where one family ends and the next begins.
///
/// A field Psynergy reaches its effect through its field kind, the type column
/// of games/tbs/src/game/actions.tsv; dispatch.c runs the kind's handlers.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "field_services_internal.h"

// The object's code, in ROM order. Each part is compiled here, as part of
// this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "field_services/script.c"
#include "field_services/requests.c"
#include "field_services/object_event_commands.c"
#include "field_services/messages.c"
#include "field_services/camera.c"
#include "field_services/view.c"
#include "field_services/object_event_motion.c"
#include "field_services/climbing.c"
#include "field_services/context.c"
#include "field_services/wave_scroll.c"
#include "field_services/weather.c"
#include "field_services/djinni.c"
#include "field_services/jupiter.c"
#include "field_services/mars.c"
#include "field_services/venus.c"
#include "field_services/mercury.c"
#include "field_services/dispatch.c"
#include "field_services/arcs.c"
#include "field_services/setup.c"
#include "field_services/mind_read.c"
#include "field_services/move.c"
#include "field_services/reveal.c"
#include "field_services/growth.c"
#include "field_services/lift.c"
#include "field_services/force.c"
#include "field_services/frost.c"
#include "field_services/ply.c"
#include "field_services/cloak.c"
#include "field_services/douse.c"
#include "field_services/carry.c"
#include "field_services/whirlwind.c"
#include "field_services/halt.c"
#include "field_services/catch.c"
#include "field_services/retreat.c"
#include "field_services/avoid.c"
#include "field_services/effect_slot.c"
#include "field_services/world_map.c"
