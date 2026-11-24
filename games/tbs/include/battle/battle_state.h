#ifndef ALCHEMY_BATTLE_STATE_H
#define ALCHEMY_BATTLE_STATE_H

/// @file
/// BattleState, the battle's own state, and the battle message window's state.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action.h"
#include "battle/battle_action_run.h"
#include "battle/playback_queue.h"
#include "system/io_reg.h"

/// One OAM entry in a sprite list: the arrow that waits for the player to
/// advance a battle message. The playback pushes it each frame it waits
/// (Oam_AddSpriteAtDepth), and the three halfwords after the link are OAM
/// attributes 0 to 2 as bit-fields.
struct AdvanceArrowSprite {
  /// The depth slot's next entry, set by Oam_AddSpriteAtDepth.
  struct AdvanceArrowSprite *next;
  u16 y : 8;     ///< pixels; bobs with a sine of the frame count
  u16 flags : 8; ///< attribute 0's high byte: 0xa0, a tall 256-colour sprite
  u16 x : 9;     ///< pixels
  /// Attribute 1's flip and size bits; 0, which with the tall shape is 8 by
  /// 16 pixels.
  u16 size : 7;
  u16 tile : 10; ///< the arrow picture's VRAM tile, reloaded each frame
  /// Attribute 2's priority (bits 1..0) and palette (bits 5..2); 0 here.
  u16 palette : 6;
  /// OAM's fourth halfword, the affine parameter slot; cleared with the tile
  /// word each frame.
  u16 affineParam;
};

struct Window;

/// The battle message, as far as its offset in the window:
/// the advance arrow is drawn at the window's corner plus this offset.
struct BattleMessageOffset {
  struct Window *window; ///< Message.window; not named here
  u16 x;                 ///< 1/256 pixels
  u16 y;                 ///< 1/256 pixels
};

/// The battle message window: its window, its message and whether the
/// next message is marked.
struct BattleMessageState {
  struct Window *window;              ///< the message window
  struct BattleMessageOffset *offset; ///< its message
  s32 marked; ///< set by PLAYBACK_MARK; passed to the next message
};

STATIC_ASSERT(sizeof(struct BattleMessageOffset) == 8);
STATIC_ASSERT(sizeof(struct BattleMessageState) == 12);
STATIC_ASSERT(offsetof(struct BattleMessageState, marked) == 8);

/// The battle message window's state, in HEAP_SLOT_BATTLE_MESSAGE, which
/// Battle_Run takes and zeroes for the battle.
#define gBattleMessageState                                                    \
  (*(struct BattleMessageState **)&gHeapSlots[HEAP_SLOT_BATTLE_MESSAGE])

/// The playback queue and the state of its playback: the phase, the entry
/// playing and the advance arrow.
struct BattlePlaybackState {
  struct BattlePlaybackQueue queue; ///< the action's playback, in play order
  s32 phase;      ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex; ///< the next queued entry to play
  s32 timer;      ///< frames to wait, or frames waited for the advance key
  struct AdvanceArrowSprite arrow; ///< the advance arrow
  s32 arrowResource; ///< the arrow's resource entry; -1 until it is shown
  s32 unitId;        ///< the unit of the hit or defeat being played
  s32 pendingSound;  ///< hit sound (an SE_ id), played on the next hit
  /// bits: 0 Djinn bonus (PLAYBACK_FLAG_DJINN_BONUS), for the enemies that fall
  /// during the action
  s32 flags;
};

/// The most actions a turn's queue holds.
enum { BATTLE_ACTION_CAPACITY = 20 };

/// The battle background's colours, BG palettes 6 to 13.
#define BATTLE_BACKGROUND_COLORS (8 * PALETTE_COLORS)
/// The battle background's darkening step (BattleBackground_SetPaletteLevel,
/// BattleBackground_Load, the element glow and the battle effects' fades): each
/// level takes a sixtieth of the full 16.16 brightness, so level 60 is almost
/// black.
#define BACKGROUND_LEVEL_STEP (FIXED_ONE / 60)

/// The frames a link battle tolerates without a connection before it gives
/// up: a count above it pauses the battle (BattleState.linkPaused) or fails
/// the sync, transfer or wait for the partner's commands under way.
#define LINK_MAX_MISSED_FRAMES 24

/// BattleState.challengeFlag's bit: the challenge effect landed on a party
/// member ("This is no time to be running away!").
#define BATTLE_CHALLENGED_PARTY 2

