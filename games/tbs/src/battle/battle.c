/// @file
/// The battle object: a battle from its opening transition to its spoils.
///
/// A battle runs from Battle_Run (battle_run.c). Each turn the actions are
/// gathered and put in action order (battle_turn.c, or battle_auto_battle.c
/// under auto battle), and each action is announced, built for its targets
/// (BattleActionRun_Build, battle_action.c), staged
/// (BattleStaging_DispatchAction, battle_staging.c) and applied to each target
/// (BattleAbility_ApplyToTarget, battle_ability.c), which queues the playback
/// BattlePlayback_Run plays (battle_playback.c).

#include "battle_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
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
#include "battle_targeting.c"
#include "battle_playback_queue.c"
#include "battle_ability.c"
#include "battle_enemy_ai.c"
#include "battle_playback.c"
#include "battle_action.c"
#include "battle_turn_end.c"
#include "battle_transition.c"
#include "battle_background.c"
#include "battle_element_glow.c"
#include "battle_formation.c"
#include "battle_sprite.c"
#include "battle_spoils.c"
