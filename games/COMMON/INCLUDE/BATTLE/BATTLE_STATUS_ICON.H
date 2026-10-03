#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

#include "BATTLE_SESSION.H"

s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObjectSlot *slot);
s32 BattleStatusIcon_Cycle(struct BattleObjectSlot *slot);

#endif
