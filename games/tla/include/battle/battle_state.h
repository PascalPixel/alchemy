#ifndef ALCHEMY_BATTLE_STATE_H
#define ALCHEMY_BATTLE_STATE_H

/// @file
/// The battle's own state: its units and their places, the turn's actions, the
/// spoils, the background's palette and the playback of an action's events.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "system/io_reg.h"
#include "battle/battle_type.h"
#include "battle/battle_event.h"
#include "system/ram_buffer.h"
#include "battle/battle_action.h"

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

/// The battle event queue and the state of its playback.
struct BattlePlaybackState {
  struct BattlePlaybackQueue queue;
  s32 phase;      ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex; ///< the next queued event to play
  s32 timer;      ///< frames to wait
  struct AdvanceArrowSprite arrow; ///< the advance arrow
  s32 arrowResource;    ///< the arrow's resource entry; -1 until it is shown
  s32 unitId;           ///< the unit of the hit or defeat being played
  s16 pendingSound;     ///< hit sound (an SE_ id), played on the next hit
  s16 keepPendingSound; ///< while nonzero, a resumed phase keeps pendingSound
  /// Set by PLAYBACK_DJINN_BONUS: enemies felled from then on give the Djinn
  /// bonus, cycling their colours and crying as they fall.
  s32 flags;
};

STATIC_ASSERT(sizeof(struct BattlePlaybackState) == 368);
STATIC_ASSERT(offsetof(struct BattlePlaybackState, pendingSound) == 360);

/// The most actions a turn's queue holds.
enum { BATTLE_ACTION_CAPACITY = 20 };

/// The battle background's colours, BG palettes 6 to 13.
#define BATTLE_BACKGROUND_COLORS (8 * PALETTE_COLORS)
/// The battle background's darkening step (BattleBackground_SetPaletteLevel,
/// BattleBackground_Load, the element glow and the battle effects' fades): each
/// level takes a sixtieth of the full 16.16 brightness, so level 60 is black.
#define BACKGROUND_LEVEL_STEP (FIXED_ONE / 60)

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

/// BattleState.opponentSlots of a party unit with no slot as a link opponent.
#define OPPONENT_SLOT_NONE 255

/// The frames a link battle tolerates without a connection before it gives
/// up: a count above it pauses the battle (BattleState.linkPaused) or fails
/// the sync or transfer under way.
#define LINK_MAX_MISSED_FRAMES 24

