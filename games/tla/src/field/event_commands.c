/// @file
/// The event_commands object, its parts in event_commands/ in ROM order,
/// ending with the event commands that follow it: the effect scene, Mind Read
/// to Cloak, the rest of Cloak and Carry. event_commands_internal.h holds
/// the includes every part shares.

#include "event_commands_internal.h"

// The parts, in ROM order.
#include "event_commands_actor_commands.c"
#include "event_commands_messages.c"
#include "event_commands_camera.c"
#include "event_commands_view.c"
#include "event_commands_movement_types.c"
#include "event_commands_exit_animations.c"
#include "event_commands_line_scroll.c"
#include "event_commands_weather.c"
#include "event_commands_ground_particles.c"
#include "event_commands_motes.c"
#include "event_commands_djinni.c"
#include "event_commands_jupiter.c"
#include "event_commands_mars.c"
#include "event_commands_venus.c"
#include "event_commands_mercury.c"
#include "event_commands_ropes.c"
#include "event_commands_psynergy_dispatch.c"
#include "event_commands_field_psynergy.c"
#include "event_commands_field_psynergy_state.c"
#include "event_commands_mind_read.c"

#include "event_commands_cloak.c"

#include "event_commands_carry.c"
