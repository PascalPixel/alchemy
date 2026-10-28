#ifndef ALCHEMY_BATTLE_ACTION_H
#define ALCHEMY_BATTLE_ACTION_H

#include "types.h"
#include "static_assert.h"

struct MotionObject;
struct SpriteEntry;

/// A unit's place on the battlefield: its sprite object, resources and
/// animations, its anchor spot and scale, and the conditions it shows.
struct BattleObj {
  struct MotionObject *object; ///< NULL once released
  u16 resource;                ///< the unit's sprite
  u16 overlay;                 ///< a second sprite drawn over it; 0 for none
  u16 animation;        ///< the status icon's animation shown; 0 for none
  u16 effect;           ///< an effect sprite drawn with the unit
  s32 anchorX;          ///< 16.16 fixed-point pixels
  s32 anchorZ;          ///< 16.16 fixed-point pixels
  s32 palette;          ///< the unit sprite's palette
  s32 scale;            ///< 16.16 fixed point, 0x10000 full size
  u16 activeConditions; ///< a bit per status with an icon
  u8 selectedCondition; ///< the status whose icon is shown
  s8 cycleTimer;        ///< frames until the next status icon
  struct SpriteEntry *animationEntry; ///< the status icon's sprite, or NULL
  struct SpriteEntry *effectEntry;    ///< the effect sprite, or NULL
  /// Set by BattleObject_MarkForRelease: the object is released with the
  /// others.
  s16 active;
  s16 fading; ///< set while a fallen enemy's picture is being removed
};

/// BattleActionRecord.command and BattlePlan's copy of it: what a unit does
/// on its turn.
enum BattleCommand {
  COMMAND_ATTACK = 0,
  COMMAND_PSYNERGY = 1,
  COMMAND_ITEM = 2,
  COMMAND_DEFEND = 3,
  COMMAND_ABILITY = 4, ///< an enemy's ability
  COMMAND_DJINN = 5,
  COMMAND_SUMMON = 6,
  COMMAND_DEFEND2 = 7, ///< an enemy's Total Defense (effect 47)
  COMMAND_NONE = 8,    ///< nothing this turn
  COMMAND_FLEE = 99
};

/// A unit's chosen action for the turn: its command, the command's parameter,
/// its target and range, and its priority in the turn order.
struct BattleActionRecord {
  s16 unitId;
  u16 slot;      ///< in a link battle, unitId as the opponent knows it
  u16 priority;  ///< its place in the turn order
  s16 command;   ///< enum BattleCommand
  s16 parameter; ///< the command's ability, item or Djinni
  s16 target;    ///< the aimed unit's id
  s16 range;     ///< the action's range
  s16 unk0e;     ///< which of the unit's actions this turn it is, from 0
};

/// A battle's spoils so far: coins, experience, enemies defeated and up to
/// four items.
struct BattleSpoils {
  s32 coins;
  s32 experience;
  s32 defeated; ///< enemies defeated
  u16 items[4];
};

STATIC_ASSERT(sizeof(struct BattleObj) == 0x2c);
STATIC_ASSERT(offsetof(struct BattleObj, anchorX) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleObj, activeConditions) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleObj, cycleTimer) == 0x1f);
STATIC_ASSERT(offsetof(struct BattleObj, active) == 0x28);

STATIC_ASSERT(sizeof(struct BattleActionRecord) == 0x10);
STATIC_ASSERT(offsetof(struct BattleActionRecord, priority) == 0x04);
STATIC_ASSERT(offsetof(struct BattleActionRecord, command) == 0x06);
STATIC_ASSERT(offsetof(struct BattleActionRecord, parameter) == 0x08);
STATIC_ASSERT(offsetof(struct BattleActionRecord, target) == 0x0a);
STATIC_ASSERT(sizeof(struct BattleSpoils) == 0x14);

#endif
