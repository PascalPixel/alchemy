#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
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
