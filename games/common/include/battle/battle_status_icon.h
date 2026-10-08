#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

#include "battle_session.h"

s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *slot);
s32 BattleStatusIcon_Cycle(struct BtlObj *slot);

#endif
