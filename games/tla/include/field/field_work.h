#ifndef ALCHEMY_FIELD_WORK_H
#define ALCHEMY_FIELD_WORK_H

/// @file
/// FieldWork, the running scene's work: The Broken Seal's, with the Psynergy
/// state The Lost Age adds at its end.

#include "types.h"
#include "edition.h"
#include "static_assert.h"
#include "system/ram_buffer.h"

struct ActorPlacement;
struct Actor;
struct MapCell;
struct TriggerView;
struct Window;

/// The actor ids FieldWork.actors maps: the party's eight characters, the
/// actors a scene places, and from FIELD_TREASURE_ACTOR on the map's treasure
/// spots.
#define FIELD_ACTOR_COUNT 80
/// The first actor id of a treasure spot (Field_PlaceTreasureSpots).
#define FIELD_TREASURE_ACTOR 64

/// The values of FieldWork.raisedTrigger that the field sequence handles
/// itself, rather than passing them to the scene (Scene_RunRaisedEvent).
enum FieldRaisedTrigger {
  /// Party members fell to poison or hazardous ground this step.
  FIELD_RAISED_TRIGGER_PARTY_FELL = -1,
  /// R on the world map: the picture of the whole world (WorldPicture_Show).
  FIELD_RAISED_TRIGGER_WORLD_PICTURE = -887,
  /// R while the world picture is offered by an arrival.
  FIELD_RAISED_TRIGGER_WORLD_PICTURE_LOOP = -888,
  /// L on the world map: the view zooms out while L is held.
  FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM = -889,
  FIELD_RAISED_TRIGGER_ROPE_OFF = -890,
  FIELD_RAISED_TRIGGER_ROPE_ON = -891,
  FIELD_RAISED_TRIGGER_TIGHTROPE_OFF = -892,
  FIELD_RAISED_TRIGGER_TIGHTROPE_ON = -893
};

/// A field scene's work: FieldSequence_Run allocates it in heap slot 108,
/// zeroes it and frees it on the way out, and the field sequence, the event
/// commands and the scene overlays all reach it through
/// Ram_HeapSlots->fieldWork.
struct FieldWork {
  /// The actor placement tables placed so far, recorded by
  /// Event_SpawnActorPlacements.
  const struct ActorPlacement *tables[4];
  const struct TriggerView *triggers; ///< the scene's trigger table
  /// The actors by actor id: 0 to 7 the party's characters, 8 on the
  /// placed actors, FIELD_TREASURE_ACTOR on the treasure spots.
  struct Actor *actors[FIELD_ACTOR_COUNT];
  /// The twelve requests: each nonzero value asks the field sequence for one
  /// thing; Field_ClearRequests clears them and Field_CountRequests sums them.
  ///@{
  s16 stepTrigger; ///< a step trigger (1 to 230) for Scene_RunStepTriggerOrExit
  s16 pushCell;    ///< a cell's attributes for Field_RunPushCell
  s16 exit;        ///< the exit taken, which ends the scene; 999 for a warp
  s16 fieldMenu;   ///< open the field menu
  /// The A button: a talk, an examine or the field menu, by what is ahead.
  s16 aButton;
  s16 pauseMenu;   ///< the Start button: open the Pause menu
  s16 talk;        ///< an actor's talk trigger, as actor id | 0x4000
  s16 examineCell; ///< a cell's cell behavior, for Field_ExamineCell
  s16 encounter;   ///< the enemy group of a random encounter
  s16 psynergy;    ///< a field Psynergy to use, as unit << 10 | Psynergy
  s16 item;        ///< an item to use, packed as Field_UseItem reads it
  /// A raised trigger's number for Scene_RunRaisedEvent, or an enum
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
  u16 heldKeys; ///< the keys held when a rope or tightrope was raised
  u8 unk188[2];
  s8 mode; ///< 2 on a map, 3 on the world map (ActorSystem_InitializeFar)
  s8 unk18b;
  u8 encounterZones[8];   ///< encounter zone for each kind of ground
  s32 encounterBias;      ///< randomized distance adjustment between battles
  s32 encounterThreshold; ///< encounter steps before a battle; 16 * Q16_ONE
  /// Scales the leader's speed into walkingDistance, in 16.16 fixed point:
  /// 409 on entry, 0 in a town.
  s32 walkingDistanceScale;
  /// Distance walked toward the next Q16_ONE, at which the party regains PP,
  /// Djinn recover and poison hurts (Field_UpdateWalking).
  s32 walkingDistance;
  struct MapCell *currentCell;  ///< the cell the leader last stepped onto
  struct MapCell *previousCell; ///< the cell before it
  s32 transition; ///< SCENE_TRANSITION(style, variant); (1, 0) on entry
  u16 unused;
  u16 transitionActive; ///< set once the opening transition has started
  s32 transitionFrames; ///< frames; 16 on entry and at each event
  s32 fastForward;      ///< debug: skip timed event waits and actor messages
  u8 unusedAfterFastForward[8];
  s16 message; ///< the next message, advanced as each window opens
  /// Set to 0xffff by Event_Start at each event; nothing reads it.
  u16 unusedAfterMessage;
  /// Both set to -1 by Event_Start at each event; nothing reads them.
  s16 unusedBeforeCamera[2];
  struct Actor *camera; ///< the actor the map view follows
  u8 unusedAfterCamera[16];
  s32 speaker; ///< the actor speaking; the leader at each event
  struct Window *messageWindow; ///< the open message window, NULL for none
  struct Window *sideWindow;    ///< the speaker's portrait window
  /// The leader's two-row actor placement table (its row and an end
  /// row), which Actor_ResetForLeader builds through its offset.
  u8 leaderPlacements[48];
  struct Window *noticeWindow; ///< a scene's timed notice
  u16 noticeFrames;
  /// The palette a field Psynergy's tint fades back to, as a palette fade's
  /// target (BG half, then sprite half), saved as the tint starts.
  u16 savedFadeTarget[448 * 3];
  /// Set while a raised or step trigger runs; Event_Start then ends the
  /// lasting Psynergy.
  s16 busy;
  s8 revealActive;       ///< nonzero while Reveal is active
  s8 effectTimerRunning; ///< lets the Reveal countdown advance
  s16 revealFrames;      ///< the frames Reveal has left
  s16 revealCenterX;     ///< where Reveal was cast, in pixels
  s16 revealCenterY;
  s8 unkcac; ///< a lasting Psynergy (155) is active
  s8 unkcad;
  s16 unkcae;
  /// The first of the two names the next yes/no choice shows in place of Yes
  /// and No (YesNoMenu_Run), 0 for those.
  s16 choiceMessage;
  s16 choiceWidth;     ///< the choice window's width, 0 for the default
  s16 choiceSelection; ///< the answer the choice starts on
  s8 psynergyActing;   ///< set while Psynergy takes effect
  s8 castStarted;      ///< set once a cast has been announced
  s16 queuedSound;     ///< a sound Scene_PlayQueuedMusic plays; -1 for none
  u8 unkcba[2];
  struct Actor *unkcbc;
  /// The cells a Psynergy touches, as cell coordinates, ended by -1.
  s16 psynergyCells[56];
  s8 unkd30;
  s8 targetX;
  s8 targetY;
  s8 targetZ;
  s16 unkd34;
  s8 sceneAttributeLow;
  s8 sceneAttributeHigh;
  s8 unkd38;
  s8 hoverReady; ///< a party member knows Hover and the ship flies
  s8 terrainKind;
  s8 climbing;
  s16 unkd3c;
  s16 unkd3e;
  s16 unkd40;
  s16 unkd42;
  s16 glow;
  s16 glowHeld;
  s16 standing;
  u16 unkd4a;
  u16 unkd4c;
  u8 unkd4e[2];
};

