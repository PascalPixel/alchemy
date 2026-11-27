#ifndef ALCHEMY_BATTLE_STATE_H
#define ALCHEMY_BATTLE_STATE_H

// The battle's own state: its units and their places, the turn's actions, the spoils, the
// background's palette and the playback of an action's events.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"
#include "battle/battle_type.h"
#include "battle/battle_event.h"
#include "battle/battle_action.h"
#include "window/menu_list.h"

// The battle event queue and the state of its playback.
typedef struct BATTLE_PLAYBACK_STATE
{
    BATTLE_PLAYBACK_QUEUE queue;
    Sint32                phase;         // BATTLE_PLAYBACK_PHASE, or a message phase
    Sint32                eventIndex;    // the next queued event to play
    Sint32                timer;         // ticks to wait
    MENU_SPR              arrow;         // the advance arrow
    Sint32                arrowResource; // the arrow's resource entry; -1 until it is shown
    Sint32                unitId;        // the unit of the hit or defeat being played
    Sint16                pendingSound;  // hit sound (an SE_ id), played on the next hit
    // while nonzero, a resumed phase keeps pendingSound
    Sint16                keepPendingSound;
    // Set by PLAYBACK_DJINN_BONUS: enemies felled from then on give the Djinn bonus, cycling their
    // colours and crying as they fall.
    Sint32                djinnBonus;
} BATTLE_PLAYBACK_STATE;

STATIC_ASSERT(sizeof(BATTLE_PLAYBACK_STATE) == 368);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_STATE, pendingSound) == 360);

// The most actions a turn's queue holds.
enum
{
    BATTLE_ACTION_CAPACITY = 20
};

// The battle background's colours, BG palettes 6 to 13.
#define BATTLE_BACKGROUND_COLORS (8 * PALETTE_COLORS)
// The battle background's darkening step (btBackgroundSetPaletteLevel, btBackgroundLoad, the
// element glow and the battle effects' fades): each level takes a sixtieth of the full 16.16
// brightness, so level 60 is black.
#define BACKGROUND_LEVEL_STEP    (toFIXED(1) / 60)

struct WINDOW;
struct MESSAGE;

// The battle message window (heap slot battleMsg): its window, its message and whether the next
// message is marked.
typedef struct BATTLE_MESSAGE_STATE
{
    struct WINDOW  *window; // the message window
    // Its message, as mnBattleMessageContinue returned it: the advance arrow is drawn at the
    // window's corner plus the message's pen.
    struct MESSAGE *msg;
    // Set by PLAYBACK_MARK; passed to the next message, which then keeps its place
    // (mnBattleMessageContinue's keepPosition).
    Sint32          marked;
} BATTLE_MESSAGE_STATE;

STATIC_ASSERT(sizeof(BATTLE_MESSAGE_STATE) == 12);

// BATTLE_STATE.opponentSlots of a party unit with no slot as a link opponent.
#define OPPONENT_SLOT_NONE    255

// The ticks a link battle tolerates without a connection before it gives up: a count above it
// pauses the battle (BATTLE_STATE.linkPaused) or fails the sync or transfer under way.
#define LINK_MAX_MISSED_TICKS 24

