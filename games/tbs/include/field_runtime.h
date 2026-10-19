#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

#include "types.h"
#include "static_assert.h"
#include "object_runtime.h"
#include "field_scene.h"

struct MapCell;
struct FieldActor;

struct SceneRecord {
  s16 resourceId;
  s8 group;
  s8 variant;
  u16 mapIndex;
  u16 reserved;
};

STATIC_ASSERT(sizeof(struct SceneRecord) == 8);

struct SceneRegion {
  s16 minX, minY, minZ;
  s16 maxX, maxY, maxZ;
  s16 condition;
  s16 value;
};

STATIC_ASSERT(sizeof(struct SceneRegion) == 16);

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
  u8 id; ///< the trigger of its scene event
  u8 unk05;
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
  struct ScenePlacement *tables[4];
  const struct SceneEvent *events;
  struct FieldActor *actors[66];
  struct MarkerSlot markers[10];
  s16 action;     ///< a scene action to run
  s16 input;      ///< an input event to dispatch
  s16 exit;       ///< the exit taken, which ends the scene
  s16 menu;       ///< open the main menu
  s16 examine;    ///< the A button: examine what is ahead
  s16 start;      ///< the Start button: open the pause menu
  u16 descriptor; ///< an actor's event, as actor | 0x1000
  s16 tile;       ///< a tile's event
  s16 battle;     ///< the enemy group of a random encounter
  s16 command;    ///< a field Psynergy to use
  s16 item;       ///< an item to use
  s16 event;      ///< a special event: -1 for poison or hazard damage
  s16 fallenCount;
  s16 survivorCount; ///< 0 when the whole party fell
  s16 fallenOwners[9];
  s16 itemArgument;
  s16 blockedSteps;
  s16 mode;
  u8 encounterGroups[8]; ///< encounter zone for each kind of ground
  s32 encounterBias;     ///< randomized distance adjustment between battles
  s32 encounterThreshold;
  s32 walkingDistanceScale; ///< scales movement for PP and status updates
  s32 walkingDistance;      ///< accumulated 16.16 distance awaiting an update
  struct MapCell *currentTile;
  struct MapCell *previousTile;
  s32 transition;
  u16 unk1c4;
  u16 transitionActive;
  s32 transitionFrames;
  s32 fastForward; ///< debug: skip timed event waits and actor messages
  u8 unk1d0[8];
  s16 message;
  u16 unk1da;
  s16 unk1dc;
  s16 unk1de;
  struct ObjectRuntime *camera;
  u8 unk1e4[0x10];
  s32 speaker;
  s32 messageWindow;
  s32 sideWindow;
  u8 unk200[0xab6];
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
  s16 queuedSound;
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

struct FieldPosition {
  s32 x, y, z;
};

STATIC_ASSERT(sizeof(struct FieldPosition) == 12);

extern const struct SceneRecord gField_SceneTable[];
extern struct SceneEntries gOverlayArea;

#endif