STATIC_ASSERT(sizeof(struct FieldWork) == 0xd50);
STATIC_ASSERT(offsetof(struct FieldWork, triggers) == 0x10);
STATIC_ASSERT(offsetof(struct FieldWork, actors) == 0x14);
STATIC_ASSERT(offsetof(struct FieldWork, stepTrigger) == 0x154);
STATIC_ASSERT(offsetof(struct FieldWork, exit) == 0x158);
STATIC_ASSERT(offsetof(struct FieldWork, encounter) == 0x164);
STATIC_ASSERT(offsetof(struct FieldWork, raisedTrigger) == 0x16a);
STATIC_ASSERT(offsetof(struct FieldWork, blockedSteps) == 0x184);
STATIC_ASSERT(offsetof(struct FieldWork, mode) == 0x18a);
STATIC_ASSERT(offsetof(struct FieldWork, encounterZones) == 0x18c);
STATIC_ASSERT(offsetof(struct FieldWork, transition) == 0x1ac);
STATIC_ASSERT(offsetof(struct FieldWork, transitionFrames) == 0x1b4);
STATIC_ASSERT(offsetof(struct FieldWork, message) == 0x1c4);
STATIC_ASSERT(offsetof(struct FieldWork, camera) == 0x1cc);
STATIC_ASSERT(offsetof(struct FieldWork, speaker) == 0x1e0);
STATIC_ASSERT(offsetof(struct FieldWork, leaderPlacements) == 0x1ec);
STATIC_ASSERT(offsetof(struct FieldWork, noticeWindow) == 0x21c);
STATIC_ASSERT(offsetof(struct FieldWork, savedFadeTarget) == 0x222);
STATIC_ASSERT(offsetof(struct FieldWork, busy) == 0xca2);
STATIC_ASSERT(offsetof(struct FieldWork, revealActive) == 0xca4);
STATIC_ASSERT(offsetof(struct FieldWork, choiceMessage) == 0xcb0);
STATIC_ASSERT(offsetof(struct FieldWork, psynergyActing) == 0xcb6);
STATIC_ASSERT(offsetof(struct FieldWork, queuedSound) == 0xcb8);
STATIC_ASSERT(offsetof(struct FieldWork, psynergyCells) == 0xcc0);
STATIC_ASSERT(offsetof(struct FieldWork, targetX) == 0xd31);
STATIC_ASSERT(offsetof(struct FieldWork, sceneAttributeLow) == 0xd36);
STATIC_ASSERT(offsetof(struct FieldWork, hoverReady) == 0xd39);
STATIC_ASSERT(offsetof(struct FieldWork, glow) == 0xd44);
STATIC_ASSERT(offsetof(struct FieldWork, unkd4c) == 0xd4c);

/// The running field scene's work, heap slot 108; FieldSequence_Run takes it
/// and frees it.
#define gFieldWork ((struct FieldWork *)Ram_HeapSlots->fieldWork)

#endif
