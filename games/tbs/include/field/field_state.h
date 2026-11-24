#ifndef ALCHEMY_FIELD_STATE_H
#define ALCHEMY_FIELD_STATE_H

/// @file
/// FieldState, the running scene's state, with its treasure spots and the
/// raised triggers the field sequence handles itself.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/heap.h"

struct ActorPlacement;
struct Trigger;
struct Actor;
struct MapGridCell;

/// The actor ids FieldState.actors maps (enum ActorId): the party's eight
/// characters, then the actors a scene places.
#define ACTOR_ID_COUNT 66

/// A treasure spot placed as an actor: a chest or a Psynergy Stone.
struct TreasureSpot {
  struct Actor *actor; ///< the actor placed for it
  u8 id;     ///< the subject of its trigger (TREASURE_SPOT_FIRST_ID on)
  u8 unused; ///< cleared with the slot
  u8 column; ///< its 16-pixel cell
  u8 row;    ///< its 16-pixel cell
};

STATIC_ASSERT(sizeof(struct TreasureSpot) == 8);
STATIC_ASSERT(offsetof(struct TreasureSpot, id) == 4);
STATIC_ASSERT(offsetof(struct TreasureSpot, column) == 6);
STATIC_ASSERT(offsetof(struct TreasureSpot, row) == 7);

/// The values of FieldState.raisedTrigger that the field sequence handles
/// itself, rather than passing them to the scene (Scene_RunRaisedTrigger).
enum FieldRaisedTrigger {
  /// Party members fell to poison or hazardous ground this step.
  FIELD_RAISED_TRIGGER_PARTY_FELL = -1,
  /// R on the world map: the map screen (MapScreen_Show).
  FIELD_RAISED_TRIGGER_MAP_SCREEN = -888,
  /// L on the world map: the view zooms out while L is held
  /// (WorldMap_ZoomOutWhileLHeld).
  FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM = -889
};

/// How long a map's screen transition takes, in frames, unless an event or
/// scene sets another length.
#define MAP_TRANSITION_FRAMES 16
/// The frames Reveal lasts once cast, ten seconds (FieldState.revealFrames).
#define REVEAL_FRAMES 600

/// A field Psynergy for FieldState.psynergy: the unit who uses it, in bits 10
/// to 13, and the Psynergy, in bits 0 to 9 (Field_UsePsynergy).
#define FIELD_PSYNERGY(unit, abilityId) ((unit) << 10 | (abilityId))

