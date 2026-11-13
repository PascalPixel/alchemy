/// @file
/// The first object of the battle effect bank: the display and HBlank helpers,
/// the canvas layer, the dispatch that runs summon and table effects, and the
/// effects from Ice and Douse to the battler tints, as The Broken Seal's
/// battle_effect.c holds them. battle_effect_internal.h holds the includes
/// every part shares; the parts under battle_effect/ follow in ROM order. The
/// bank is five objects, not The Broken Seal's one, because the merge could
/// not reconcile cybele.c and liquifier.c with their neighbours.

#include "battle_effect_internal.h"

// The parts, in ROM order.
#include "battle_effect/display.c"
#include "battle_effect/falling.c"
#include "battle_effect/streaks.c"
#include "battle_effect/target_strike.c"
#include "battle_effect/death_plunge.c"
#include "battle_effect/canvas.c"
#include "battle_effect/planet_diver.c"
#include "battle_effect/mirrored_wall.c"
#include "battle_effect/condemn.c"
#include "battle_effect/bind.c"
#include "battle_effect/stars.c"
#include "battle_effect/procne.c"
#include "battle_effect/nereid.c"
#include "battle_effect/vortices.c"
#include "battle_effect/prism.c"
#include "battle_effect/plasma.c"
#include "battle_effect/volcano.c"
#include "battle_effect/target_motes.c"
#include "battle_effect/djinn_heal.c"
#include "battle_effect/dispatch.c"
#include "battle_effect/battler_tint.c"
