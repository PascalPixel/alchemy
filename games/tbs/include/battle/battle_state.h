#ifndef ALCHEMY_BATTLE_STATE_H
#define ALCHEMY_BATTLE_STATE_H

// BATTLE_STATE, the battle's own state, and the battle message window's state.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action.h"
#include "battle/battle_action_run.h"
#include "battle/playback_queue.h"
#include "system.h"

// One OAM entry in a sprite list: the arrow that waits for the player to advance a battle message.
// The playback pushes it each tick it waits (syOAMAddSpriteAtDepth), and the three halfwords after
// the link are OAM attributes 0 to 2 as bit-fields.
typedef struct ADVANCE_ARROW_SPR
{
	// The depth slot's next entry, set by syOAMAddSpriteAtDepth.
	struct ADVANCE_ARROW_SPR *next;
	Uint16                    y       : 8; // pixels; bobs with a sine of the tick count
	// attribute 0's high byte: 0xa0, a tall 256-colour sprite
	Uint16                    flags   : 8;
	Uint16                    x       : 9; // pixels
	// Attribute 1's flip and size bits; 0, which with the tall shape is 8 by 16 pixels.
	Uint16                    size    : 7;
	Uint16                    tile    : 10; // the arrow picture's VRAM tile, reloaded each tick
	// Attribute 2's priority (bits 1..0) and palette (bits 5..2); 0 here.
	Uint16                    palette : 6;
	// OAM's fourth halfword, the affine parameter slot; cleared with the tile word each tick.
	Uint16                    affineParam;
} ADVANCE_ARROW_SPR;

struct WINDOW;

// The battle message, as far as its offset in the window: the advance arrow is drawn at the
// window's corner plus this offset.
typedef struct BATTLE_MESSAGE_OFFSET
{
	struct WINDOW *window; // MESSAGE.window; not named here
	Uint16         x;      // 1/256 pixels
	Uint16         y;      // 1/256 pixels
} BATTLE_MESSAGE_OFFSET;

// The battle message window: its window, its message and whether the next message is marked.
typedef struct BATTLE_MESSAGE_STATE
{
	struct WINDOW         *window; // the message window
	BATTLE_MESSAGE_OFFSET *offset; // its message
	Sint32                 marked; // set by PLAYBACK_MARK; passed to the next message
} BATTLE_MESSAGE_STATE;

STATIC_ASSERT(sizeof(BATTLE_MESSAGE_OFFSET) == 8);
STATIC_ASSERT(sizeof(BATTLE_MESSAGE_STATE) == 12);
STATIC_ASSERT(offsetof(BATTLE_MESSAGE_STATE, marked) == 8);

// The battle message window's state, in HEAP_SLOT_BATTLE_MESSAGE, which btRun takes and zeroes for
// the battle.
#define BattleMessageStateWork (*(BATTLE_MESSAGE_STATE **)&HeapSlots[HEAP_SLOT_BATTLE_MESSAGE])

// The playback queue and the state of its playback: the phase, the entry playing and the advance
// arrow.
typedef struct BATTLE_PLAYBACK_STATE
{
	BATTLE_PLAYBACK_QUEUE queue;         // the action's playback, in play order
	Sint32                phase;         // BATTLE_PLAYBACK_PHASE, or a message phase
	Sint32                eventIndex;    // the next queued entry to play
	Sint32                timer;         // ticks to wait, or ticks waited for the advance key
	ADVANCE_ARROW_SPR     arrow;         // the advance arrow
	Sint32                arrowResource; // the arrow's resource entry; -1 until it is shown
	Sint32                unitId;        // the unit of the hit or defeat being played
	Sint32                pendingSound;  // hit sound (an SE_ id), played on the next hit
	// PLAYBACK_FLAG_DJINN_BONUS: the Djinn bonus, for the enemies that fall during the action
	Sint32                djinnBonus;
} BATTLE_PLAYBACK_STATE;

// The most actions a turn's queue holds.
enum
{
	BATTLE_ACTION_CAPACITY = 20
};

// The battle background's colours, BG palettes 6 to 13.
#define BATTLE_BACKGROUND_COLORS (8 * PALETTE_COLORS)
// The battle background's darkening step (btBackgroundSetPaletteLevel, btBackgroundLoad, the
// element glow and the battle effects' fades): each level takes a sixtieth of the full 16.16
// brightness, so level 60 is almost black.
#define BACKGROUND_LEVEL_STEP    (toFIXED(1) / 60)

// The ticks a link battle tolerates without a connection before it gives up: a count above it
// pauses the battle (BATTLE_STATE.linkPaused) or fails the sync, transfer or wait for the partner's
// commands under way.
#define LINK_MAX_MISSED_TICKS    24

// BATTLE_STATE.challengeFlag's bit: the challenge effect landed on a party member ("This is no time
// to be running away!").
#define BATTLE_CHALLENGED_PARTY  2

// The battle has six enemy sprite slots; a large enemy takes two side by side, a small one
// (btSpriteIsSmall) one.
#define ENEMY_SPRITE_SLOTS       6

