/// @file
/// The battle object: a battle from its opening transition to its spoils.
/// battle_internal.h holds what the parts share (the includes and the names
/// two or more of them use); the code is in the files under battle/, included
/// here in the order it has in the ROM. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// one family ends and the next begins.
///
/// A battle runs from Battle_Run (encounter.c). Each round the
/// actions are gathered and put in turn order (round.c, or auto_battle.c under
/// auto battle), and each action is announced and planned
/// (BattleTurn_Build, plan.c), staged
/// (BattleStaging_DispatchAction, staging.c) and applied to each target
/// (BattleAbility_ApplyToTarget, target_exec.c), which queues the battle events
/// BattlePlayback_Run plays (playback.c). README.md in this folder gives the
/// words, prefixes and spellings the parts use.

#include "battle_internal.h"

// The battle object's code, in ROM order. Each part is compiled here, as
// part of this one object, never on its own: the order of these includes is
// the order of the code in the ROM.
#include "battle/debug.c"
#include "battle/setup.c"
#include "battle/link.c"
#include "battle/encounter.c"
#include "battle/sprites.c"
#include "battle/placement.c"
#include "battle/objects.c"
#include "battle/motion.c"
#include "battle/auto_battle.c"
#include "battle/round.c"
#include "battle/staging.c"
#include "battle/targeting.c"
#include "battle/events.c"
#include "battle/target_exec.c"
#include "battle/enemy_ai.c"
#include "battle/playback.c"
#include "battle/plan.c"
#include "battle/turn_end.c"
#include "battle/transition.c"
#include "battle/background.c"
#include "battle/star_field.c"
#include "battle/formation.c"
#include "battle/classes.c"
#include "battle/spoils.c"
