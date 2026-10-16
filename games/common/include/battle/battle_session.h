#ifndef ALCHEMY_BATTLE_SESSION_H
#define ALCHEMY_BATTLE_SESSION_H

#include "types.h"
#include "layout_guard.h"

struct MotionObject;
struct SpriteEntry;

struct BattleObj {
  struct MotionObject *object;
  u16 resource;
  u16 overlay;
  u16 animation;
  u16 effect;
  s32 anchorX;
  s32 anchorZ;
  s32 palette;
  s32 scale;
  u16 activeConditions;
  u8 selectedCondition;
  s8 cycleTimer;
  struct SpriteEntry *animationEntry;
  struct SpriteEntry *effectEntry;
  s16 active;
  s16 fading;
};

// BattleActionRecord.command and BattlePlan's copy of it: what a unit does
// on its turn.
enum BattleCommand {
  CMD_ATTACK = 0,
  CMD_PSYNERGY = 1,
  CMD_ITEM = 2,
  CMD_DEFEND = 3,
  CMD_ABILITY = 4, // an enemy's ability
  CMD_DJINN = 5,
  CMD_SUMMON = 6,
  CMD_DEFEND2 = 7, // an enemy's Total Defense (effect 47)
  CMD_NONE = 8,    // nothing this turn
  CMD_FLEE = 99
};

struct BattleActionRecord {
  s16 unitId;
  u16 slot;
  u16 priority;
  s16 command;
  s16 parameter;
  s16 target;
  s16 range;
  s16 unk0e;
};

struct BattleSpoils {
  s32 coins;
  s32 experience;
  s32 defeated;
  u16 items[4];
};

LAYOUT_SIZE_GUARD(BattleObjectSlot_Size, struct BattleObj, 0x2c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_AnchorX, struct BattleObj, anchorX, 0x0c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Conditions, struct BattleObj,
                    activeConditions, 0x1c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_CycleTimer, struct BattleObj, cycleTimer,
                    0x1f);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Active, struct BattleObj, active, 0x28);

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Priority, struct BattleActionRecord,
                    priority, 0x04);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Command, struct BattleActionRecord,
                    command, 0x06);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Parameter, struct BattleActionRecord,
                    parameter, 0x08);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Target, struct BattleActionRecord,
                    target, 0x0a);
LAYOUT_SIZE_GUARD(BattleSpoils_Size, struct BattleSpoils, 0x14);

#endif
