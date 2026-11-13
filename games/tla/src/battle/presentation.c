/// @file
/// The rest of the battle object (battle.c): a unit's motion graphics, the
/// strongest unit, applying an ability to its targets, playback, the end of a
/// turn, the screen transitions, the background, camera and highlights, enemy
/// formations and the spoils, as The Broken Seal's battle.c ends. The ROM shows
/// one object; battle.c says why it is two here. presentation_internal.h holds
/// the includes every part shares; the parts under presentation/ follow in ROM
/// order.

#include "presentation_internal.h"

// The parts, in ROM order.
#include "presentation/motion.c"
#include "presentation/strongest_unit.c"
#include "presentation/targeting.c"
#include "presentation/playback.c"
#include "presentation/action.c"
#include "presentation/turn_end.c"
#include "presentation/transition.c"
#include "presentation/background.c"
#include "presentation/formation.c"
#include "presentation/spoils.c"