/// The battle's own state: its enemies, units and their objects, the turn's
/// actions, the background palette and the events.
struct BattleState {
  s32 formation;      ///< the encounter's enemy formation
  s16 spriteSlots[6]; ///< the unit in each enemy sprite slot, 0 when free
  /// The kinds of enemy in the formation, for the letters that tell two of a
  /// kind apart: five at most, where The Broken Seal keeps six.
  u16 enemyKinds[5];
  u16 kindsEnd;           ///< read only by the defeat search, as a sixth kind
  u32 nameLetterMasks[6]; ///< for each kind, the name letters already given
  s8 lastNameLetter[6];   ///< for each kind, the last name letter given
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
  /// Failed tries to flee this battle; each adds 2000 to the chance
  /// (BattleFlee_CheckSuccess).
  u8 fleeFailures;
  u8 ceaseFire; ///< set by Lull with MsgAllResting, "Everybody is resting!"
  /// For each party unit, its slot as a link opponent's enemy, minus 128;
  /// OPPONENT_SLOT_NONE for none.
  u8 opponentSlots[8];
  u8 linkSide;         ///< this console's multiplayer id
  u8 linkMisses;       ///< frames the link has been down
  u8 linkPaused;       ///< set when the link drops
  u8 paddingAfterLink; ///< up to the word arrowTiles starts on
  s32 arrowTiles; ///< resource entry of the advance arrow's tiles; -1 for none
  s16 partyUnits[7]; ///< ends with BATTLE_UNIT_LIST_END
  s16 enemyIds[7];   ///< ends with BATTLE_UNIT_LIST_END
  /// Each unit's index in slots, party ids 0 to 7 then the enemies; 255 for
  /// none (BATTLE_OBJECT_NONE).
  u8 placement[16];
  struct BattleObject slots[14];
  struct BattleActionRecord
      actions[BATTLE_ACTION_CAPACITY];   ///< the actions chosen for the turn
  u8 unusedAfterActions[292];            ///< no code names it
  struct BattleSpoils spoils;            ///< what the battle has won so far
  u16 palette[BATTLE_BACKGROUND_COLORS]; ///< the backdrop's colours, before
                                         ///< scaling
  FIXED brightness; ///< the backdrop's brightness, 16.16, FIXED_ONE full
  /// Its first halfword is the battle background passed to the palette fade.
  u16 background;              ///< the battle background's id
  u8 unusedAfterBackground[4]; ///< no code names it
  u16 highlightStep; ///< stepped each frame while the units are highlighted
  u16 highlightMode; ///< nonzero while the units are highlighted
  u8 paddingAfterHighlight[2]; ///< up to the word plan starts on
  /// The action being carried out, as BattleActionRun_Build lays it out.
  struct BattlePlan plan;
  s32 skipPlayback; ///< nonzero leaves the action's events unplayed
  struct BattlePlaybackState events;
  /// A value Battle_AdjustDebugValue edits with the d-pad while Start is held.
  s32 debugValue;
  /// The unit ready to counterattack (ABILITY_EFFECT_COUNTER) that was just
  /// struck, and the opponent that struck it.
  ///@{
  s32 counterUnitId;
  s32 counterTargetId;
  ///@}
  /// Daedalus strikes again at the end of the turns after it is summoned:
  /// each queued strike is a bit, set when an enemy summoned it, and the next
  /// turn's queue is gathered while this one's is played.
  ///@{
  u16 daedalusFromEnemies;
  u16 nextDaedalusFromEnemies;
  u8 daedalusStrikes;
  u8 nextDaedalusStrikes;
  ///@}
  /// Nonzero leaves the battle objects' sprite priorities as they are, while
  /// Flora's effect plays.
  u8 keepObjectPriorities;
  /// Nonzero keeps the backdrop's palette while a summon (Charon, Atalanta,
  /// Neptune, Moloch) colours the screen.
  u8 keepBackdropPalette;
  /// Poseidon's force field: 1 for an enemy of id 101 to 103 while flag 0x173
  /// is clear; until the Trident lands, MsgFieldProtects turns abilities away.
  u8 forceField;
  /// The animation the acting unit takes as it calls up its Psynergy; 3 as
  /// its action starts.
  u8 actingAnimation;
  u8 paddingAfterActingAnimation[2]; ///< up to the word aiStates starts on
  /// The action choice's memory for the enemies that act more than once a
  /// turn.
  struct BattleAIState aiStates[4];
};

STATIC_ASSERT(offsetof(struct BattleState, partyStatus) == 65);
STATIC_ASSERT(offsetof(struct BattleState, challengeFlag) == 67);
STATIC_ASSERT(offsetof(struct BattleState, isLink) == 68);
STATIC_ASSERT(offsetof(struct BattleState, firstStrike) == 69);
STATIC_ASSERT(offsetof(struct BattleState, partyUnits) == 88);
STATIC_ASSERT(offsetof(struct BattleState, enemyIds) == 102);
STATIC_ASSERT(offsetof(struct BattleState, placement) == 116);
STATIC_ASSERT(offsetof(struct BattleState, slots) == 132);
STATIC_ASSERT(offsetof(struct BattleState, actions) == 748);
STATIC_ASSERT(offsetof(struct BattleState, spoils) == 1360);
STATIC_ASSERT(offsetof(struct BattleState, spoils.queuedSummons) == 1380);
STATIC_ASSERT(offsetof(struct BattleState, spoils.queueCount) == 1387);
STATIC_ASSERT(offsetof(struct BattleState, highlightStep) == 1654);
STATIC_ASSERT(offsetof(struct BattleState, highlightMode) == 1656);
STATIC_ASSERT(offsetof(struct BattleState, events) == 1764);
STATIC_ASSERT(offsetof(struct BattleState, plan) == 1660);
STATIC_ASSERT(offsetof(struct BattleState, debugValue) == 2132);
STATIC_ASSERT(offsetof(struct BattleState, daedalusFromEnemies) == 2144);
STATIC_ASSERT(offsetof(struct BattleState, keepBackdropPalette) == 2151);
STATIC_ASSERT(offsetof(struct BattleState, forceField) == 2152);
STATIC_ASSERT(offsetof(struct BattleState, aiStates) == 2156);
STATIC_ASSERT(sizeof(struct BattleState) == 2172);

#endif