// The battle's own state: its enemies, units and their battle objects, the turn's actions, the
// spoils, the background palette, the action under way and the playback.
//
// It lives in the HEAP_SLOT_BATTLE heap slot: btRun takes and zeroes it, sets arrowTiles to -1 and
// formation, and the next scene change clears the heap. Every battle part reads it through
// BattleStateWork, and the battle effects read its background id and palette.
typedef struct BATTLE_STATE
{
	Sint32 formation; // the encounter's enemy formation
	// The unit in each enemy sprite slot, 0 when free.
	Sint16 spriteSlots[ENEMY_SPRITE_SLOTS];
	// The enemies met so far, for their name letters, and the first enemy to fall (firstDefeated).
	Uint16 enemyKinds[6];
	// Name letters in use by each enemy in enemyKinds, a bit per letter.
	Uint32 nameLetterMasks[6];
	// The letter each enemy in enemyKinds gave out last; -1 for none yet.
	Sint8  lastNameLetter[6];
	Uint8  unusedAfterNameLetters[2]; // no code names it
	// The enemyKinds index of the first enemy to fall; 6 for none.
	Uint16 firstDefeated;
	// DEFEAT_ONE, DEFEAT_SEVERAL or DEFEAT_BY_PARTY_MEMBER
	Uint16 defeatState;
	Uint8  enemyKindCount; // enemies listed in enemyKinds, at most 5
	Uint8  partyStatus;    // PARTY_STATUS_SUMMONS or PARTY_STATUS_NO_SUMMONS; the
	// staging draws it less PARTY_STATUS_SHOW_PP, names and HP only the formation's
	// FORMATION_LAYOUT_; SHUFFLED in a link battle
	Uint8  layout;
	// BATTLE_CHALLENGED_PARTY once a party member was challenged; while nonzero, the party cannot
	// run.
	Uint8  challengeFlag;
	Uint8  isLink;      // 1 in a link battle
	Uint8  firstStrike; // BATTLE_FIRST_STRIKE: who strikes first
	// Failed attempts to run; each adds 2000 to the base chance of 5000.
	Uint8  fleeFailures;
	Uint8  unusedAfterFleeFailures; // no code names it
	// For each party unit, its slot as a link opponent's enemy, minus FIRST_ENEMY_UNIT; 0xff for
	// none.
	Uint8  opponentSlots[8];
	Uint8  linkSide;         // this console's multiplayer id
	Uint8  linkMisses;       // ticks the link has been down
	Uint8  linkPaused;       // set when the link drops or the console's id changes
	Uint8  paddingAfterLink; // up to the word arrowTiles starts on
	Sint32
		// resource entry of the advance arrow's tiles; -1 for none
						  arrowTiles;
	Sint16                partyUnits[7];                   // ends with BATTLE_UNIT_LIST_END
	Sint16                enemyIds[ENEMY_BATTLE_SIZE + 1]; // ends with BATTLE_UNIT_LIST_END
	// The units' battle objects, reached through placement (btObjectGet).
	BATTLE_OBJECT         objects[UNIT_MAX];
	// Each unit's index in slots, indexed by party unit id (0 to 7), or 8 + n for enemy unit
	// FIRST_ENEMY_UNIT + n: a party unit's place in the active party, an enemy's own 8 + n; 0xff
	// for a party unit with no battle object.
	Uint8                 objectIndex[UNIT_MAX];
	Uint8                 unusedAfterPlacement[2]; // no code names it
	// The actions chosen for the turn.
	BATTLE_ACTION         actions[BATTLE_ACTION_CAPACITY];
	Uint8                 unusedAfterActions[260]; // no code names it
	// What the defeated enemies leave; spoils.defeated, their count, is also what btRun returns
	// after a win or a broken link.
	BATTLE_SPOILS         spoils;
	// the battle background's colours, before scaling
	Uint16                palette[BATTLE_BACKGROUND_COLORS];
	// the battle background's brightness, 16.16, toFIXED(1) full
	FIXED                 brightness;
	Uint16                bg;                       // the battle background's id
	Uint8                 unusedAfterBg[4];         // no code names it
	Uint16                highlightStep;            // index into BattleHighlightBlendRamp
	Uint16                highlightMode;            // nonzero while the units are highlighted
	Uint8                 paddingAfterHighlight[2]; // up to the word actionRun starts on
	// The action being carried out, as btActionRunBuild lays it out.
	BATTLE_ACTION_RUN     actionRun;
	BATTLE_PLAYBACK_STATE playback;
	// A value btAdjustDebugValue edits with the d-pad while Start is held.
	Sint32                debugValue;
} BATTLE_STATE;

STATIC_ASSERT(sizeof(BATTLE_STATE) == 2092);

STATIC_ASSERT(offsetof(BATTLE_STATE, partyStatus) == 65);
STATIC_ASSERT(offsetof(BATTLE_STATE, challengeFlag) == 67);
STATIC_ASSERT(offsetof(BATTLE_STATE, partyUnits) == 88);
STATIC_ASSERT(offsetof(BATTLE_STATE, enemyIds) == 102);
STATIC_ASSERT(offsetof(BATTLE_STATE, objects) == 116);
STATIC_ASSERT(offsetof(BATTLE_STATE, objectIndex) == 732);
STATIC_ASSERT(offsetof(BATTLE_STATE, actions) == 748);
STATIC_ASSERT(offsetof(BATTLE_STATE, spoils) == 1328);
STATIC_ASSERT(offsetof(BATTLE_STATE, palette) == 1348);
STATIC_ASSERT(offsetof(BATTLE_STATE, brightness) == 1604);
STATIC_ASSERT(offsetof(BATTLE_STATE, bg) == 1608);
STATIC_ASSERT(offsetof(BATTLE_STATE, actionRun) == 1620);
STATIC_ASSERT(offsetof(BATTLE_STATE, playback) == 1720);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_STATE, phase) == 328);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_STATE, djinnBonus) == 364);

// The battle's state: HeapSlots[HEAP_SLOT_BATTLE] under its own name.
#define BattleStateWork (*(BATTLE_STATE **)&HeapSlots[HEAP_SLOT_BATTLE])

#endif
