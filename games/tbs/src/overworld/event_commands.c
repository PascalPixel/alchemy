/// @file
/// The field Psynergy and script object: the commands map scripts run (waits,
/// items, questions, destinations, actors, messages and the camera),
/// moving on the field (views, jumps and climbing), the field's weather and
/// scanline effects, Djinn joining the party, each field Psynergy's effect, the
/// field effect sprites its particles use, and the world map screen.
/// event_commands_internal.h holds the includes and the macros, types and
/// inline helpers more than one part uses; the code is in the files under
/// event_commands/, included here in ROM order, each opening with what it
/// holds (head -2 event_commands/*.c). The ROM shows one object; the parts
/// are ours, cut where one family ends and the next begins.
///
/// A field Psynergy reaches its effect through its field kind, the type column
/// of games/tbs/src/game/abilities.tsv; dispatch.c runs the kind's handlers.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "event_commands_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "event_commands/script.c"
#include "event_commands/requests.c"
#include "event_commands/actor_commands.c"
#include "event_commands/messages.c"
#include "event_commands/camera.c"
#include "event_commands/view.c"
#include "event_commands/actor_motion.c"
#include "event_commands/climbing.c"
#include "event_commands/context.c"
#include "event_commands/wave_scroll.c"
#include "event_commands/weather.c"
#include "event_commands/djinni.c"
#include "event_commands/jupiter.c"
#include "event_commands/mars.c"
#include "event_commands/venus.c"
#include "event_commands/mercury.c"
#include "event_commands/dispatch.c"
#include "event_commands/arcs.c"
#include "event_commands/setup.c"
#include "event_commands/mind_read.c"
#include "event_commands/move.c"
#include "event_commands/reveal.c"
#include "event_commands/growth.c"
#include "event_commands/lift.c"
#include "event_commands/force.c"
#include "event_commands/frost.c"
#include "event_commands/ply.c"
#include "event_commands/cloak.c"
#include "event_commands/douse.c"
#include "event_commands/carry.c"
#include "event_commands/whirlwind.c"
#include "event_commands/halt.c"
#include "event_commands/catch.c"
#include "event_commands/retreat.c"
#include "event_commands/avoid.c"
#include "event_commands/effect_slot.c"
#include "event_commands/world_map.c"
