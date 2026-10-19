#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

/// @file
/// The field's runtime work: map cells, scene records and regions, map markers
/// and FieldWork.

#include "types.h"
#include "static_assert.h"
#include "object_runtime.h"
#include "field_scene.h"

struct MapCell;
struct FieldActor;

/// A scene's row in gField_SceneTable: its overlay's resource, its group
/// (the place it belongs to), a variant and its map.
struct SceneRecord {
  s16 resourceId;
  s8 group;
  s8 variant; ///< copied to gGameState.sceneVariant; 2 sets flag 0x123
  u16 mapIndex;
  u16 reserved; ///< 0 in every row; not read
};

STATIC_ASSERT(sizeof(struct SceneRecord) == 8);

/// A scene's exit region: the box that, while its condition holds, takes the
/// party leader out through its exit.
struct SceneRegion {
  s16 minX, minY, minZ; ///< pixels, inclusive; minX -1 ends the table
  s16 maxX, maxY, maxZ; ///< pixels, exclusive
  s16 condition;        ///< -1 always, a flag | 0x1000 set, else the flag clear
  s16 value;            ///< the exit kind, stored in FieldWork's exit
};

STATIC_ASSERT(sizeof(struct SceneRegion) == 16);

/// The entry table at the start of each scene overlay: its initialize
/// function and the functions returning its entrances, exits, placements,
/// events and regions, each behind a veneer word.
struct SceneEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 entrancesVeneer;
  const struct SceneEntrance *(*entrances)(void);
  u32 exitsVeneer;
  const u32 *(*exits)(void);
  u32 placementsVeneer;
  const struct ScenePlacement *(*placements)(void);
  u32 eventsVeneer;
  const struct SceneEvent *(*events)(void);
  u32 regionsVeneer;
  const struct SceneRegion *(*regions)(void);
};

STATIC_ASSERT(sizeof(struct SceneEntries) == 48);
STATIC_ASSERT(offsetof(struct SceneEntries, exits) == 0x14);
STATIC_ASSERT(offsetof(struct SceneEntries, regions) == 0x2c);

/// A map marker placed as an actor: a chest or a Psynergy stone.
struct MarkerSlot {
  struct ObjectRuntime *object;
  u8 id;     ///< the trigger of its scene event
  u8 unk05;  ///< cleared with the slot; not named by TBS code
  u8 column; ///< its 16-pixel cell
  u8 row;
};

STATIC_ASSERT(sizeof(struct MarkerSlot) == 8);
STATIC_ASSERT(offsetof(struct MarkerSlot, id) == 4);
STATIC_ASSERT(offsetof(struct MarkerSlot, column) == 6);
STATIC_ASSERT(offsetof(struct MarkerSlot, row) == 7);

/// Full event work allocated by Field_RunScene (heap slot 27). The twelve
/// requests from action to event are handled by the field loop.
struct FieldWork {
  /// The placement tables placed so far, recorded by Event_SpawnObjectTable.
  struct ScenePlacement *tables[4];
  const struct SceneEvent *events; ///< the scene's event table
  /// The actors by id: 0 to 7 the party's characters, 8 on the placed actors.
  struct FieldActor *actors[66];
  struct MarkerSlot markers[10]; ///< the map's markers (MARKER_MAX)
  /// The twelve requests: each nonzero value asks the field loop for one
  /// thing; Field_ClearRequests clears them and Field_CountRequests sums them.
  ///@{
  s16 action;     ///< a step event (1 to 239) for Scene_RunTouchOrExit
  s16 input;      ///< a tile code (252 to 254) for Field_RunPushTileEvent
  s16 exit;       ///< the exit taken, which ends the scene
  s16 menu;       ///< open the main menu
  s16 examine;    ///< the A button: examine what is ahead
  s16 start;      ///< the Start button: open the pause menu
  u16 descriptor; ///< an actor's event, as actor | 0x1000
  s16 tile;       ///< a tile's collision code, for Field_SearchTile
  s16 battle;     ///< the enemy group of a random encounter
  s16 command;    ///< a field Psynergy to use, as owner << 10 | Psynergy
  s16 item;       ///< an item to use, packed as Field_UseItem reads it
  /// A raised event for Scene_RunRaisedEvent, or -1 when characters fell to
  /// poison or hazard damage, -888 the world map, -889 a visibility change.
  s16 event;
  ///@}
  s16 fallenCount;     ///< the characters who fell this step
  s16 survivorCount;   ///< 0 when the whole party fell
  s16 fallenOwners[9]; ///< the characters who fell, from the active party
  s16 itemArgument;    ///< the inventory slot Field_UseItem receives
  /// Frames the leader has pushed against something, counted by the player
  /// controls; touch events and push tiles need more than 12.
  s16 blockedSteps;
  s16 mode;                 ///< MODE_WORLD_MAP (3) on the world map
  u8 encounterGroups[8];    ///< encounter zone for each kind of ground
  s32 encounterBias;        ///< randomized distance adjustment between battles
  s32 encounterThreshold;   ///< encounter steps before a battle; 0x100000
  s32 walkingDistanceScale; ///< scales movement for PP and status updates
  s32 walkingDistance;      ///< accumulated 16.16 distance awaiting an update
  struct MapCell *currentTile;  ///< the cell the leader last stepped onto
  struct MapCell *previousTile; ///< the cell before it
  s32 transition;       ///< SCENE_TRANSITION(style, variant); 0x100 on entry
  u16 unk1c4;           ///< not named by TBS code
  u16 transitionActive; ///< set once the opening transition has started
  s32 transitionFrames; ///< frames; 16 on entry and at each event
  s32 fastForward;      ///< debug: skip timed event waits and actor messages
  u8 unk1d0[8];         ///< not named by TBS code
  s16 message;          ///< the next message, advanced as each window opens
  u16 unk1da;           ///< set to 0xffff by FieldEvent_Begin; not read by name
  s16 unk1dc;           ///< set to -1 by FieldEvent_Begin; not read by name
  s16 unk1de;           ///< set to -1 by FieldEvent_Begin; not read by name
  struct ObjectRuntime *camera; ///< the object the map view follows
  u8 unk1e4[0x10];              ///< not named by TBS code
  s32 speaker;       ///< the actor speaking; the leader at each event
  s32 messageWindow; ///< the open message window, 0 for none
  s32 sideWindow;    ///< the speaker's portrait window
  /// ObjectTable_ResetForObject builds the leader's two-row placement table
  /// at its start; nothing else names it.
  u8 unk200[0xab6];
  /// Set while a raised or touch event runs; FieldEvent_Begin then resets the
  /// effect counter.
  u16 busy;
  s16 revealActive;  ///< nonzero while Reveal is active
  s16 revealFrames;  ///< the frames Reveal has left
  s16 revealCenterX; ///< where Reveal was cast, in pixels
  s16 revealCenterY;
  /// Nonzero lets the Reveal countdown advance; menus and other effects pause
  /// it.
  s16 effectTimerRunning;
  s16 nameMessage;    ///< first message of the menu's name window (0: none)
  s16 nameWidth;      ///< width of the name window (0: the default)
  s8 resolvingAction; ///< set while Psynergy takes effect
  s8 castStarted;     ///< set once a cast has been announced
  s16 queuedSound;    ///< a sound BattleFx_PlayQueuedSound plays; -1 for none
};