/// A field scene's state: FieldSequence_Run allocates it in heap slot
/// HEAP_SLOT_FIELD, zeroes it and frees it on the way out, and the field
/// sequence, the event commands and the scene overlays all reach it through
/// gFieldState.
struct FieldState {
  /// The actor placement tables placed so far, recorded by
  /// Event_SpawnActorPlacements.
  struct ActorPlacement *tables[4];
  const struct Trigger *triggers; ///< the scene's trigger table
  /// The actors by actor id: 0 to 7 the party's characters, 8 on the
  /// placed actors.
  struct Actor *actors[ACTOR_ID_COUNT];
  struct TreasureSpot
      treasureSpots[10]; ///< the map's treasure spots (TREASURE_SPOT_MAX)
  /// The twelve requests: each nonzero value asks the field sequence for one
  /// thing; Field_ClearRequests clears them and Field_CountRequests sums them.
  ///@{
  s16 stepTrigger; ///< a step trigger (1 to 239) for Scene_RunStepTriggerOrExit
  s16 pushCell;    ///< a cell behavior (252 to 254) for Field_RunPushCell
  s16 exit;        ///< the exit taken, which ends the scene
  s16 fieldMenu;   ///< open the field menu
  /// The A button: a talk, an examine or the field menu, by what is ahead.
  s16 aButton;
  s16 pauseMenu;   ///< the Start button: open the Pause menu
  u16 talk;        ///< an actor's talk trigger, as actor id | 0x1000
  s16 examineCell; ///< a cell's cell behavior, for Field_ExamineCell
  s16 encounter;   ///< the formation of a random encounter
  s16 psynergy;    ///< a field Psynergy to use, as FIELD_PSYNERGY makes it
  s16 item;        ///< an item to use, packed as Field_UseItem reads it
  /// A raised trigger's number for Scene_RunRaisedTrigger, or an enum
  /// FieldRaisedTrigger.
  s16 raisedTrigger;
  ///@}
  s16 fallenCount;     ///< the characters who fell this step
  s16 survivorCount;   ///< 0 when the whole party fell
  s16 fallenOwners[9]; ///< the characters who fell, from the active party
  s16 itemSlot;        ///< the inventory slot Field_UseItem receives
  /// Frames the leader has pushed against something, counted by the player
  /// controls; step triggers and push cells need more than 12.
  s16 blockedSteps;
  s16 mode;                 ///< an enum ActorSystemMode
  u8 encounterZones[8];     ///< encounter zone for each kind of ground
  s32 encounterBias;        ///< randomized distance adjustment between battles
  FIXED encounterThreshold; ///< encounter steps before a battle; 16 * FIXED_ONE
  /// Scales the leader's speed into walkingDistance, in 16.16 fixed point:
  /// FIXED_ONE / 160 on entry, 0 in a town.
  FIXED walkingDistanceScale;
  /// Distance walked toward the next FIXED_ONE, at which the party regains PP,
  /// Djinn recover and poison hurts (Field_UpdateWalking).
  FIXED walkingDistance;
  struct MapGridCell *currentCell;  ///< the cell the leader last stepped onto
  struct MapGridCell *previousCell; ///< the cell before it
  s32 transition; ///< SCENE_TRANSITION(style, variant); (1, 0) on entry
  u16 unused;
  u16 transitionActive; ///< set once the opening transition has started
  s32 transitionFrames; ///< frames; MAP_TRANSITION_FRAMES on entry and at each
                        ///< event
  s32 fastForward;      ///< debug: skip timed event waits and actor messages
  u8 unusedAfterFastForward[8];
  s16 message; ///< the next message, advanced as each window opens
  /// Set to 0xffff by Event_Start at each event; nothing reads it.
  u16 unusedAfterMessage;
  /// Both set to -1 by Event_Start at each event; nothing reads them.
  s16 unusedBeforeCamera[2];
  struct Actor
      *camera; ///< the actor the map view follows, which has no actor id
  u8 unusedAfterCamera[16];
  s32 speaker;       ///< the actor speaking; the leader at each event
  s32 messageWindow; ///< the open message window, 0 for none
  s32 sideWindow;    ///< the speaker's portrait window
  /// The leader's two-row actor placement table (its row and an end
  /// row), which Actor_ResetForLeader builds through its offset.
  u8 leaderPlacements[48];
  u8 unusedAfterPlacements[6];
  /// The palette a field Psynergy's tint fades back to, as a palette fade's
  /// target (BG half, then sprite half), saved as the tint starts.
  u16 savedFadeTarget[448 * 3];
  /// Set while a raised or step trigger runs; Event_Start then resets the
  /// effect counter.
  u16 busy;
  s16 revealActive;  ///< nonzero while Reveal is active
  s16 revealFrames;  ///< the frames Reveal has left, REVEAL_FRAMES as cast
  s16 revealCenterX; ///< where Reveal was cast, in pixels
  s16 revealCenterY;
  /// Nonzero lets the Reveal countdown advance; menus and other effects pause
  /// it.
  s16 effectTimerRunning;
  /// The first of the two names the next yes/no choice shows in place of Yes
  /// and No (YesNoMenu_Run), 0 for those.
  s16 choiceMessage;
  s16 choiceWidth;   ///< the choice window's width, 0 for the default
  s8 psynergyActing; ///< set while Psynergy takes effect
  s8 castStarted;    ///< set once a cast has been announced
  s16 queuedSound;   ///< a sound Scene_PlayQueuedMusic plays; -1 for none
  /// Set while the field menu is open (FieldMenu_Run), which pauses Reveal's
  /// sparkles; only the German, Spanish, French and Italian editions keep it.
  s8 menuOpen;
};

