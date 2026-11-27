/* The field Psynergy and script command object: the commands scripts run
   (waits, items, questions, destinations, actors, messages and the camera),
   moving on the field (views, jumps and climbing), the field's weather and line
   scrolls, Djinn joining the party, each field Psynergy, the field effect
   sprites its particles use, and the map screen. script_commands_internal.h
   holds the includes and the macros, types and inline helpers more than one
   part uses; the code is in the script_commands_*.c files beside it, included
   here in link order, one group to a part, each opening with what it holds.

   A field Psynergy reaches its code through its field kind, the type column of
   games/tbs/src/game/ability_table.c; script_commands_field_psynergy.c runs the
   kind's handlers.

   README.md in this folder gives the words, prefixes and spellings the parts
   share. */

#include "script_commands_internal.h"

/* The parts, in ROM order (src/README.md, "Objects and parts"). */
#include "script_commands_script.c"
#include "script_commands_requests.c"
#include "script_commands_actor_commands.c"
#include "script_commands_messages.c"
#include "script_commands_camera.c"
#include "script_commands_view.c"
#include "script_commands_movement_types.c"
#include "script_commands_climbing.c"
#include "script_commands_exit_animations.c"
#include "script_commands_line_scroll.c"
#include "script_commands_weather.c"
#include "script_commands_djinni.c"
#include "script_commands_jupiter.c"
#include "script_commands_mars.c"
#include "script_commands_venus.c"
#include "script_commands_mercury.c"
#include "script_commands_field_psynergy.c"
#include "script_commands_arcs.c"
#include "script_commands_field_psynergy_state.c"
#include "script_commands_mind_read.c"
#include "script_commands_move.c"
#include "script_commands_reveal.c"
#include "script_commands_growth.c"
#include "script_commands_lift.c"
#include "script_commands_force.c"
#include "script_commands_frost.c"
#include "script_commands_ply.c"
#include "script_commands_cloak.c"
#include "script_commands_douse.c"
#include "script_commands_carry.c"
#include "script_commands_whirlwind.c"
#include "script_commands_halt.c"
#include "script_commands_catch.c"
#include "script_commands_retreat.c"
#include "script_commands_avoid.c"
#include "script_commands_field_particles.c"
#include "script_commands_map_screen.c"
