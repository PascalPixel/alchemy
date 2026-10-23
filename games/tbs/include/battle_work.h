#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

/// @file
/// BattleWork, the battle's own work, and the battle display work.

#include "unit/battle/battle.h"
#include "types.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_event.h"
#include "battle_session.h"

/// One OAM entry in a sprite list: the arrow that waits for the player to
/// advance a battle message. The playback pushes it each frame it waits
/// (Runtime_PushSlotEntry), and the three halfwords after the link are OAM
/// attributes 0 to 2 as bit-fields.
struct AdvanceArrowSprite {
  /// The depth slot's next entry, set by Runtime_PushSlotEntry.
  struct AdvanceArrowSprite *next;
  u16 y : 8;     ///< pixels; bobs with a sine of the frame count
  u16 flags : 8; ///< attribute 0's high byte: 0xa0, a tall 256-colour sprite
  u16 x : 9;     ///< pixels
  /// Attribute 1's flip and size bits; 0, which with the tall shape is 8 by
  /// 16 pixels.
  u16 size : 7;
  u16 tile : 10; ///< the arrow frame's VRAM tile, reloaded each frame
  /// Attribute 2's priority (bits 1..0) and palette (bits 5..2); 0 here.
  u16 palette : 6;
  /// OAM's fourth halfword, the affine parameter slot; cleared with the tile
  /// word each frame.
  u16 affineParam;
};

struct UiWindow;

/// The battle message's text channel, as far as its offset in the window:
/// the advance arrow is drawn at the window's corner plus this offset.
struct BattleMessageOffset {
  struct UiWindow *window; ///< UiChannelSlot.work; not named here
  u16 x;                   ///< 1/256 pixels
  u16 y;                   ///< 1/256 pixels
};

/// The battle message display: its window, its text channel and whether the
/// next message is marked.
struct BattleMessageWork {
  struct UiWindow *window;            ///< the message window
  struct BattleMessageOffset *offset; ///< its text channel
  s32 marked; ///< set by PLAYBACK_MARK; passed to the next message
};

STATIC_ASSERT(sizeof(struct BattleMessageOffset) == 8);
STATIC_ASSERT(sizeof(struct BattleMessageWork) == 12);
STATIC_ASSERT(offsetof(struct BattleMessageWork, marked) == 8);

/// The battle message display, in HEAP_SLOT_BATTLE_MESSAGE, which
/// Battle_Run takes and zeroes for the battle.
extern struct BattleMessageWork *gBattleMessageWork;

/// The battle event queue and the state of its playback: the phase, the event
/// playing and the advance arrow.
struct BattlePlaybackWork {
  struct BattlePlaybackQueue queue; ///< the action's events, in play order
  s32 phase;      ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex; ///< the next queued event to play
  s32 timer;      ///< frames to wait, or frames waited for the advance key
  struct AdvanceArrowSprite arrow; ///< the advance arrow
  s32 arrowResource; ///< the arrow's resource entry; -1 until it is shown
  s32 actorId;       ///< the unit of the hit or defeat being played
  s32 pendingSong;   ///< hit sound (an SE_ id), played on the next hit
  /// bits: 0 Djinn bonus (PLAYBACK_FLAG_DJINN_BONUS), for the enemies that fall
  /// during the action
  s32 flags;
};

/// Six enemies at most, and a turn's queue of up to 20 actions.
enum { BATTLE_ENEMY_CAPACITY = 6, BATTLE_ACTION_CAPACITY = 20 };