/// The battle's own state: its enemies, units and their battle objects, the
/// turn's actions, the spoils, the background palette, the action under way and
/// the playback.
///
/// It lives in the HEAP_SLOT_BATTLE heap slot: Battle_Run takes and
/// zeroes it, sets arrowTiles to -1 and formation, and the next scene change
/// clears the heap. Every battle part reads it through gBattleState, and the
/// battle effects read its background id and palette.
struct BattleState {
  s32 formation;      ///< the encounter's enemy formation
  s16 spriteSlots[6]; ///< the unit in each enemy sprite slot, 0 when free
  /// The enemies met so far, for their name letters, and the first
  /// enemy to fall (firstDefeated).
  u16 enemyKinds[6];
  /// Name letters in use by each enemy in enemyKinds, a bit per letter.
  u32 nameLetterMasks[6];
  /// The letter each enemy in enemyKinds gave out last; -1 for none yet.
  s8 lastNameLetter[6];
  u8 unusedAfterNameLetters[2]; ///< no code names it
  /// The enemyKinds index of the first enemy to fall; 6 for none.
  u16 firstDefeated;
  u16 defeatState;   ///< DEFEAT_ONE, DEFEAT_SEVERAL or DEFEAT_BY_PARTY_MEMBER
  u8 enemyKindCount; ///< enemies listed in enemyKinds, at most 5
  u8 partyStatus;    ///< PARTY_STATUS_SUMMONS or PARTY_STATUS_NO_SUMMONS; the
                     ///< staging draws it less PARTY_STATUS_SHOW_PP, names and
                     ///< HP only
  u8 layout; ///< the formation's FORMATION_LAYOUT_; SHUFFLED in a link battle
  /// BATTLE_CHALLENGED_PARTY once a party member was challenged; while
  /// nonzero, the party cannot run.
  u8 challengeFlag;
  u8 isLink;      ///< 1 in a link battle
  u8 firstStrike; ///< enum BattleFirstStrike: who strikes first
  /// Failed attempts to run; each adds 2000 to the base chance of 5000.
  u8 fleeFailures;
  u8 unusedAfterFleeFailures; ///< no code names it
  /// For each party unit, its slot as a link opponent's enemy, minus
  /// FIRST_ENEMY_UNIT; 0xff for none.
  u8 opponentSlots[8];
  u8 linkSide;         ///< this console's multiplayer id
  u8 linkMisses;       ///< frames the link has been down
  u8 linkPaused;       ///< set when the link drops or the console's id changes
  u8 paddingAfterLink; ///< up to the word arrowTiles starts on
  s32 arrowTiles; ///< resource entry of the advance arrow's tiles; -1 for none
  s16 partyUnits[7];                   ///< ends with BATTLE_UNIT_LIST_END
  s16 enemyIds[ENEMY_BATTLE_SIZE + 1]; ///< ends with BATTLE_UNIT_LIST_END
  /// The units' battle objects, reached through placement (BattleObject_Get).
  struct BattleObject objects[UNIT_MAX];
  /// Each unit's index in slots, indexed by party unit id (0 to 7), or 8 + n
  /// for enemy unit FIRST_ENEMY_UNIT + n: a party unit's place in the active
  /// party, an enemy's own 8 + n; 0xff for a party unit with no battle object.
  u8 objectIndex[UNIT_MAX];
  u8 unusedAfterPlacement[2]; ///< no code names it
  /// The actions chosen for the turn.
  struct BattleAction actions[BATTLE_ACTION_CAPACITY];
  u8 unusedAfterActions[260]; ///< no code names it
  /// What the defeated enemies leave; spoils.defeated, their count, is also
  /// what Battle_Run returns after a win or a broken link.
  struct BattleSpoils spoils;
  /// the battle background's colours, before scaling
  u16 palette[BATTLE_BACKGROUND_COLORS];
  /// the battle background's brightness, 16.16, FIXED_ONE full
  FIXED brightness;
  u16 background;              ///< the battle background's id
  u8 unusedAfterBackground[4]; ///< no code names it
  u16 highlightStep;           ///< index into gBattleHighlight_BlendRamp
  u16 highlightMode;           ///< nonzero while the units are highlighted
  u8 paddingAfterHighlight[2]; ///< up to the word actionRun starts on
  /// The action being carried out, as BattleActionRun_Build lays it out.
  struct BattleActionRun actionRun;
  struct BattlePlaybackState playback;
  /// A value Battle_AdjustDebugValue edits with the d-pad while
  /// Start is held.
  s32 debugValue;
};

STATIC_ASSERT(sizeof(struct BattleState) == 2092);

STATIC_ASSERT(offsetof(struct BattleState, partyStatus) == 65);
STATIC_ASSERT(offsetof(struct BattleState, challengeFlag) == 67);
STATIC_ASSERT(offsetof(struct BattleState, partyUnits) == 88);
STATIC_ASSERT(offsetof(struct BattleState, enemyIds) == 102);
STATIC_ASSERT(offsetof(struct BattleState, objects) == 116);
STATIC_ASSERT(offsetof(struct BattleState, objectIndex) == 732);
STATIC_ASSERT(offsetof(struct BattleState, actions) == 748);
STATIC_ASSERT(offsetof(struct BattleState, spoils) == 1328);
STATIC_ASSERT(offsetof(struct BattleState, palette) == 1348);
STATIC_ASSERT(offsetof(struct BattleState, brightness) == 1604);
STATIC_ASSERT(offsetof(struct BattleState, background) == 1608);
STATIC_ASSERT(offsetof(struct BattleState, actionRun) == 1620);
STATIC_ASSERT(offsetof(struct BattleState, playback) == 1720);
STATIC_ASSERT(offsetof(struct BattlePlaybackState, phase) == 328);
STATIC_ASSERT(offsetof(struct BattlePlaybackState, flags) == 364);

/// The battle's state: gHeapSlots[HEAP_SLOT_BATTLE] under its own name.
#define gBattleState (*(struct BattleState **)&gHeapSlots[HEAP_SLOT_BATTLE])

#endif
