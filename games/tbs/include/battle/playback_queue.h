#ifndef ALCHEMY_PLAYBACK_QUEUE_H
#define ALCHEMY_PLAYBACK_QUEUE_H

/// @file
/// The playback queue: the commands BattlePlayback_Push queues and the
/// playback phases that play them.

#include "edition.h"
#include "static_assert.h"

enum { BATTLE_PLAYBACK_CAPACITY = 64 };

/// BattlePlaybackWork.phase: how far BattlePlayback_Run has got through the
/// turn's targets. Its message phases (PLAY_MESSAGE and the like) are
/// private to playback.c.
enum BattlePlaybackPhase {
  BATTLE_PLAYBACK_PHASE_IDLE = 0, ///< paused until the animation resumes it
  BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET =
      1,                              ///< resolves the next target's commands
  BATTLE_PLAYBACK_PHASE_DISPATCH = 2, ///< plays the queued commands in order
  BATTLE_PLAYBACK_PHASE_READY = 4     ///< every target has been played
};

/// The playback queue: up to 64 commands, each an opcode (enum
/// BattlePlaybackCommand) and one operand, in the order they play. It lives in
/// BattleWork.playback.queue: BattlePlayback_Push appends to it and playback
/// reads it.
struct BattlePlaybackQueue {
  u8 opcodes[BATTLE_PLAYBACK_CAPACITY]; ///< enum BattlePlaybackCommand
  /// Each command's operand: a unit, item or ability id, a number, a message or
  /// a sound.
  u32 operands[BATTLE_PLAYBACK_CAPACITY];
  s32 targetIndex; ///< the plan's next target to resolve
  s32 count;       ///< commands queued
};

STATIC_ASSERT(sizeof(struct BattlePlaybackQueue) == 0x148);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, operands) == 0x040);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, targetIndex) == 0x140);
STATIC_ASSERT(offsetof(struct BattlePlaybackQueue, count) == 0x144);

/// The playback commands BattlePlayback_Push queues, each with one operand, for
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
  PLAYBACK_MARK = 6,            ///< sets gBattleMessageWork->marked
  PLAYBACK_CLEAR_ARGUMENTS = 7, ///< forgets the names and numbers pushed
  PLAYBACK_HIT = 8,             ///< the unit flinches and flashes
  PLAYBACK_DEFEAT = 9,          ///< the unit falls; an enemy fades out
  PLAYBACK_DRAW_STATUS = 10,    ///< redraws the party's status
  PLAYBACK_REFRESH_UNIT = 11,   ///< the unit's status icons, colour, pose
  PLAYBACK_REMOVE_UNIT = 12,    ///< an enemy leaves the battle
  PLAYBACK_DJINN_BONUS = 13,    ///< later defeats give the Djinn bonus
  PLAYBACK_SOUND = 14           ///< plays a sound
};

#endif
