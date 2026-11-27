#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

/* The status icons shown over a unit in battle. */

#include "edition.h"
#include "battle_action.h"

void btUnitBuildStatFlags(Sint32 id, BATTLE_OBJECT *slot);
void btStatIconCycle(BATTLE_OBJECT *slot);

#endif
