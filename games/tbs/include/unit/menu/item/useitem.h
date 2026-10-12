#ifndef ALCHEMY_UNIT_MENU_ITEM_USEITEM_H
#define ALCHEMY_UNIT_MENU_ITEM_USEITEM_H

#include "types.h"

void Item_PrepareUsePalette(void);
void Item_UseNoOpCallback(void);
s32 Item_Use(s32 slot, s32 owner_id, s32 target_id);
s32 Item_ReturnOne(void);
s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 source_id, s32 target_id, s32 fixed_scale);
void Item_PlayUseAnimation(u32 item);
void Ability_PlayUseAnimation(s32 action);

#endif
