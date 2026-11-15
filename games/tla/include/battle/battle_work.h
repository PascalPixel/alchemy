#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"
#include "static_assert.h"
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
struct BattlePlaybackWork {
  struct BattlePlaybackQueue queue;
  s32 phase;      ///< enum BattlePlaybackPhase, or a message phase
  s32 eventIndex; ///< the next queued event to play
  s32 timer;      ///< frames to wait
  struct AdvanceArrowSprite arrow; ///< the advance arrow
  s32 arrowResource;    ///< the arrow's resource entry; -1 until it is shown
  s32 unitId;           ///< the unit of the hit or defeat being played
  s16 pendingSound;     ///< hit sound (an SE_ id), played on the next hit
  s16 keepPendingSound; ///< while nonzero, a resumed phase keeps pendingSound
  s32 flags;            ///< cleared when playback starts
};

STATIC_ASSERT(sizeof(struct BattlePlaybackWork) == 0x170);
STATIC_ASSERT(offsetof(struct BattlePlaybackWork, pendingSound) == 0x168);

/// The most actions a turn's queue holds.
enum { BATTLE_ACTION_CAPACITY = 20 };

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
struct BattleMessageWork {
  struct Window *window;              ///< the message window
  struct BattleMessageOffset *offset; ///< its message
  s32 marked; ///< set by PLAYBACK_MARK; passed to the next message
};

STATIC_ASSERT(sizeof(struct BattleMessageOffset) == 8);
STATIC_ASSERT(sizeof(struct BattleMessageWork) == 12);

/// The battle's own work: its enemies, units and their objects, the turn's
/// actions, the background palette and the events.
struct BattleWork {
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
  struct BattleObject slots[14];
  struct BattleActionRecord
      actions[BATTLE_ACTION_CAPACITY]; ///< the actions chosen for the turn
  u8 unusedAfterActions[0x124];        ///< no code names it
  struct BattleSpoils spoils;          ///< what the battle has won so far
  u16 palette[128]; ///< the backdrop's colours, before scaling
  s32 brightness;   ///< the backdrop's brightness, 16.16, 0x10000 full
  /// Its first halfword is the battle background passed to the palette fade.
  u16 background;              ///< the battle background's id
  u8 unusedAfterBackground[4]; ///< no code names it
  u16 highlightStep; ///< stepped each frame while the units are highlighted
  u16 highlightMode; ///< nonzero while the units are highlighted
  u8 paddingAfterHighlight[2]; ///< up to the word plan starts on
  /// The action being carried out, as BattleActionRun_Build lays it out.
  struct BattlePlan plan;
  s32 skipPlayback; ///< nonzero leaves the action's events unplayed
  struct BattlePlaybackWork events;
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
  /// Each enemy's targeting memory (a struct AIState), by AI request.
  u32 aiStates[4];
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
STATIC_ASSERT(offsetof(struct BattleWork, spoils.queuedSummons) == 0x564);
STATIC_ASSERT(offsetof(struct BattleWork, spoils.queueCount) == 0x56b);
STATIC_ASSERT(offsetof(struct BattleWork, highlightStep) == 0x676);
STATIC_ASSERT(offsetof(struct BattleWork, highlightMode) == 0x678);
STATIC_ASSERT(offsetof(struct BattleWork, events) == 0x6e4);
STATIC_ASSERT(offsetof(struct BattleWork, plan) == 0x67c);
STATIC_ASSERT(offsetof(struct BattleWork, debugValue) == 0x854);
STATIC_ASSERT(offsetof(struct BattleWork, daedalusFromEnemies) == 0x860);
STATIC_ASSERT(offsetof(struct BattleWork, keepBackdropPalette) == 0x867);
STATIC_ASSERT(offsetof(struct BattleWork, forceField) == 0x868);
STATIC_ASSERT(offsetof(struct BattleWork, aiStates) == 0x86c);
STATIC_ASSERT(sizeof(struct BattleWork) == 0x87c);

struct LinkRuntimeState;

/// The battle's work: heap slot HEAP_SLOT_BATTLE's block under its own name.
extern struct LinkRuntimeState *gBattleWork;

#endif
