#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "edition.h"
#include "static_assert.h"

enum { BATTLE_EVENT_CAPACITY = 64 };

/// BattleEventState.phase: how far BattleEvent_Playback has got through the
/// plan's targets.
enum BattlePlaybackPhase {
  BATTLE_PLAYBACK_IDLE = 0,           ///< paused until the effect resumes it
  BATTLE_PLAYBACK_RESOLVE_TARGET = 1, ///< resolves the next target's events
  BATTLE_PLAYBACK_DISPATCH = 2,       ///< plays the queued events in order
  BATTLE_PLAYBACK_READY = 4           ///< every target has been played
};

/// The battle event queue: up to 64 events, each an opcode (enum
/// BattleEventOpcode) and one operand, in the order they play.
struct BattleEventQueue {
  u8 opcodes[BATTLE_EVENT_CAPACITY];
  u32 operands[BATTLE_EVENT_CAPACITY];
  s32 targetIndex; ///< the plan's next target to resolve
  s32 count;       ///< events queued
};

STATIC_ASSERT(sizeof(struct BattleEventQueue) == 0x148);
STATIC_ASSERT(offsetof(struct BattleEventQueue, operands) == 0x040);
STATIC_ASSERT(offsetof(struct BattleEventQueue, targetIndex) == 0x140);
STATIC_ASSERT(offsetof(struct BattleEventQueue, count) == 0x144);

/// The battle events BattleEvent_Push queues, each with one operand, for
/// BattleEvent_Playback (or BattleEvent_DispatchQueued) to play in order. The
/// names, numbers and messages build what the player reads: the arguments
/// first, then the message that shows them.
enum BattleEventOpcode {
  BATTLE_EVENT_UNIT_NAME = 0,       ///< a unit's name for the next message
  BATTLE_EVENT_NUMBER = 1,          ///< a number for the next message
  BATTLE_EVENT_ITEM_NAME = 2,       ///< an item's name for the next message
  BATTLE_EVENT_ABILITY_NAME = 3,    ///< an ability's name for the next message
  BATTLE_EVENT_MESSAGE = 4,         ///< a message, waiting for the player
  BATTLE_EVENT_MESSAGE_NO_WAIT = 5, ///< a message, going straight on
  BATTLE_EVENT_MARK = 6,            ///< sets gBattleDisplayWork->marked
  BATTLE_EVENT_CLEAR_ARGUMENTS = 7, ///< forgets the names and numbers pushed
  BATTLE_EVENT_HIT = 8,             ///< the unit flinches and flashes
  BATTLE_EVENT_DEFEAT = 9,          ///< the unit falls; an enemy fades out
  BATTLE_EVENT_DRAW_STATUS = 10,    ///< redraws the party's status
  BATTLE_EVENT_REFRESH_UNIT = 11,   ///< the unit's status icons, colour, pose
  BATTLE_EVENT_REMOVE_UNIT = 12,    ///< an enemy leaves the battle
  BATTLE_EVENT_DJINN_BONUS = 13,    ///< later defeats give the Djinn bonus
  BATTLE_EVENT_SOUND = 14           ///< plays a sound
};

s32 BattleActor_DestroyTemporaryObject(s32 unitId);

void BattleEvent_Push(u32 opcode, u32 operand);
u32 BattleEvent_DispatchQueued(void);

#endif
