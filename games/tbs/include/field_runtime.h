#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

/// @file
/// The field's runtime work: map grid cells, map records and warp regions, map
/// markers and FieldWork.

#include "types.h"
#include "static_assert.h"
#include "object_runtime.h"
#include "map_tables.h"

struct MapGridCell;
struct ObjectEvent;

/// A map's row in gMapHeaders: its overlay's resource, its map section,
/// its map type and its layout. One row per map number; field/maps.s writes
/// them, and its MapId_ symbols are the row numbers.
struct MapHeader {
  s16 resourceId; ///< the overlay Overlay_Load loads into gOverlayArea
  /// The map section: entering a new one clears the FLAG_MAPSEC_TEMP_ flags
  /// and the step counters and moves Retreat's destination;
  /// FLAG_MAPSEC_VISITED(mapSec) marks it visited (Map_ResetFlagsOnEnter).
  s8 mapSec;
  /// An enum MapType, copied to gGameState.mapType:
  /// MAP_TYPE_TOWN for towns and their interiors (53 rows: Vale,
  /// Bilibin, Kalay, Tolbi and others), MAP_TYPE_FIELD for the rest.
  s8 mapType;
  u16 layoutIndex; ///< the layout loaded for it, copied to gGameState.layoutId
  u16 reserved;    ///< 0 in every row; not read
};

STATIC_ASSERT(sizeof(struct MapHeader) == 8);

/// A map's warp region: the box that, while its condition holds, takes the
/// party leader out through its warp.
struct MapWarpRegion {
  s16 minX, minY, minZ; ///< pixels, inclusive; minX -1 ends the table
  s16 maxX, maxY, maxZ; ///< pixels, exclusive
  s16 condition;        ///< -1 always, a flag | 0x1000 set, else the flag clear
  s16 value;            ///< the warp kind, stored in FieldWork's warp
};

STATIC_ASSERT(sizeof(struct MapWarpRegion) == 16);

/// The entry table at the start of each map overlay: its initialize
/// function and the functions returning its warp destinations, warps, object
/// event templates, map events and warp regions, each behind a veneer word.
struct MapEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 warpDestsVeneer;
  const struct MapWarpDest *(*warpDests)(void);
  u32 warpsVeneer;
  const u32 *(*warps)(void);
  u32 objectEventTemplatesVeneer;
  const struct ObjectEventTemplate *(*objectEventTemplates)(void);
  u32 eventsVeneer;
  const struct MapEvent *(*events)(void);
  u32 regionsVeneer;
  const struct MapWarpRegion *(*regions)(void);
};

STATIC_ASSERT(sizeof(struct MapEntries) == 48);
STATIC_ASSERT(offsetof(struct MapEntries, warps) == 0x14);
STATIC_ASSERT(offsetof(struct MapEntries, regions) == 0x2c);

/// A map marker placed as an object event: a chest or a Psynergy stone.
struct MarkerSlot {
  struct Object *object; ///< the object event placed for it
  u8 id;                 ///< the trigger of its map event, 100 to 239
  u8 unused;             ///< cleared with the slot; no code names it
  u8 column;             ///< its 16-pixel cell
  u8 row;                ///< its 16-pixel cell
};

STATIC_ASSERT(sizeof(struct MarkerSlot) == 8);
STATIC_ASSERT(offsetof(struct MarkerSlot, id) == 4);
STATIC_ASSERT(offsetof(struct MarkerSlot, column) == 6);
STATIC_ASSERT(offsetof(struct MarkerSlot, row) == 7);

/// The values of FieldWork.event that the field loop handles itself, rather
/// than passing them to the map (Map_RunRaisedEvent).
enum FieldEventCode {
  /// Party members fell to poison or hazardous ground this step.
  FIELD_EVENT_PARTY_FELL = -1,
  /// R on the world map: the picture of the whole world (WorldMap_Show).
  FIELD_EVENT_WORLD_MAP_PICTURE = -888,
  /// L on the world map: the view zooms out while L is held
  /// (FieldEffect_RunVisibilityTransition).
  FIELD_EVENT_WORLD_MAP_ZOOM = -889
};