STATIC_ASSERT(sizeof(struct FieldWork) == 0xccc);
STATIC_ASSERT(offsetof(struct FieldWork, tables) == 0);
STATIC_ASSERT(offsetof(struct FieldWork, events) == 0x10);
STATIC_ASSERT(offsetof(struct FieldWork, actors) == 0x14);
STATIC_ASSERT(offsetof(struct FieldWork, markers) == 0x11c);
STATIC_ASSERT(offsetof(struct FieldWork, action) == 0x16c);
STATIC_ASSERT(offsetof(struct FieldWork, descriptor) == 0x178);
STATIC_ASSERT(offsetof(struct FieldWork, battle) == 0x17c);
STATIC_ASSERT(offsetof(struct FieldWork, event) == 0x182);
STATIC_ASSERT(offsetof(struct FieldWork, blockedSteps) == 0x19c);
STATIC_ASSERT(offsetof(struct FieldWork, mode) == 0x19e);
STATIC_ASSERT(offsetof(struct FieldWork, encounterGroups) == 0x1a0);
STATIC_ASSERT(offsetof(struct FieldWork, encounterBias) == 0x1a8);
STATIC_ASSERT(offsetof(struct FieldWork, encounterThreshold) == 0x1ac);
STATIC_ASSERT(offsetof(struct FieldWork, walkingDistanceScale) == 0x1b0);
STATIC_ASSERT(offsetof(struct FieldWork, walkingDistance) == 0x1b4);
STATIC_ASSERT(offsetof(struct FieldWork, currentTile) == 0x1b8);
STATIC_ASSERT(offsetof(struct FieldWork, previousTile) == 0x1bc);
STATIC_ASSERT(offsetof(struct FieldWork, transition) == 0x1c0);
STATIC_ASSERT(offsetof(struct FieldWork, transitionActive) == 0x1c6);
STATIC_ASSERT(offsetof(struct FieldWork, transitionFrames) == 0x1c8);
STATIC_ASSERT(offsetof(struct FieldWork, fastForward) == 0x1cc);
STATIC_ASSERT(offsetof(struct FieldWork, message) == 0x1d8);
STATIC_ASSERT(offsetof(struct FieldWork, camera) == 0x1e0);
STATIC_ASSERT(offsetof(struct FieldWork, unk1da) == 0x1da);
STATIC_ASSERT(offsetof(struct FieldWork, unk1dc) == 0x1dc);
STATIC_ASSERT(offsetof(struct FieldWork, unk1de) == 0x1de);
STATIC_ASSERT(offsetof(struct FieldWork, speaker) == 0x1f4);
STATIC_ASSERT(offsetof(struct FieldWork, messageWindow) == 0x1f8);
STATIC_ASSERT(offsetof(struct FieldWork, sideWindow) == 0x1fc);
STATIC_ASSERT(offsetof(struct FieldWork, busy) == 0xcb6);
STATIC_ASSERT(offsetof(struct FieldWork, revealActive) == 0xcb8);
STATIC_ASSERT(offsetof(struct FieldWork, revealFrames) == 0xcba);
STATIC_ASSERT(offsetof(struct FieldWork, revealCenterX) == 0xcbc);
STATIC_ASSERT(offsetof(struct FieldWork, revealCenterY) == 0xcbe);
STATIC_ASSERT(offsetof(struct FieldWork, effectTimerRunning) == 0xcc0);
STATIC_ASSERT(offsetof(struct FieldWork, nameMessage) == 0xcc2);
STATIC_ASSERT(offsetof(struct FieldWork, nameWidth) == 0xcc4);
STATIC_ASSERT(offsetof(struct FieldWork, resolvingAction) == 0xcc6);
STATIC_ASSERT(offsetof(struct FieldWork, castStarted) == 0xcc7);
STATIC_ASSERT(offsetof(struct FieldWork, queuedSound) == 0xcc8);

/// A position on the field.
struct FieldPosition {
  s32 x, y, z; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct FieldPosition) == 12);

extern const struct SceneRecord gField_SceneTable[];
extern struct SceneEntries gOverlayArea;

#endif
