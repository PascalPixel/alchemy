#ifndef ALCHEMY_BATTLE_ACTION_H
#define ALCHEMY_BATTLE_ACTION_H

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

struct MotionObject;
struct SpriteAnimation;

/// A unit's place on the battlefield: its sprite object, resources and
/// animations, its anchor spot and scale, and the conditions it shows.
struct BattleObject {
  struct MotionObject *object; ///< NULL once released
  u16 resource;                ///< the unit's sprite
  u16 overlay;                 ///< a second sprite drawn over it; 0 for none
  u16 animation; ///< the status icon's animation shown; 0 for none
  u16 shadow;    ///< the shadow sprite drawn under the unit
  FIXED anchorX; ///< 16.16 fixed-point pixels
  FIXED anchorZ; ///< 16.16 fixed-point pixels
  s32 palette;   ///< the unit sprite's colour mode, an enum SpriteColorMode
  FIXED scale;   ///< 16.16 fixed point, FIXED_ONE full size
  u16 activeStatuses;                     ///< a bit per status with an icon
  u8 selectedCondition;                   ///< the status whose icon is shown
  s8 cycleTimer;                          ///< frames until the next status icon
  struct SpriteAnimation *animationEntry; ///< the status icon's sprite, or NULL
  struct SpriteAnimation *shadowEntry;    ///< the shadow sprite, or NULL
  /// Set by BattleObject_MarkForRelease: the object is released with the
  /// others.
  s16 active;
  // The Broken Seal's last halfword, fading, is two bytes here: the battle
  // code reads and writes each on its own.
  u8 fading;  ///< set while a fallen enemy's picture is being removed
  u8 visible; ///< 1 from BattleObject_Reset, 0 from BattleObject_Make
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
  COMMAND_TOTAL_DEFENSE = 7, ///< an enemy's Total Defense (effect 47)
  COMMAND_NONE = 8,          ///< nothing this turn
  /// The Djinn Counter (ability 299) a unit strikes back with, outside the
  /// turn order (Battle_Run queues it for BattleWork.pendingUnit).
  COMMAND_DJINN_COUNTER = 9,
  /// Daedalus's strike again at a turn's end, queued by Battle_Run while
  /// summons of Daedalus are owed (BattleStaging_PlanDaedalus).
  COMMAND_DAEDALUS = 10,
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
  /// Which of the unit's actions this turn it is, from 0: Poseidon's and the
  /// Doom Dragon's forms choose each by it (BattleAI_ChooseAction).
  s16 turnAction;
};

/// A battle's spoils so far: coins, experience, enemies defeated and up to
/// four items.
struct BattleSpoils {
  s32 coins;
  s32 experience;
  s32 defeated; ///< enemies defeated
  u16 items[4];
  /// The two enemies last felled (BattleEnemy_RecordDefeat), which Regen Dance
  /// (ABILITY_EFFECT_CALL_QUEUED) calls back: their enemy ids and name letters,
  /// the place of the last felled and how many there are.
  ///@{
  u16 queuedSummons[2];
  u8 queuedLetters[2];
  s8 queueHead;
  s8 queueCount;
  ///@}
};

STATIC_ASSERT(sizeof(struct BattleObject) == 0x2c);
STATIC_ASSERT(offsetof(struct BattleObject, anchorX) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleObject, activeStatuses) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleObject, cycleTimer) == 0x1f);
STATIC_ASSERT(offsetof(struct BattleObject, active) == 0x28);

STATIC_ASSERT(sizeof(struct BattleActionRecord) == 0x10);
STATIC_ASSERT(offsetof(struct BattleActionRecord, priority) == 0x04);
STATIC_ASSERT(offsetof(struct BattleActionRecord, command) == 0x06);
STATIC_ASSERT(offsetof(struct BattleActionRecord, parameter) == 0x08);
STATIC_ASSERT(offsetof(struct BattleActionRecord, target) == 0x0a);
STATIC_ASSERT(sizeof(struct BattleSpoils) == 0x1c);

/// A unit's chosen action as the party's command menus write it: the
/// BattleActionRecord, read with its fields unsigned.
struct BattleAction {
  u16 unitId;
  u16 slot;
  s16 priority;
  u16 command;
  u16 parameter;
  u16 target;
  u16 range;
  u16 turnAction;
};

#endif
