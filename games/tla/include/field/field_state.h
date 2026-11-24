#ifndef ALCHEMY_FIELD_STATE_H
#define ALCHEMY_FIELD_STATE_H

/// @file
/// FieldState, the running scene's state, with the field Psynergy's state at
/// its end.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "static_assert.h"
#include "system/ram_buffer.h"
#include "field/scene_tables.h"

struct ActorPlacement;
struct Actor;
struct MapCell;
struct TriggerView;
struct Window;

/// The actor ids FieldState.actors maps: the party's eight characters, the
/// actors a scene places, and from FIELD_TREASURE_ACTOR on the map's treasure
/// spots.
#define FIELD_ACTOR_COUNT 80

/// FieldState.scoopedItem: coins rather than an item, their count in the low
/// bits.
#define SCOOPED_COINS 0x8000
/// The actor that shows what Scoop dug up.
#define SCOOPED_ITEM_ACTOR 79
/// The first actor id of a treasure spot (Field_PlaceTreasureSpots).
#define FIELD_TREASURE_ACTOR 64
/// FieldState.talk and Scene_TalkToActor: with this bit a container is opened
/// without its "checked the ..." message, as Scoop opens what it digs up; the
/// actor id is in the bits under it.
#define FIELD_TALK_SILENT 0x1000
/// Scene_TalkToActor's id: the actor id, under FIELD_TALK_SILENT.
#define FIELD_TALK_ACTOR_MASK 0xfff
/// FieldState.talk: set with the actor id under it to ask for that actor's
/// talk.
#define FIELD_TALK_REQUEST 0x4000
/// The frames of a scene's default fade in, from the backdrop colour.
#define MAP_TRANSITION_FRAMES 16

/// The values of FieldState.raisedTrigger that the field sequence handles
/// itself, rather than passing them to the scene (Scene_RunRaisedTrigger).
enum FieldRaisedTrigger {
  /// Party members fell to poison or hazardous ground this step.
  FIELD_RAISED_TRIGGER_PARTY_FELL = -1,
  /// Teleport: the picture of the whole world, shown once to choose where to
  /// go (FieldPsynergy_Teleport).
  FIELD_RAISED_TRIGGER_TELEPORT = -887,
  /// R on the world map: the picture of the whole world (MapScreen_Show).
  FIELD_RAISED_TRIGGER_MAP_SCREEN = -888,
  /// L on the world map: the view zooms out while L is held.
  FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM = -889,
  FIELD_RAISED_TRIGGER_ROPE_OFF = -890,
  FIELD_RAISED_TRIGGER_ROPE_ON = -891,
  FIELD_RAISED_TRIGGER_TIGHTROPE_OFF = -892,
  FIELD_RAISED_TRIGGER_TIGHTROPE_ON = -893
};

/// The raised trigger the menu keys pass to the scene while
/// FLAG_MENUS_RAISE_TRIGGER is set.
#define FIELD_RAISED_TRIGGER_MENUS 250

/// The raised triggers the world map's movement passes to its scene, whose
/// events change how the party travels.
enum WorldMapRaisedTrigger {
  /// A on the world map: WorldMap_BoardShip, the party boards the ship
  /// where it lies.
  WORLD_MAP_RAISED_BOARD_SHIP = 251,
  /// A while sailing: WorldMap_LeaveShip, the party goes ashore.
  WORLD_MAP_RAISED_LEAVE_SHIP = 252,
  /// A dash key while sailing, once the ship has its wings:
  /// WorldMap_SetUpFlyingShip, the ship takes off.
  WORLD_MAP_RAISED_TAKE_OFF = 253,
  /// The winged ship come down to the water: WorldMap_SetUpShip, it sails
  /// again.
  WORLD_MAP_RAISED_LAND = 254
};

/// How the actors are moved and drawn (FieldState.mode,
/// ActorSystem_Initialize).
enum ActorSystemMode {
  /// A town, a dungeon or the sprite viewer: the scrolling camera.
  ACTOR_SYSTEM_MAP = 2,
  /// The world map: the fixed camera, which projects 3D points onto the
  /// screen.
  ACTOR_SYSTEM_WORLD_MAP = 3,
  /// A battle: the fixed camera, and the battle's own motion update
  /// (Actor_UpdateAllMotion).
  ACTOR_SYSTEM_BATTLE = 4
};