STATIC_ASSERT(sizeof(struct FieldState) == 3276);
STATIC_ASSERT(offsetof(struct FieldState, tables) == 0);
STATIC_ASSERT(offsetof(struct FieldState, triggers) == 16);
STATIC_ASSERT(offsetof(struct FieldState, actors) == 20);
STATIC_ASSERT(offsetof(struct FieldState, treasureSpots) == 284);
STATIC_ASSERT(offsetof(struct FieldState, stepTrigger) == 364);
STATIC_ASSERT(offsetof(struct FieldState, talk) == 376);
STATIC_ASSERT(offsetof(struct FieldState, encounter) == 380);
STATIC_ASSERT(offsetof(struct FieldState, raisedTrigger) == 386);
STATIC_ASSERT(offsetof(struct FieldState, blockedSteps) == 412);
STATIC_ASSERT(offsetof(struct FieldState, mode) == 414);
STATIC_ASSERT(offsetof(struct FieldState, encounterZones) == 416);
STATIC_ASSERT(offsetof(struct FieldState, encounterBias) == 424);
STATIC_ASSERT(offsetof(struct FieldState, encounterThreshold) == 428);
STATIC_ASSERT(offsetof(struct FieldState, walkingDistanceScale) == 432);
STATIC_ASSERT(offsetof(struct FieldState, walkingDistance) == 436);
STATIC_ASSERT(offsetof(struct FieldState, currentCell) == 440);
STATIC_ASSERT(offsetof(struct FieldState, previousCell) == 444);
STATIC_ASSERT(offsetof(struct FieldState, transition) == 448);
STATIC_ASSERT(offsetof(struct FieldState, transitionActive) == 454);
STATIC_ASSERT(offsetof(struct FieldState, transitionFrames) == 456);
STATIC_ASSERT(offsetof(struct FieldState, fastForward) == 460);
STATIC_ASSERT(offsetof(struct FieldState, message) == 472);
STATIC_ASSERT(offsetof(struct FieldState, camera) == 480);
STATIC_ASSERT(offsetof(struct FieldState, unusedAfterMessage) == 474);
STATIC_ASSERT(offsetof(struct FieldState, unusedBeforeCamera) == 476);
STATIC_ASSERT(offsetof(struct FieldState, speaker) == 500);
STATIC_ASSERT(offsetof(struct FieldState, messageWindow) == 504);
STATIC_ASSERT(offsetof(struct FieldState, sideWindow) == 508);
STATIC_ASSERT(offsetof(struct FieldState, savedFadeTarget) == 566);
STATIC_ASSERT(offsetof(struct FieldState, busy) == 3254);
STATIC_ASSERT(offsetof(struct FieldState, revealActive) == 3256);
STATIC_ASSERT(offsetof(struct FieldState, revealFrames) == 3258);
STATIC_ASSERT(offsetof(struct FieldState, revealCenterX) == 3260);
STATIC_ASSERT(offsetof(struct FieldState, revealCenterY) == 3262);
STATIC_ASSERT(offsetof(struct FieldState, effectTimerRunning) == 3264);
STATIC_ASSERT(offsetof(struct FieldState, choiceMessage) == 3266);
STATIC_ASSERT(offsetof(struct FieldState, choiceWidth) == 3268);
STATIC_ASSERT(offsetof(struct FieldState, psynergyActing) == 3270);
STATIC_ASSERT(offsetof(struct FieldState, castStarted) == 3271);
STATIC_ASSERT(offsetof(struct FieldState, queuedSound) == 3272);

/// The running field scene's state, heap slot HEAP_SLOT_FIELD;
/// FieldSequence_Run takes it and frees it.
#define gFieldState (*(struct FieldState **)&gHeapSlots[HEAP_SLOT_FIELD])

#endif
