/// @file
/// The battle object: a battle from its opening transition to its spoils.
/// battle_internal.h holds what the parts share (the includes and the names
/// two or more of them use); the code is in the files under battle/, included
/// here in the order it has in the ROM. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// one family ends and the next begins.
///
/// A battle runs from Battle_Run (run.c). Each turn the
/// actions are gathered and put in action order (turn.c, or auto_battle.c under
/// auto battle), and each action is announced, built for its targets
/// (BattleActionRun_Build, action.c), staged
/// (BattleStaging_DispatchAction, staging.c) and applied to each target
/// (BattleAbility_ApplyToTarget, ability.c), which queues the playback
/// BattlePlayback_Run plays (playback.c). README.md in this folder gives the
/// words, prefixes and spellings the parts use.

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