/// The frames the leader pushes against a step trigger, a ledge, a gap or a
/// wall before it runs, jumps or climbs (FieldState.blockedSteps).
#define FIELD_PUSH_FRAMES 12

/// A field scene's state: FieldSequence_Run allocates it in HEAP_SLOT_FIELD,
/// zeroes it and frees it on the way out, and the field sequence, the event
/// commands and the scene overlays all reach it through
/// Ram_HeapSlots->fieldState.
struct FieldState {
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
  /// The exit taken, which ends the scene; EXIT_KEEP_DESTINATION for a warp.
  s16 exit;
  s16 fieldMenu; ///< open the field menu
  /// The A button: a talk, an examine or the field menu, by what is ahead.
  s16 aButton;
  s16 pauseMenu;   ///< the Start button: open the Pause menu
  s16 talk;        ///< an actor's talk, as actor id | FIELD_TALK_REQUEST
  s16 examineCell; ///< a cell's cell behavior, for Field_ExamineCell
  s16 encounter;   ///< the enemy group of a random encounter
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
  /// controls; step triggers and push cells need more than FIELD_PUSH_FRAMES.
  s16 blockedSteps;
  /// The keys held: a ledge (KEY_DOWN) and a climb (KEY_UP or KEY_DOWN) read
  /// them, and they are kept when a rope or tightrope was raised.
  u16 heldKeys;
  u8 unusedAfterHeldKeys[2]; ///< no code names it
  s8 mode;                   ///< an enum ActorSystemMode
  /// The area's graphics are to be reloaded on the World Map
  /// (WorldMap_Freeze, WorldMap_ReloadPendingAreaGraphics).
  s8 areaGraphicsPending;
  u8 encounterZones[8];     ///< encounter zone for each kind of ground
  s32 encounterBias;        ///< randomized distance adjustment between battles
  FIXED encounterThreshold; ///< encounter steps before a battle; 16 * FIXED_ONE
  /// Scales the leader's speed into walkingDistance, in 16.16 fixed point:
  /// 409 on entry, 0 in a town.
  FIXED walkingDistanceScale;
  /// Distance walked toward the next FIXED_ONE, at which the party regains PP,
  /// Djinn recover and poison hurts (Field_UpdateWalking).
  FIXED walkingDistance;
  struct MapCell *currentCell;  ///< the cell the leader last stepped onto
  struct MapCell *previousCell; ///< the cell before it
  /// A SCENE_TRANSITION(style, variant): a TRANSITION_BACKDROP_FADE on entry.
  s32 transition;
  u16 unused;
  u16 transitionActive; ///< set once the opening transition has started
  /// The transition's frames: MAP_TRANSITION_FRAMES on entry and at each event.
  s32 transitionFrames;
  s32 fastForward; ///< debug: skip timed event waits and actor messages
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
  /// The leader's two-row actor placement table (its row and an end row),
  /// which Actor_ResetForLeader builds.
  struct ActorPlacement leaderPlacements[2];
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
  /// FIELD_PSYNERGY_RING's ring is shown (FieldPsynergy_Ring); A, Select,
  /// Start or a shortcut ends it.
  s8 ringActive;
  s8 ringTimerRunning; ///< off while another field Psynergy is cast
  s16 ringFrames;      ///< frames left of the ring; 0 ends it
  /// The first of the two names the next yes/no choice shows in place of Yes
  /// and No (YesNoMenu_Run), 0 for those.
  s16 choiceMessage;
  s16 choiceWidth;     ///< the choice window's width, 0 for the default
  s16 choiceSelection; ///< the answer the choice starts on
  s8 psynergyActing;   ///< set while Psynergy takes effect
  s8 castStarted;      ///< set once a cast has been announced
  s16 queuedSound;     ///< a sound Scene_PlayQueuedMusic plays; -1 for none
  u8 unusedAfterQueuedSound[2]; ///< no code names it
  struct Actor *movedBlock;     ///< the block Actor_LiftTargetBlock lifted, for
                                ///< Actor_PlaceMovedBlock
  /// The cells a Psynergy touches, as cell coordinates, ended by -1.
  s16 psynergyCells[56];
  s8 psynergyHitObject;
  s8 targetX;
  s8 targetY;
  s8 targetZ;
  /// The item Scoop dug up (or its coins, SCOOPED_COINS | count), given when
  /// the leader examines it (SCOOPED_ITEM_ACTOR).
  s16 scoopedItem;
  s8 sceneAttributeLow;
  s8 sceneAttributeHigh;
  s8 blazeLongSpray;
  s8 hoverReady; ///< a party member knows Hover and the ship flies
  s8 terrainKind;
  s8 climbing;
  /// The centre of the Cyclone teleporter cell (kind 2) beside the caster,
  /// in pixels, that the Cyclone carries the leader to; -1 for none.
  s16 cycloneCarryX;
  s16 cycloneCarryZ;
  /// The centre of the Cyclone lift cell (kind 3) beside the caster, in
  /// pixels, that the Cyclone lifts the leader onto; -1 for none.
  s16 cycloneLiftX;
  s16 cycloneLiftZ;
  s16 glow;
  s16 glowHeld;
  s16 standing;
  u16 sandTrailPriority;
  u16 psynergyDrawFlags;
  u8 padding[2];
};

