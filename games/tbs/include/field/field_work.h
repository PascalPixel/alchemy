#ifndef ALCHEMY_FIELD_WORK_H
#define ALCHEMY_FIELD_WORK_H

/// @file
/// FieldWork, the running scene's work, with its treasure spots and the raised
/// triggers the scene sequence handles itself.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

struct ActorTemplate;
struct Trigger;
struct Actor;
struct Actor;
struct MapGridCell;

/// The actor ids FieldWork.actors maps (enum ActorId): the party's eight
/// characters, then the actors a scene places. An actor id indexes this table,
/// not the 64 slots of gActors.
#define ACTOR_ID_COUNT 66

/// A treasure spot placed as an actor: a chest or a Psynergy Stone.
struct TreasureSpot {
  struct Actor *actor; ///< the actor placed for it
  u8 id;               ///< the subject of its trigger, 100 to 239
  u8 unused;           ///< cleared with the slot; no code names it
  u8 column;           ///< its 16-pixel cell
  u8 row;              ///< its 16-pixel cell
};

STATIC_ASSERT(sizeof(struct TreasureSpot) == 8);
STATIC_ASSERT(offsetof(struct TreasureSpot, id) == 4);
STATIC_ASSERT(offsetof(struct TreasureSpot, column) == 6);
STATIC_ASSERT(offsetof(struct TreasureSpot, row) == 7);

/// The values of FieldWork.raisedTrigger that the scene sequence handles
/// itself, rather than passing them to the scene (Scene_RunRaisedTrigger).
enum FieldRaisedTrigger {
  /// Party members fell to poison or hazardous ground this step.
  FIELD_RAISED_TRIGGER_PARTY_FELL = -1,
  /// R on the world map: the picture of the whole world (WorldPicture_Show).
  FIELD_RAISED_TRIGGER_WORLD_PICTURE = -888,
  /// L on the world map: the view zooms out while L is held
  /// (FieldEffect_RunVisibilityTransition).
  FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM = -889
};

/// A field scene's work: SceneSequence_Run allocates it in heap slot
/// HEAP_SLOT_FIELD, zeroes it and frees it on the way out, and the scene
/// sequence, the event commands and the scene overlays all reach it through
/// gFieldWork. The twelve requests from stepTrigger to raisedTrigger are
/// handled by the scene sequence.
struct FieldWork {
  /// The actor template tables placed so far, recorded by
  /// Event_SpawnActorTemplates.
  struct ActorTemplate *tables[4];
  const struct Trigger *triggers; ///< the scene's trigger table
  /// The actors by actor id: 0 to 7 the party's characters, 8 on the
  /// placed actors.
  struct Actor *actors[ACTOR_ID_COUNT];
  struct TreasureSpot
      treasureSpots[10]; ///< the map's treasure spots (TREASURE_SPOT_MAX)
  /// The twelve requests: each nonzero value asks the scene sequence for one
  /// thing; Field_ClearRequests clears them and Field_CountRequests sums them.
  ///@{
  s16 stepTrigger; ///< a step trigger (1 to 239) for Scene_RunStepTriggerOrExit
  s16 pushCell;    ///< a metatile behavior (252 to 254) for Field_RunPushCell
  s16 exit;        ///< the exit taken, which ends the scene
  s16 fieldMenu;   ///< open the field menu
  /// The A button: a talk, an examine or the field menu, by what is ahead.
  s16 aButton;
  s16 pauseMenu;   ///< the Start button: open the Pause menu
  u16 talk;        ///< an actor's talk trigger, as actor id | 0x1000
  s16 examineCell; ///< a cell's metatile behavior, for Field_ExamineCell
  s16 encounter;   ///< the formation of a random encounter
  s16 psynergy;    ///< a field Psynergy to use, as unit << 10 | Psynergy
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
  s16 mode;               ///< an enum ActorSystemMode
  u8 encounterZones[8];   ///< encounter zone for each kind of ground
  s32 encounterBias;      ///< randomized distance adjustment between battles
  s32 encounterThreshold; ///< encounter steps before a battle; 0x100000
  /// Scales the leader's speed into walkingDistance, 16.16 fixed point: 0x199
  /// (about 1/160) on entry, 0 in a town.
  s32 walkingDistanceScale;
  /// Distance walked toward the next 0x10000, at which the party regains PP,
  /// Djinn recover and poison hurts (Field_UpdateWalking).
  s32 walkingDistance;
  struct MapGridCell *currentCell;  ///< the cell the leader last stepped onto
  struct MapGridCell *previousCell; ///< the cell before it
  s32 transition;       ///< SCENE_TRANSITION(style, variant); 0x100 on entry
  u16 unused;           ///< no code names it
  u16 transitionActive; ///< set once the opening transition has started
  s32 transitionFrames; ///< frames; 16 on entry and at each event
  s32 fastForward;      ///< debug: skip timed event waits and actor messages
  u8 unusedAfterFastForward[8]; ///< no code names it
  s16 message; ///< the next message, advanced as each window opens
  /// Set to 0xffff by Event_Start at each event; nothing reads it.
  u16 unusedAfterMessage;
  /// Both set to -1 by Event_Start at each event; nothing reads them.
  s16 unusedBeforeCamera[2];
  struct Actor
      *camera; ///< the actor the map view follows, which has no actor id
  u8 unusedAfterCamera[0x10]; ///< no code names it
  s32 speaker;                ///< the actor speaking; the leader at each event
  s32 messageWindow;          ///< the open message window, 0 for none
  s32 sideWindow;             ///< the speaker's portrait window
  /// The leader's two-row actor template table (its row and an end
  /// row), which Actor_ResetForLeader builds through its offset.
  u8 leaderTemplates[0x30];
  u8 unusedAfterTemplates[0xa86]; ///< no code names it
  /// Set while a raised or step trigger runs; Event_Start then resets the
  /// effect counter.
  u16 busy;
  s16 revealActive;  ///< nonzero while Reveal is active
  s16 revealFrames;  ///< the frames Reveal has left
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
};

