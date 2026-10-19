#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "edition.h"
#include "static_assert.h"

enum { BATTLE_EVENT_CAPACITY = 64 };

enum BattlePlaybackPhase {
  BATTLE_PLAYBACK_IDLE = 0,
  BATTLE_PLAYBACK_RESOLVE_TARGET = 1,
  BATTLE_PLAYBACK_DISPATCH = 2,
  BATTLE_PLAYBACK_READY = 4
};

struct BattleEventQueue {
  u8 opcodes[BATTLE_EVENT_CAPACITY];
  u32 operands[BATTLE_EVENT_CAPACITY];
  s32 targetIndex;
  s32 count;
};

STATIC_ASSERT(sizeof(struct BattleEventQueue) == 0x148);
STATIC_ASSERT(offsetof(struct BattleEventQueue, operands) == 0x040);
STATIC_ASSERT(offsetof(struct BattleEventQueue, targetIndex) == 0x140);
STATIC_ASSERT(offsetof(struct BattleEventQueue, count) == 0x144);

// The battle events BattleEvPush queues, each with one operand, for
// BattleEvent_Playback (or BattleEv_DispatchQueued) to play in order. The
// names, numbers and messages build what the player reads: the arguments
// first, then the message that shows them.
enum BattleEventOpcode {
  BATTLE_EVENT_UNIT_NAME = 0,       // a unit's name for the next message
  BATTLE_EVENT_NUMBER = 1,          // a number for the next message
  BATTLE_EVENT_ITEM_NAME = 2,       // an item's name for the next message
  BATTLE_EVENT_ABILITY_NAME = 3,    // an ability's name for the next message
  BATTLE_EVENT_MESSAGE = 4,         // a message, waiting for the player
  BATTLE_EVENT_MESSAGE_NO_WAIT = 5, // a message, going straight on
  BATTLE_EVENT_MARK = 6,            // sets gBattleDisplayWork->marked
  BATTLE_EVENT_CLEAR_ARGUMENTS = 7, // forgets the names and numbers pushed
  BATTLE_EVENT_HIT = 8,             // the unit flinches and flashes
  BATTLE_EVENT_DEFEAT = 9,          // the unit falls; an enemy fades out
  BATTLE_EVENT_DRAW_STATUS = 10,    // redraws the party's status
  BATTLE_EVENT_REFRESH_UNIT = 11,   // the unit's status icons, colour, pose
  BATTLE_EVENT_REMOVE_UNIT = 12,    // an enemy leaves the battle
  BATTLE_EVENT_DJINN_BONUS = 13,    // later defeats give the Djinn bonus
  BATTLE_EVENT_SOUND = 14           // plays a sound
};

#endif
