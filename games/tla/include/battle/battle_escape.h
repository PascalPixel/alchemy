#ifndef ALCHEMY_BATTLE_ESCAPE_H
#define ALCHEMY_BATTLE_ESCAPE_H

// Auto battle's escape: the party running from the battle.

#include "edition.h"
#include "battle_action.h"

#include "types.h"

Sint32 btFleeCheckSuccess(void);
Bool   btFleePlayRun(BATTLE_ACTION_RECORD *action);

#endif