// The battle's own state: its enemies, units and their objects, the turn's actions, the background
// palette and the events.
typedef struct BATTLE_STATE
{
    Sint32 formation;      // the encounter's enemy formation
    Sint16 spriteSlots[6]; // the unit in each enemy sprite slot, 0 when free
    // The kinds of enemy in the formation, for the letters that tell two of a kind apart: five at
    // most, where The Broken Seal keeps six.
    Uint16 enemyKinds[5];
    Uint16 kindsEnd; // read only by the defeat search, as a sixth kind
    // for each kind, the name letters already given
    Uint32 nameLetterMasks[6];
    Sint8  lastNameLetter[6];         // for each kind, the last name letter given
    Uint8  unusedAfterNameLetters[2]; // no code names it
    // The earliest kind in enemyKinds that fell, which the victory message names; ENEMY_BATTLE_SIZE
    // until one falls.
    Uint16 firstDefeated;
    // DEFEAT_ONE, DEFEAT_SEVERAL or DEFEAT_BY_PARTY_MEMBER
    Uint16 defeatState;
    Uint8  enemyKindCount; // kinds of enemy listed, at most 5
    Uint8  partyStatus;    // how the party status window draws
    // The formation's layout: 0 or 1 lists the enemies in the order they were drawn.
    Uint8  layout;
    // bits: 1 a party unit was challenged
    Uint8  challengeFlag;
    Uint8  isLink;      // 1 in a link battle
    Uint8  firstStrike; // who strikes first: 1 the party, 2 the enemies
    // Failed tries to flee this battle; each adds 2000 to the chance (btFleeCheckSuccess).
    Uint8  fleeFailures;
    // set by Lull with MsgAllResting, "Everybody is resting!"
    Uint8  ceaseFire;
    // For each party unit, its slot as a link opponent's enemy, minus 128; OPPONENT_SLOT_NONE for
    // none.
    Uint8  opponentSlots[8];
    Uint8  linkSide;         // this console's multiplayer id
    Uint8  linkMisses;       // ticks the link has been down
    Uint8  linkPaused;       // set when the link drops
    Uint8  paddingAfterLink; // up to the word arrowTiles starts on
    Sint32
        // resource entry of the advance arrow's tiles; -1 for none
                  arrowTiles;
    Sint16        partyUnits[7]; // ends with BATTLE_UNIT_LIST_END
    Sint16        enemyIds[7];   // ends with BATTLE_UNIT_LIST_END
    // Each unit's index in slots, party ids 0 to 7 then the enemies; 255 for none
    // (BATTLE_OBJECT_NONE).
    Uint8         placement[16];
    BATTLE_OBJECT slots[14];
    BATTLE_ACTION_RECORD
    actions[BATTLE_ACTION_CAPACITY];               // the actions chosen for the turn
    Uint8                 unusedAfterActions[292]; // no code names it
    BATTLE_SPOILS         spoils;                  // what the battle has won so far
    // the backdrop's colours, before scaling
    Uint16                palette[BATTLE_BACKGROUND_COLORS];
    FIXED                 brightness; // the backdrop's brightness, 16.16, toFIXED(1) full
    // Its first halfword is the battle background passed to the palette fade.
    Uint16                bg;               // the battle background's id
    Uint8                 unusedAfterBg[4]; // no code names it
    // stepped each tick while the units are highlighted
    Uint16                highlightStep;
    Uint16                highlightMode;            // nonzero while the units are highlighted
    Uint8                 paddingAfterHighlight[2]; // up to the word plan starts on
    // The action being carried out, as btActionRunBuild lays it out.
    BATTLE_PLAN           plan;
    Sint32                skipPlayback; // nonzero leaves the action's events unplayed
    BATTLE_PLAYBACK_STATE events;
    // A value btAdjustDebugValue edits with the d-pad while Start is held.
    Sint32                debugValue;
    // The unit ready to counterattack (ABILITY_EFFECT_COUNTER) that was just struck, and the
    // opponent that struck it.
    Sint32                counterUnitId;
    Sint32                counterTargetId;
    // Daedalus strikes again at the end of the turns after it is summoned: each queued strike is a
    // bit, set when an enemy summoned it, and the next turn's queue is gathered while this one's is
    // played.
    Uint16                daedalusFromEnemies;
    Uint16                nextDaedalusFromEnemies;
    Uint8                 daedalusStrikes;
    Uint8                 nextDaedalusStrikes;
    // Nonzero leaves the battle objects' sprite priorities as they are, while Flora's effect plays.
    Uint8                 keepObjectPriorities;
    // Nonzero keeps the backdrop's palette while a summon (Charon, Atalanta, Neptune, Moloch)
    // colours the screen.
    Uint8                 keepBackdropPalette;
    // Poseidon's force field: 1 when the formation's last enemy is one of ENEMY_POSEIDON to
    // ENEMY_POSEIDON_3, outside FLAG_MONSTER_BATTLE; until the Trident lands, MsgFieldProtects
    // turns abilities away.
    Uint8                 forceField;
    // The animation the acting unit takes as it calls up its Psynergy; 3 as its action starts.
    Uint8                 actingAnimation;
    Uint8                 paddingAfterActingAnimation[2]; // up to the word aiStates starts on
    // The action choice's memory for the enemies that act more than once a turn.
    BATTLE_AI_STATE       aiStates[4];
} BATTLE_STATE;

STATIC_ASSERT(offsetof(BATTLE_STATE, partyStatus) == 65);
STATIC_ASSERT(offsetof(BATTLE_STATE, challengeFlag) == 67);
STATIC_ASSERT(offsetof(BATTLE_STATE, isLink) == 68);
STATIC_ASSERT(offsetof(BATTLE_STATE, firstStrike) == 69);
STATIC_ASSERT(offsetof(BATTLE_STATE, partyUnits) == 88);
STATIC_ASSERT(offsetof(BATTLE_STATE, enemyIds) == 102);
STATIC_ASSERT(offsetof(BATTLE_STATE, placement) == 116);
STATIC_ASSERT(offsetof(BATTLE_STATE, slots) == 132);
STATIC_ASSERT(offsetof(BATTLE_STATE, actions) == 748);
STATIC_ASSERT(offsetof(BATTLE_STATE, spoils) == 1360);
STATIC_ASSERT(offsetof(BATTLE_STATE, spoils.queuedSummons) == 1380);
STATIC_ASSERT(offsetof(BATTLE_STATE, spoils.queueCount) == 1387);
STATIC_ASSERT(offsetof(BATTLE_STATE, highlightStep) == 1654);
STATIC_ASSERT(offsetof(BATTLE_STATE, highlightMode) == 1656);
STATIC_ASSERT(offsetof(BATTLE_STATE, events) == 1764);
STATIC_ASSERT(offsetof(BATTLE_STATE, plan) == 1660);
STATIC_ASSERT(offsetof(BATTLE_STATE, debugValue) == 2132);
STATIC_ASSERT(offsetof(BATTLE_STATE, daedalusFromEnemies) == 2144);
STATIC_ASSERT(offsetof(BATTLE_STATE, keepBackdropPalette) == 2151);
STATIC_ASSERT(offsetof(BATTLE_STATE, forceField) == 2152);
STATIC_ASSERT(offsetof(BATTLE_STATE, aiStates) == 2156);
STATIC_ASSERT(sizeof(BATTLE_STATE) == 2172);

#endif
