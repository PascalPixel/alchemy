#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

/* The status icons shown over a unit in battle. */

#include "edition.h"
#include "battle_action.h"

void btUnitBuildStatusFlags(Sint32 id, BATTLE_OBJECT *slot);
void btStatusIconCycle(BATTLE_OBJECT *slot);

#endif
