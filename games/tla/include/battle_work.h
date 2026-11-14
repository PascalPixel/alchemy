#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle/battle_event.h"
#include "system/ram_buffer.h"
#include "battle/battle_action.h"

/// The battle event queue and the state of its playback.
struct BattleEventState {
  struct BattleEventQueue queue;
  s32 phase;       ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex;  ///< the next queued event to play
  s32 timer;       ///< frames to wait
  u8 unk154[0x14]; ///< not named by TLA code
  s16 pendingCue;  ///< hit sound, played on the next hit
  s16 unk16a;      ///< while nonzero, a resumed phase keeps pendingCue
  s32 flags;       ///< cleared when playback starts
};

STATIC_ASSERT(sizeof(struct BattleEventState) == 0x170);
STATIC_ASSERT(offsetof(struct BattleEventState, pendingCue) == 0x168);

/// The battle's own work: its enemies, units and their objects, the turn's
/// actions, the background palette and the events.
struct BattleWork {
  s32 formation;      ///< the encounter's enemy formation
  s16 spriteSlots[6]; ///< the unit in each enemy sprite slot, 0 when free
  // XXX: The Broken Seal's enemyKinds (u16[6]: the kinds of enemy, for the
  // letters that tell two of a kind apart); declared so,
  // BattleParty_ChooseReserve compiles otherwise. Its readers' code would
  // settle the type.
  u8 unk010[12];
  u32 nameLetterMasks[6];    ///< for each kind, the name letters already given
  s8 lastNameLetter[6];      ///< for each kind, the last name letter given
  u8 unusedAfterChannels[2]; ///< no code names it
  u16 firstDefeated;
  u16 defeatState;
  u8 enemyKindCount; ///< kinds of enemy listed, at most 5
  u8 partyStatus;    ///< how the party status window draws
  /// The formation's layout: 0 or 1 lists the enemies in the order they were
  /// drawn.
  u8 layout;
  /// bits: 1 a party unit was challenged
  u8 challengeFlag;
  u8 isLink;      ///< 1 in a link battle
  u8 firstStrike; ///< who strikes first: 1 the party, 2 the enemies
  u8 unk046;
  u8 unk047; ///< set with MsgAllResting (ability effect 81)
  /// For each party unit, its slot as a link opponent's enemy, minus 128;
  /// a mark for none.
  u8 opponentSlots[8];
  u8 linkSide;         ///< this console's multiplayer id
  u8 linkMisses;       ///< frames the link has been down
  u8 linkPaused;       ///< set when the link drops
  u8 paddingAfterLink; ///< up to the word arrowTiles starts on
  s32 arrowTiles; ///< resource entry of the advance arrow's tiles; -1 for none
  s16 partyUnits[7]; ///< ends with 0xff
  s16 enemyIds[7];   ///< ends with 0xff
  u8 placement[16];  ///< each unit's index in slots; 0xff for none
  struct BattleObj slots[14];
  struct BattleActionRecord actions[20]; ///< the actions chosen for the turn
  u8 unk42c[0x124];                      ///< not named by TLA code
  struct BattleSpoils spoils;            ///< what the battle has won so far
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
  u16 background; ///< the battle background's id
  u8 unk672[4];
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
STATIC_ASSERT(offsetof(struct BattleWork, isLink) == 0x044);
STATIC_ASSERT(offsetof(struct BattleWork, firstStrike) == 0x045);
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

struct LinkRuntimeState;

extern struct LinkRuntimeState *gBattleWork;

#endif