STATIC_ASSERT(sizeof(struct FieldWork) == 0xccc);
STATIC_ASSERT(offsetof(struct FieldWork, tables) == 0);
STATIC_ASSERT(offsetof(struct FieldWork, triggers) == 0x10);
STATIC_ASSERT(offsetof(struct FieldWork, actors) == 0x14);
STATIC_ASSERT(offsetof(struct FieldWork, treasureSpots) == 0x11c);
STATIC_ASSERT(offsetof(struct FieldWork, stepTrigger) == 0x16c);
STATIC_ASSERT(offsetof(struct FieldWork, talk) == 0x178);
STATIC_ASSERT(offsetof(struct FieldWork, encounter) == 0x17c);
STATIC_ASSERT(offsetof(struct FieldWork, raisedTrigger) == 0x182);
STATIC_ASSERT(offsetof(struct FieldWork, blockedSteps) == 0x19c);
STATIC_ASSERT(offsetof(struct FieldWork, mode) == 0x19e);
STATIC_ASSERT(offsetof(struct FieldWork, encounterZones) == 0x1a0);
STATIC_ASSERT(offsetof(struct FieldWork, encounterBias) == 0x1a8);
STATIC_ASSERT(offsetof(struct FieldWork, encounterThreshold) == 0x1ac);
STATIC_ASSERT(offsetof(struct FieldWork, walkingDistanceScale) == 0x1b0);
STATIC_ASSERT(offsetof(struct FieldWork, walkingDistance) == 0x1b4);
STATIC_ASSERT(offsetof(struct FieldWork, currentCell) == 0x1b8);
STATIC_ASSERT(offsetof(struct FieldWork, previousCell) == 0x1bc);
STATIC_ASSERT(offsetof(struct FieldWork, transition) == 0x1c0);
STATIC_ASSERT(offsetof(struct FieldWork, transitionActive) == 0x1c6);
STATIC_ASSERT(offsetof(struct FieldWork, transitionFrames) == 0x1c8);
STATIC_ASSERT(offsetof(struct FieldWork, fastForward) == 0x1cc);
STATIC_ASSERT(offsetof(struct FieldWork, message) == 0x1d8);
STATIC_ASSERT(offsetof(struct FieldWork, camera) == 0x1e0);
STATIC_ASSERT(offsetof(struct FieldWork, unusedAfterMessage) == 0x1da);
STATIC_ASSERT(offsetof(struct FieldWork, unusedBeforeCamera) == 0x1dc);
STATIC_ASSERT(offsetof(struct FieldWork, speaker) == 0x1f4);
STATIC_ASSERT(offsetof(struct FieldWork, messageWindow) == 0x1f8);
STATIC_ASSERT(offsetof(struct FieldWork, sideWindow) == 0x1fc);
STATIC_ASSERT(offsetof(struct FieldWork, busy) == 0xcb6);
STATIC_ASSERT(offsetof(struct FieldWork, revealActive) == 0xcb8);
STATIC_ASSERT(offsetof(struct FieldWork, revealFrames) == 0xcba);
STATIC_ASSERT(offsetof(struct FieldWork, revealCenterX) == 0xcbc);
STATIC_ASSERT(offsetof(struct FieldWork, revealCenterY) == 0xcbe);
STATIC_ASSERT(offsetof(struct FieldWork, effectTimerRunning) == 0xcc0);
STATIC_ASSERT(offsetof(struct FieldWork, choiceMessage) == 0xcc2);
STATIC_ASSERT(offsetof(struct FieldWork, choiceWidth) == 0xcc4);
STATIC_ASSERT(offsetof(struct FieldWork, psynergyActing) == 0xcc6);
STATIC_ASSERT(offsetof(struct FieldWork, castStarted) == 0xcc7);
STATIC_ASSERT(offsetof(struct FieldWork, queuedSound) == 0xcc8);

/// The running field scene's work, heap slot HEAP_SLOT_FIELD; SceneSequence_Run
/// takes it and frees it.
extern struct FieldWork *gFieldWork;

#endif
