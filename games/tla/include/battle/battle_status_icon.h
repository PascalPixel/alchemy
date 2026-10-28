#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

#include "edition.h"
#include "battle_action.h"

s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot);
s32 BattleStatusIcon_Cycle(struct BattleObj *slot);

#endif
