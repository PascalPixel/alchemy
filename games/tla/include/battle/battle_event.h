#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "edition.h"
#include "static_assert.h"

/// The most events a playback queues (BattlePlaybackWork's opcodes and
/// operands).
enum { BATTLE_PLAYBACK_CAPACITY = 64 };

/// BattlePlaybackWork.phase: how far BattlePlayback_Run has got through the
/// plan's targets.
enum BattlePlaybackPhase {
  BATTLE_PLAYBACK_PHASE_IDLE = 0, ///< paused until the effect resumes it
  BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET =
      1,                              ///< resolves the next target's events
  BATTLE_PLAYBACK_PHASE_DISPATCH = 2, ///< plays the queued events in order
  BATTLE_PLAYBACK_PHASE_READY = 4     ///< every target has been played
};

/// The battle event queue: up to 64 events, each an opcode (enum
/// BattlePlaybackCommand) and one operand, in the order they play.
struct BattlePlaybackQueue {
  u8 opcodes[BATTLE_PLAYBACK_CAPACITY];
  u32 operands[BATTLE_PLAYBACK_CAPACITY];
  s32 targetIndex; ///< the plan's next target to resolve
  s32 count;       ///< events queued
};

STATIC_ASSERT(sizeof(struct BattlePlaybackQueue) == 328);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, operands) == 64);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, targetIndex) == 320);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, count) == 324);

/// The battle events BattlePlayback_Push queues, each with one operand, for
/// BattlePlayback_Run (or BattlePlayback_RunAll) to play in order. The
/// names, numbers and messages build what the player reads: the arguments
/// first, then the message that shows them.
enum BattlePlaybackCommand {
  PLAYBACK_UNIT_NAME = 0,       ///< a unit's name for the next message
  PLAYBACK_NUMBER = 1,          ///< a number for the next message
  PLAYBACK_ITEM_NAME = 2,       ///< an item's name for the next message
  PLAYBACK_ABILITY_NAME = 3,    ///< an ability's name for the next message
  PLAYBACK_MESSAGE = 4,         ///< a message, waiting for the player
  PLAYBACK_MESSAGE_NO_WAIT = 5, ///< a message, going straight on
  PLAYBACK_MARK = 6,            ///< sets gBattleDisplayWork->marked
  PLAYBACK_CLEAR_ARGUMENTS = 7, ///< forgets the names and numbers pushed
  PLAYBACK_HIT = 8,             ///< the unit flinches and flashes
  PLAYBACK_DEFEAT = 9,          ///< the unit falls; an enemy fades out
  PLAYBACK_DRAW_STATUS = 10,    ///< redraws the party's status
  PLAYBACK_REFRESH_UNIT = 11,   ///< the unit's status icons, colour, pose
  PLAYBACK_REMOVE_UNIT = 12,    ///< an enemy leaves the battle
  PLAYBACK_DJINN_BONUS = 13,    ///< later defeats give the Djinn bonus
  PLAYBACK_SOUND = 14,          ///< plays a sound
  PLAYBACK_BLOW_AWAY = 15 ///< the unit leaves the lists, as one that flees
};

s32 BattleObject_DestroyTemporaryObject(s32 unitId);

void BattlePlayback_Push(u32 opcode, u32 operand);

/// Queues a message that waits for the player.
#define PUSH_MESSAGE(message)                                                  \
  BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)(message))
u32 BattlePlayback_RunAll(void);

#endif
