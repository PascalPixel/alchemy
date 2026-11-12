#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

#include "edition.h"
#include "battle_action.h"

void BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot);
void BattleStatusIcon_Cycle(struct BattleObj *slot);

#endif
