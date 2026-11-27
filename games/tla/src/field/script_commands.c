// The script_commands object, its parts in script_commands/ in ROM order, ending with the script
// commands that follow it: the effect scene, Mind Read to Cloak, the rest of Cloak and Carry.
// script_commands_internal.h holds the includes every part shares.

#include "script_commands_internal.h"

// The parts, in ROM order.
#include "script_commands_actor_commands.c"
#include "script_commands_messages.c"
#include "script_commands_camera.c"
#include "script_commands_view.c"
#include "script_commands_movement_types.c"
#include "script_commands_exit_animations.c"
#include "script_commands_line_scroll.c"
#include "script_commands_weather.c"
#include "script_commands_ground_particles.c"
#include "script_commands_motes.c"
#include "script_commands_djinni.c"
#include "script_commands_jupiter.c"
#include "script_commands_mars.c"
#include "script_commands_venus.c"
#include "script_commands_mercury.c"
#include "script_commands_ropes.c"
#include "script_commands_psynergy_dispatch.c"
#include "script_commands_field_psynergy.c"
#include "script_commands_field_psynergy_state.c"
#include "script_commands_mind_read.c"

#include "script_commands_cloak.c"

#include "script_commands_carry.c"
