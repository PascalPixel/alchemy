/// @file
/// The battle object: a battle from its opening transition to its spoils.
///
/// A battle runs from Battle_Run (run.c). Each turn the actions are gathered
/// and put in action order (turn.c, or auto_battle.c under auto battle), and
/// each action is announced, built for its targets (BattleActionRun_Build,
/// action.c), staged (BattleStaging_DispatchAction, staging.c) and applied to
/// each target (BattleAbility_ApplyToTarget, ability.c), which queues the
/// playback BattlePlayback_Run plays (playback.c).

#include "battle_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "battle/debug.c"
#include "battle/setup.c"
#include "battle/link.c"
#include "battle/run.c"
#include "battle/sprite_slots.c"
#include "battle/placement.c"
#include "battle/objects.c"
#include "battle/motion.c"
#include "battle/auto_battle.c"
#include "battle/turn.c"
#include "battle/staging.c"
#include "battle/targeting.c"
#include "battle/playback_queue.c"
#include "battle/ability.c"
#include "battle/enemy_ai.c"
#include "battle/playback.c"
#include "battle/action.c"
#include "battle/turn_end.c"
#include "battle/transition.c"
#include "battle/background.c"
#include "battle/element_glow.c"
#include "battle/formation.c"
#include "battle/battle_sprite.c"
#include "battle/spoils.c"
