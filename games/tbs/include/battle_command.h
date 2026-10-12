#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "unit/battle/tgtexec.h"
#include "btlsys.h"
#include "item.h"
#include "keystate.h"
#include "frame.h"
#include "btlwork.h"

#define BATTLE_AUTO_MODE (*(volatile u8 *)&gDebugMode)
#define BATTLE_OPTIONS gKeysHeld

extern const u32 Battle_ActionFlags[];
extern const u8	 Battle_ActionStatus[];

#endif