/// The battle's own work: its enemies, units and their objects, the turn's
/// actions, the spoils, the background palette, the plan and the events.
///
/// It lives in the HEAP_SLOT_BATTLE work slot: Battle_Run takes and
/// zeroes it, sets arrowTiles to -1 and enemyGroup, and the next map change
/// clears the heap. Every battle part reads it through gBattleWork, and the
/// battle effects read its background id and palette.
struct BattleWork {
  s32 formation;      ///< the encounter's enemy formation
  s16 spriteSlots[6]; ///< the unit in each enemy sprite slot, 0 when free
  /// The enemy classes met so far, for their name letters, and the first
  /// class to fall (firstDefeated).
  u16 enemyKinds[6];
  /// Name letters in use by each class in enemyKinds, a bit per letter.
  u32 nameLetterMasks[6];
  /// The letter each class in enemyKinds gave out last; -1 for none yet.
  s8 lastNameLetter[6];
  u8 unusedAfterChannels[2]; ///< no code names it
  /// The enemyKinds index of the first class to fall; 6 for none.
  u16 firstDefeated;
  u16 defeatState;   ///< DEFEAT_ONE, DEFEAT_SEVERAL or DEFEAT_BY_PARTY_MEMBER
  u8 enemyKindCount; ///< classes listed in enemyKinds, at most 5
  u8 partyStatus;    ///< PARTY_STATUS_SUMMONS or PARTY_STATUS_NO_SUMMONS; the
                     ///< staging draws it less PARTY_STATUS_SHOW_PP, names and
                     ///< HP only
  u8 layout; ///< FORMATION_LAYOUT_SHUFFLED or FORMATION_LAYOUT_INTERLEAVED
  /// bits: 1 a party unit was challenged; nonzero, the party cannot run
  u8 challengeFlag;
  u8 isLink;      ///< 1 in a link battle
  u8 firstStrike; ///< BATTLE_ENCOUNTER_: who strikes first
  /// Failed attempts to run; each adds 2000 to the base chance of 5000.
  u8 fleeFailures;
  u8 unusedAfterEscapes; ///< no code names it
  /// For each party unit, its slot as a link opponent's enemy, minus
  /// FIRST_ENEMY_UNIT; 0xff for none.
  u8 opponentSlots[8];
  u8 linkSide;         ///< this console's multiplayer id
  u8 linkMisses;       ///< frames the link has been down
  u8 linkPaused;       ///< set when the link drops or the console's id changes
  u8 paddingAfterLink; ///< up to the word arrowTiles starts on
  s32 arrowTiles; ///< resource entry of the advance arrow's tiles; -1 for none
  s16 partyUnits[7];                       ///< ends with BATTLE_UNIT_LIST_END
  s16 enemyIds[BATTLE_ENEMY_CAPACITY + 1]; ///< ends with BATTLE_UNIT_LIST_END
  /// The units' battle objects, reached through placement (BattleObject_Get).
  struct BattleObject objects[14];
  /// Each unit's index in slots, indexed by party unit id (0 to 7), or 8 + n
  /// for enemy unit 0x80 + n: a party unit's place in the active party, an
  /// enemy's own 8 + n; 0xff for a party unit with no object.
  u8 objectIndex[14];
  u8 unusedAfterPlacement[2]; ///< no code names it
  /// The actions chosen for the turn.
  struct BattleAction actions[BATTLE_ACTION_CAPACITY];
  u8 unusedAfterActions[0x104]; ///< no code names it
  /// What the defeated enemies leave; spoils.defeated, their count, is also
  /// what Battle_Run returns after a win or a broken link.
  struct BattleSpoils spoils;
  u16 palette[128]; ///< the backdrop's colours, before scaling
  s32 brightness;   ///< the backdrop's brightness, 16.16, 0x10000 full
  u16 background;   ///< the battle background's id
  u8 unusedAfterBackground[4]; ///< no code names it
  u16 highlightStep;           ///< index into gBattleHighlight_BlendRamp
  u16 highlightMode;           ///< nonzero while the units are highlighted
  u8 paddingAfterHighlight[2]; ///< up to the word plan starts on
  /// The action being carried out, as BattleTurn_Build lays it out.
  struct BattleTurn turn;
  struct BattlePlaybackWork playback;
  /// A value Runtime_AdjustDebugValueWithButtons edits with the d-pad while
  /// Start is held.
  s32 debugValue;
};

STATIC_ASSERT(sizeof(struct BattleWork) == 0x82c);

STATIC_ASSERT(offsetof(struct BattleWork, partyStatus) == 0x041);
STATIC_ASSERT(offsetof(struct BattleWork, challengeFlag) == 0x043);
STATIC_ASSERT(offsetof(struct BattleWork, partyUnits) == 0x058);
STATIC_ASSERT(offsetof(struct BattleWork, enemyIds) == 0x066);
STATIC_ASSERT(offsetof(struct BattleWork, objects) == 0x074);
STATIC_ASSERT(offsetof(struct BattleWork, objectIndex) == 0x2dc);
STATIC_ASSERT(offsetof(struct BattleWork, actions) == 0x2ec);
STATIC_ASSERT(offsetof(struct BattleWork, spoils) == 0x530);
STATIC_ASSERT(offsetof(struct BattleWork, palette) == 0x544);
STATIC_ASSERT(offsetof(struct BattleWork, brightness) == 0x644);
STATIC_ASSERT(offsetof(struct BattleWork, background) == 0x648);
STATIC_ASSERT(offsetof(struct BattleWork, turn) == 0x654);
STATIC_ASSERT(offsetof(struct BattleWork, playback) == 0x6b8);
STATIC_ASSERT(offsetof(struct BattlePlaybackWork, phase) == 0x148);
STATIC_ASSERT(offsetof(struct BattlePlaybackWork, flags) == 0x16c);

/// The battle's work: gHeapSlots[HEAP_SLOT_BATTLE] under its own name.
extern struct BattleWork *gBattleWork;

#endif
