#ifndef ALCHEMY_BATTLE_SESSION_H
#define ALCHEMY_BATTLE_SESSION_H

/// @file
/// A battle's objects and sprites, each unit's command and action, and
/// the spoils.

#include "types.h"
#include "static_assert.h"

struct Object;
struct SpriteEntry;

/// A unit's place on the battlefield: its sprite object, resources and
/// animations, its anchor spot and scale, and the statuses it shows.
/// BattleWork.objects holds the 14, reached by unit id through
/// BattleObject_Get; BattleObject_Make fills one in when the unit is placed.
struct BattleObject {
  struct Object *object; ///< NULL once released
  u16 resource;          ///< the unit's sprite
  u16 overlay;   ///< a second sprite drawn over it, the weapon; 0 for none
  u16 animation; ///< the status icon's animation shown; 0 for none
  u16 effect;    ///< an effect sprite drawn with the unit
  s32 anchorX;   ///< 16.16 fixed-point pixels
  s32 anchorZ;   ///< 16.16 fixed-point pixels
  /// The unit sprite's SpriteColorMode: an enemy's own, 1 for a
  /// character on the enemy side (a link opponent), else 0.
  s32 palette;
  s32 scale;                          ///< 16.16 fixed point, 0x10000 full size
  u16 activeStatuses;                 ///< a bit per status with an icon (ICON_)
  u8 shownStatus;                     ///< the status whose icon is shown
  s8 cycleTimer;                      ///< frames until the next status icon
  struct SpriteEntry *animationEntry; ///< the status icon's sprite, or NULL
  struct SpriteEntry *effectEntry;    ///< the effect sprite, or NULL
  /// Set by BattleObject_MarkForRelease: the object is released with the
  /// others by BattleObject_DestroyAll.
  s16 active;
  s16 fading; ///< set while a fallen enemy's picture is being removed
};

/// BattleAction.command and BattleTurn's copy of it: what a unit does
/// on its turn.
enum BattleCommand {
  COMMAND_ATTACK = 0,
  COMMAND_PSYNERGY = 1,
  COMMAND_ITEM = 2,
  COMMAND_DEFEND = 3,
  COMMAND_ENEMY_ABILITY = 4, ///< an enemy's ability
  COMMAND_DJINN = 5,
  COMMAND_SUMMON = 6,
  COMMAND_TOTAL_DEFENSE =
      7,            ///< an enemy's Total Defense (ABILITY_EFFECT_TOTAL_DEFENSE)
  COMMAND_NONE = 8, ///< nothing this turn
  COMMAND_FLEE = 99
};

/// A unit's chosen action for the turn: its command, the command's parameter,
/// its target and range, and its priority in the turn order. BattleWork.actions
/// holds the turn's actions.
struct BattleAction {
  s16 unitId;
  u16 slot;      ///< in a link battle, unitId as the opponent knows it
  u16 priority;  ///< from the unit's Agility; higher acts first
  s16 command;   ///< enum BattleCommand
  s16 parameter; ///< an ability id, or COMMAND_ITEM's inventory index (-1 gone)
  s16 target;    ///< the aimed unit's id
  s16 range;     ///< the action's range
  s16 unused;    ///< no code names it
};

/// A battle's spoils so far: coins, experience, enemies defeated and up to
/// four items. BattleWork.spoils, cleared as the battle starts and added to as
/// each enemy falls.
struct BattleSpoils {
  s32 coins;
  s32 experience;
  s32 defeated; ///< enemies defeated
  /// Item ids dropped, 0 for an empty place; each id once, and a fifth drop
  /// replaces the lowest ranked (BattleSpoils_RankItem) if it ranks higher.
  u16 items[4];
};

STATIC_ASSERT(sizeof(struct BattleObject) == 0x2c);
STATIC_ASSERT(offsetof(struct BattleObject, anchorX) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleObject, activeStatuses) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleObject, cycleTimer) == 0x1f);
STATIC_ASSERT(offsetof(struct BattleObject, active) == 0x28);

STATIC_ASSERT(sizeof(struct BattleAction) == 0x10);
STATIC_ASSERT(offsetof(struct BattleAction, priority) == 0x04);
STATIC_ASSERT(offsetof(struct BattleAction, command) == 0x06);
STATIC_ASSERT(offsetof(struct BattleAction, parameter) == 0x08);
STATIC_ASSERT(offsetof(struct BattleAction, target) == 0x0a);
STATIC_ASSERT(sizeof(struct BattleSpoils) == 0x14);

#endif
