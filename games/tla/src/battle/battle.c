/* The battle module's code, as one object in parts: the debug functions, a
   battle's setup, link battles, the whole battle, the battle sprites and
   objects, the units' movements, auto battle, a turn's actions and the start of
   an action's staging, then a unit's motion graphics, the strongest unit,
   applying an ability to its targets, playback, the end of a turn, the screen
   transitions, the background, camera and highlights, enemy formations and the
   spoils. The parts are cut where The Broken Seal's are and included in the
   order their code lies in the module; battle_internal.h holds what they
   share. */

#include "battle_internal.h"

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
   parts"). */
#include "battle_debug.c"
#include "battle_setup.c"
#include "battle_link.c"
#include "battle_run.c"
#include "battle_sprite_slots.c"
#include "battle_placement.c"
#include "battle_objects.c"
#include "battle_motion.c"
#include "battle_auto_battle.c"
#include "battle_turn.c"
#include "battle_staging.c"

/* The view battle_turn.c keeps ends here; the parts below see the name as its
   header declares it. */
#undef gmUnitCalcStub

#include "battle_motion_graphics.c"
#include "battle_strongest_unit.c"
#include "battle_targeting.c"
#include "battle_playback.c"
#include "battle_action.c"
#include "battle_turn_end.c"
#include "battle_transition.c"
#include "battle_background.c"
#include "battle_formation.c"
#include "battle_spoils.c"
