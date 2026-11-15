/// @file
/// The event_commands object, its parts in event_commands/ in ROM order,
/// ending with the event commands that follow it: the effect scene, Mind Read
/// to Cloak, the rest of Cloak and Carry. event_commands_internal.h holds
/// the includes every part shares.

#include "event_commands_internal.h"

// The parts, in ROM order.
#include "event_commands/actor_commands.c"
#include "event_commands/messages.c"
#include "event_commands/camera.c"
#include "event_commands/view.c"
#include "event_commands/movement_types.c"
#include "event_commands/exit_animations.c"
#include "event_commands/line_scroll.c"
#include "event_commands/weather.c"
#include "event_commands/ground_particles.c"
#include "event_commands/motes.c"
#include "event_commands/djinni.c"
#include "event_commands/jupiter.c"
#include "event_commands/mars.c"
#include "event_commands/venus.c"
#include "event_commands/mercury.c"
#include "event_commands/ropes.c"
#include "event_commands/psynergy_dispatch.c"
#include "event_commands/field_psynergy.c"

#undef Actor_ShowEmoteById
#undef PaletteFade_SetTarget
#undef Actor_ReleaseFar

#include "event_commands/field_psynergy_work.c"
#include "event_commands/mind_read.c"

#undef CLOAK_FLICKER_PALETTE

#undef Actor_ApplyRandomChildValues
#undef ScreenPoint
#undef EffectWork

#include "event_commands/cloak.c"

#include "event_commands/carry.c"