/// A field map's work, all of it: Map_Run allocates it in heap slot
/// 27 (HEAP_SLOT_FIELD), zeroes it and frees it on the way out. The map
/// overlays read it as struct EventWork through gFieldWork. The twelve
/// requests from action to event are handled by the field loop.
struct FieldWork {
  /// The object event template tables placed so far, recorded by
  /// FieldScript_SpawnObjectTable.
  struct ObjectEventTemplate *tables[4];
  const struct MapEvent *events; ///< the map's event table
  /// The object events by local id: 0 to 7 the party's characters, 8 on the
  /// placed object events.
  struct ObjectEvent *objectEvents[66];
  struct MarkerSlot markers[10]; ///< the map's markers (MARKER_MAX)
  /// The twelve requests: each nonzero value asks the field loop for one
  /// thing; Field_ClearRequests clears them and Field_CountRequests sums them.
  ///@{
  s16 action;   ///< a step event (1 to 239) for Map_RunCoordEventOrWarp
  s16 pushTile; ///< a metatile behavior (252 to 254) for Field_RunPushTileEvent
  s16 warp;     ///< the warp taken, which ends the map
  s16 menu;     ///< open the main menu
  s16 examine;  ///< the A button: examine what is ahead
  s16 start;    ///< the Start button: open the pause menu
  u16 talk;     ///< an object event's talk event, as local id | 0x1000
  s16 tile;     ///< a cell's metatile behavior, for Field_SearchTile
  s16 battle;   ///< the enemy group of a random encounter
  s16 psynergy; ///< a field Psynergy to use, as owner << 10 | Psynergy
  s16 item;     ///< an item to use, packed as Field_UseItem reads it
  /// A raised event for Map_RunRaisedEvent, or an enum FieldEventCode.
  s16 event;
  ///@}
  s16 fallenCount;     ///< the characters who fell this step
  s16 survivorCount;   ///< 0 when the whole party fell
  s16 fallenOwners[9]; ///< the characters who fell, from the active party
  s16 itemArgument;    ///< the inventory slot Field_UseItem receives
  /// Frames the leader has pushed against something, counted by the player
  /// controls; coord events and push tiles need more than 12.
  s16 blockedSteps;
  s16 mode;               ///< an enum ObjectSystemMode
  u8 encounterGroups[8];  ///< encounter zone for each kind of ground
  s32 encounterBias;      ///< randomized distance adjustment between battles
  s32 encounterThreshold; ///< encounter steps before a battle; 0x100000
  /// Scales the leader's speed into walkingDistance, 16.16 fixed point: 0x199
  /// (about 1/160) on entry, 0 in a town.
  s32 walkingDistanceScale;
  /// Distance walked toward the next 0x10000, at which the party regains PP,
  /// Djinn recover and poison hurts (Field_ProcessStep).
  s32 walkingDistance;
  struct MapGridCell *currentTile;  ///< the cell the leader last stepped onto
  struct MapGridCell *previousTile; ///< the cell before it
  s32 transition;       ///< MAP_TRANSITION(style, variant); 0x100 on entry
  u16 unused;           ///< no code names it
  u16 transitionActive; ///< set once the opening transition has started
  s32 transitionFrames; ///< frames; 16 on entry and at each script
  s32 fastForward; ///< debug: skip timed script waits and object event messages
  u8 unusedAfterFastForward[8]; ///< no code names it
  s16 message; ///< the next message, advanced as each window opens
  /// XXX: set to 0xffff by FieldScript_Start and never read (see the field
  /// README).
  u16 unk1da;
  s16 unk1dc;            ///< XXX: set to -1 by FieldScript_Start and never read
  s16 unk1de;            ///< XXX: set to -1 by FieldScript_Start and never read
  struct Object *camera; ///< the object the map view follows
  u8 unusedAfterCamera[0x10]; ///< no code names it
  s32 speaker;       ///< the object event speaking; the leader at each script
  s32 messageWindow; ///< the open message window, 0 for none
  s32 sideWindow;    ///< the speaker's portrait window
  /// The leader's two-row object event template table (its row and an end
  /// row), which ObjectTable_ResetForObject builds through its offset.
  u8 leaderTemplates[0x30];
  u8 unusedAfterTemplates[0xa86]; ///< no code names it
  /// Set while a raised or coord event runs; FieldScript_Start then resets the
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
  s16 queuedSound; ///< a sound FieldEffect_PlayQueuedSound plays; -1 for none
};

STATIC_ASSERT(sizeof(struct FieldWork) == 0xccc);
STATIC_ASSERT(offsetof(struct FieldWork, tables) == 0);
STATIC_ASSERT(offsetof(struct FieldWork, events) == 0x10);
STATIC_ASSERT(offsetof(struct FieldWork, objectEvents) == 0x14);
STATIC_ASSERT(offsetof(struct FieldWork, markers) == 0x11c);
STATIC_ASSERT(offsetof(struct FieldWork, action) == 0x16c);
STATIC_ASSERT(offsetof(struct FieldWork, talk) == 0x178);
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

/// Every map's record, by map number (field/maps.s).
extern const struct MapHeader gMapHeaders[];
/// The running map's overlay, loaded into these 0x8000 bytes of EWRAM at
/// each map change; its code opens with its entry table.
extern struct MapEntries gOverlayArea;

#endif