/// FieldState.psynergyDrawFlags, which a scene sets for Pound and Scoop to
/// draw in its room.
#define PSYNERGY_DRAW_NO_SCENE_SPRITE 0x1 ///< nor Pound's glow on the ground
/// The fist or hand at priority 1, not sorted by depth.
#define PSYNERGY_DRAW_FIXED_PRIORITY 0x2

STATIC_ASSERT(sizeof(struct FieldState) == 3408);
STATIC_ASSERT(offsetof(struct FieldState, triggers) == 16);
STATIC_ASSERT(offsetof(struct FieldState, actors) == 20);
STATIC_ASSERT(offsetof(struct FieldState, stepTrigger) == 340);
STATIC_ASSERT(offsetof(struct FieldState, exit) == 344);
STATIC_ASSERT(offsetof(struct FieldState, encounter) == 356);
STATIC_ASSERT(offsetof(struct FieldState, raisedTrigger) == 362);
STATIC_ASSERT(offsetof(struct FieldState, blockedSteps) == 388);
STATIC_ASSERT(offsetof(struct FieldState, mode) == 394);
STATIC_ASSERT(offsetof(struct FieldState, encounterZones) == 396);
STATIC_ASSERT(offsetof(struct FieldState, transition) == 428);
STATIC_ASSERT(offsetof(struct FieldState, transitionFrames) == 436);
STATIC_ASSERT(offsetof(struct FieldState, message) == 452);
STATIC_ASSERT(offsetof(struct FieldState, camera) == 460);
STATIC_ASSERT(offsetof(struct FieldState, speaker) == 480);
STATIC_ASSERT(offsetof(struct FieldState, leaderPlacements) == 492);
STATIC_ASSERT(offsetof(struct FieldState, noticeWindow) == 540);
STATIC_ASSERT(offsetof(struct FieldState, savedFadeTarget) == 546);
STATIC_ASSERT(offsetof(struct FieldState, busy) == 3234);
STATIC_ASSERT(offsetof(struct FieldState, revealActive) == 3236);
STATIC_ASSERT(offsetof(struct FieldState, choiceMessage) == 3248);
STATIC_ASSERT(offsetof(struct FieldState, psynergyActing) == 3254);
STATIC_ASSERT(offsetof(struct FieldState, queuedSound) == 3256);
STATIC_ASSERT(offsetof(struct FieldState, psynergyCells) == 3264);
STATIC_ASSERT(offsetof(struct FieldState, targetX) == 3377);
STATIC_ASSERT(offsetof(struct FieldState, sceneAttributeLow) == 3382);
STATIC_ASSERT(offsetof(struct FieldState, hoverReady) == 3385);
STATIC_ASSERT(offsetof(struct FieldState, glow) == 3396);
STATIC_ASSERT(offsetof(struct FieldState, psynergyDrawFlags) == 3404);

/// The running field scene's state, in HEAP_SLOT_FIELD; FieldSequence_Run takes
/// it and frees it.
#define gFieldState ((struct FieldState *)Ram_HeapSlots->fieldState)

#endif
