/// @file
/// The field Psynergy and event command object: the commands events run (waits,
/// items, questions, destinations, actors, messages and the camera),
/// moving on the field (views, jumps and climbing), the field's weather and
/// line scrolls, Djinn joining the party, each field Psynergy, the
/// field effect sprites its particles use, and the World Map screen.
/// event_commands_internal.h holds the includes and the macros, types and
/// inline helpers more than one part uses; the code is in the files under
/// event_commands/, included here in link order, one family to a part, each
/// opening with what it holds (head -2 event_commands/*.c).
///
/// A field Psynergy reaches its code through its field kind, the type column
/// of games/tbs/src/game/ability_table.c; event_commands_field_psynergy.c runs
/// the kind's handlers.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "event_commands_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "event_commands_event.c"
#include "event_commands_requests.c"
#include "event_commands_actor_commands.c"
#include "event_commands_messages.c"
#include "event_commands_camera.c"
#include "event_commands_view.c"
#include "event_commands_movement_types.c"
#include "event_commands_climbing.c"
#include "event_commands_exit_animations.c"
#include "event_commands_line_scroll.c"
#include "event_commands_weather.c"
#include "event_commands_djinni.c"
#include "event_commands_jupiter.c"
#include "event_commands_mars.c"
#include "event_commands_venus.c"
#include "event_commands_mercury.c"
#include "event_commands_field_psynergy.c"
#include "event_commands_arcs.c"
#include "event_commands_field_psynergy_work.c"
#include "event_commands_mind_read.c"
#include "event_commands_move.c"
#include "event_commands_reveal.c"
#include "event_commands_growth.c"
#include "event_commands_lift.c"
#include "event_commands_force.c"
#include "event_commands_frost.c"
#include "event_commands_ply.c"
#include "event_commands_cloak.c"
#include "event_commands_douse.c"
#include "event_commands_carry.c"
#include "event_commands_whirlwind.c"
#include "event_commands_halt.c"
#include "event_commands_catch.c"
#include "event_commands_retreat.c"
#include "event_commands_avoid.c"
#include "event_commands_field_particles.c"
#include "event_commands_world_picture.c"
