#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_event.h"
#include "ram_buffer.h"
#include "battle/battle_action.h"

/// The battle event queue and the state of its playback.
struct BattleEventState {
  struct BattleEventQueue queue;
  s32 phase;         ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex;    ///< the next queued event to play
  s32 timer;         ///< frames to wait
  u8 unk154[0x14];   ///< not named by TLA code
  s16 pendingCue;    ///< hit sound, played on the next hit
  s16 unk16a;        ///< while nonzero, a resumed phase keeps pendingCue
  s32 flags;         ///< cleared when playback starts
};

STATIC_ASSERT(sizeof(struct BattleEventState) == 0x170);
STATIC_ASSERT(offsetof(struct BattleEventState, pendingCue) == 0x168);

/// The battle's own work: its enemies, units and their objects, the turn's
/// actions, the background palette and the events.
struct BattleWork {
  s32 enemyGroup; ///< the encounter's enemy formation
  u8 unk004[0x3d]; ///< not named by TLA code
  u8 partyStatus;  ///< how the party status window draws
  /// The formation's layout, as The Broken Seal's battleType: 0 or 1 lists
  /// the enemies in the order they were drawn.
  u8 unk042;
  /// bits: 1 a party unit was challenged
  u8 challengeFlag;
  u8 twoSided;      ///< nonzero in a link battle
  u8 encounterMode; ///< who strikes first: 1 the party, 2 the enemies
  u8 unk046[2]; ///< [1] set with MsgAllResting (ability effect 81)
  /// For each party unit, its slot as a link opponent's enemy, minus 128;
  /// a mark for none.
  u8 ownerSlots[8];
  u8 linkSide;   ///< this console's multiplayer id
  u8 unk051;     ///< not named by TLA code
  u8 linkPaused; ///< set when the link drops
  u8 unk053[5];  ///< not named by TLA code
  s16 partyUnits[7]; ///< ends with 0xff
  s16 enemyIds[7];   ///< ends with 0xff
  u8 placement[16];  ///< each unit's index in slots; 0xff for none
  struct BattleObj slots[14];
  struct BattleActionRecord actions[20]; ///< the actions chosen for the turn
  u8 unk42c[0x124]; ///< not named by TLA code
  struct BattleSpoils spoils; ///< what the battle has won so far
  /// A queue of two enemies to call in (an ability of effect 87): their
  /// classes and name letters, its head and its length.
  ///@{
  u16 queuedSummons[2];
  u8 queuedCharges[2];
  s8 queueHead;
  s8 queueCount;
  ///@}
  u16 palette[128]; ///< the backdrop's colours, before scaling
  s32 brightness;   ///< the backdrop's brightness, 16.16, 0x10000 full
  /// Its first halfword is the battle background passed to the palette fade.
  u8 unk670[6];
  u16 highlightStep; ///< stepped each frame while the units are highlighted
  u16 highlightMode; ///< nonzero while the units are highlighted
  /// Scheduling the event playback clears its byte at 0x67d (written as
  /// unk670[0x0d]).
  u8 unk67a[0x6a];
  struct BattleEventState events;
  u8 unk854[4]; ///< not named by TLA code
  /// The target and the actor of a hit on a unit whose equipBonus[1] is set,
  /// as one draft stores them.
  ///@{
  s32 unk858;
  s32 unk85c;
  ///@}
  u8 unk860[8]; ///< not named by TLA code
  /// 1 for an enemy of id 101 to 103 while flag 0x173 is clear; one draft's
  /// ability check reads it.
  u8 unk868;
};

STATIC_ASSERT(offsetof(struct BattleWork, partyStatus) == 0x041);
STATIC_ASSERT(offsetof(struct BattleWork, challengeFlag) == 0x043);
STATIC_ASSERT(offsetof(struct BattleWork, twoSided) == 0x044);
STATIC_ASSERT(offsetof(struct BattleWork, encounterMode) == 0x045);
STATIC_ASSERT(offsetof(struct BattleWork, partyUnits) == 0x058);
STATIC_ASSERT(offsetof(struct BattleWork, enemyIds) == 0x066);
STATIC_ASSERT(offsetof(struct BattleWork, placement) == 0x074);
STATIC_ASSERT(offsetof(struct BattleWork, slots) == 0x084);
STATIC_ASSERT(offsetof(struct BattleWork, actions) == 0x2ec);
STATIC_ASSERT(offsetof(struct BattleWork, spoils) == 0x550);
STATIC_ASSERT(offsetof(struct BattleWork, queuedSummons) == 0x564);
STATIC_ASSERT(offsetof(struct BattleWork, queueCount) == 0x56b);
STATIC_ASSERT(offsetof(struct BattleWork, highlightStep) == 0x676);
STATIC_ASSERT(offsetof(struct BattleWork, highlightMode) == 0x678);
STATIC_ASSERT(offsetof(struct BattleWork, events) == 0x6e4);
STATIC_ASSERT(offsetof(struct BattleWork, unk868) == 0x868);

#endif
