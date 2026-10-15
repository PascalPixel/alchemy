#ifndef ALCHEMY_BATTLE_SESSION_H
#define ALCHEMY_BATTLE_SESSION_H

#include "types.h"
#include "layout_guard.h"

struct MotionObject;
struct SpriteEntry;

struct BattleObj
{
	struct MotionObject *object;
	u16					 resource;
	u16					 overlay;
	u16					 animation;
	u16					 effect;
	s32					 anchor_x;
	s32					 anchor_z;
	s32					 palette;
	s32					 scale;
	u16					 active_conditions;
	u8					 selected_condition;
	s8					 cycle_timer;
	struct SpriteEntry	*animation_entry;
	struct SpriteEntry	*effect_entry;
	s16					 active;
	s16					 fading;
};

struct BattleActionRecord
{
	s16 unit_id;
	u16 slot;
	u16 priority;
	s16 command;
	s16 parameter;
	s16 target;
	s16 range;
	s16 unknown_0e;
};

struct BattleSpoils
{
	s32 coins;
	s32 experience;
	s32 defeated;
	u16 items[4];
};

LAYOUT_SIZE_GUARD(BattleObjectSlot_Size, struct BattleObj, 0x2c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_AnchorX, struct BattleObj, anchor_x, 0x0c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Conditions, struct BattleObj, active_conditions, 0x1c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_CycleTimer, struct BattleObj, cycle_timer, 0x1f);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Active, struct BattleObj, active, 0x28);

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Priority, struct BattleActionRecord, priority, 0x04);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Command, struct BattleActionRecord, command, 0x06);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Parameter, struct BattleActionRecord, parameter, 0x08);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Target, struct BattleActionRecord, target, 0x0a);
LAYOUT_SIZE_GUARD(BattleSpoils_Size, struct BattleSpoils, 0x14);

#endif
