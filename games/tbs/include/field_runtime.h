#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

#include "types.h"
#include "layout_guard.h"
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

struct SceneRegion {
  s16 minX, minY, minZ;
  s16 maxX, maxY, maxZ;
  s16 condition;
  s16 value;
};

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

struct ObjectSlotTable {
  u8 unk00[0x14];
  struct ObjectRuntime *slots[0];
};

struct FieldStepWork {
  const struct ScenePlacement *tables[4];
  const struct SceneEvent *events;
  struct ObjectRuntime *actors[66];
  u8 unk11c[0x50];
  s16 stepEvent;
  s16 warpEvent;
  u8 unk170[0xc];
  u16 encounterEnemyGroup; // the next battle's enemy group, 0 for none
  s16 psynergyRequest;
  u8 unk180[2];
  u16 raisedTrigger;
  s16 fallenCount;
  s16 standingCount;
  u16 fallen[11];
  s16 mode;
  u8 encounterFull[4];
  u8 unk1a4[0xc];
  s32 encounterRate;
  s32 encounterSteps;
  struct MapCell *tile;
  struct MapCell *previousTile;
};

// Full event work allocated by Field_RunScene (heap slot 27). The twelve
// requests from action to event are handled by the field loop.
struct FieldWork {
  u8 unk000[0x10];
  s32 view;
  struct FieldActor *actors[86];
  s16 action;     // a scene action to run
  s16 input;      // an input event to dispatch
  s16 exit;       // the exit taken, which ends the scene
  s16 menu;       // open the main menu
  s16 examine;    // the A button: examine what is ahead
  s16 start;      // the Start button: open the pause menu
  u16 descriptor; // an actor's event, as actor | 0x1000
  s16 tile;       // a tile's event
  s16 battle;     // the enemy group of a random encounter
  s16 command;    // a field Psynergy to use
  s16 item;       // an item to use
  s16 event;      // a special event: -1 for poison or hazard damage
  s16 count;      // the characters who fell
  s16 survived;   // 0 when the whole party fell
  s16 quantities[9];
  s16 itemArgument;
  u8 unk19c[2];
  s16 mode;
  u8 unk1a0[8];
  s32 unk1a8;
  u8 unk1ac[4];
  s32 unk1b0;
  s32 unk1b4;
  u8 unk1b8[4];
  u8 *cells;
  s32 transition;
  u16 unk1c4;
  u16 transitionActive;
  s32 transitionFrames;
  u8 unk1cc[0xaea];
  u16 busy;
  s16 revealActive;  // nonzero while Reveal is active
  s16 revealFrames;  // the frames Reveal has left
  s16 revealCenterX; // where Reveal was cast, in pixels
  s16 revealCenterY;
  // Nonzero lets the Reveal countdown advance; menus and other effects pause
  // it.
  s16 effectTimerRunning;
  u8 unkcc2[4];
  s8 resolvingAction; // set while a Psynergy's effect plays
  s8 castStarted;     // set once a cast has been announced
  s16 queuedSound;
};

struct FieldPosition {
  s32 x, y, z;
};

LAYOUT_SIZE_GUARD(SceneRecord_Size, struct SceneRecord, 8);
LAYOUT_SIZE_GUARD(SceneRegion_Size, struct SceneRegion, 16);
LAYOUT_SIZE_GUARD(SceneEntries_Size, struct SceneEntries, 48);
LAYOUT_OFFSET_GUARD(SceneEntries_Exits, struct SceneEntries, exits, 0x14);
LAYOUT_OFFSET_GUARD(SceneEntries_Regions, struct SceneEntries, regions, 0x2c);
LAYOUT_OFFSET_GUARD(ObjectSlotTable_Slots, struct ObjectSlotTable, slots, 0x14);
LAYOUT_SIZE_GUARD(FieldStepWork_Size, struct FieldStepWork, 0x1c0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Events, struct FieldStepWork, events, 0x10);
LAYOUT_OFFSET_GUARD(FieldStepWork_Actors, struct FieldStepWork, actors, 0x14);
LAYOUT_OFFSET_GUARD(FieldStepWork_StepEvent, struct FieldStepWork, stepEvent,
                    0x16c);
LAYOUT_OFFSET_GUARD(FieldStepWork_EncounterEnemyGroup, struct FieldStepWork,
                    encounterEnemyGroup, 0x17c);
LAYOUT_OFFSET_GUARD(FieldStepWork_Mode, struct FieldStepWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(FieldStepWork_EncounterRate, struct FieldStepWork,
                    encounterRate, 0x1b0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Tile, struct FieldStepWork, tile, 0x1b8);
LAYOUT_SIZE_GUARD(FieldWork_Size, struct FieldWork, 0xccc);
LAYOUT_OFFSET_GUARD(FieldWork_Actors, struct FieldWork, actors, 0x14);
LAYOUT_OFFSET_GUARD(FieldWork_Action, struct FieldWork, action, 0x16c);
LAYOUT_OFFSET_GUARD(FieldWork_Descriptor, struct FieldWork, descriptor, 0x178);
LAYOUT_OFFSET_GUARD(FieldWork_Battle, struct FieldWork, battle, 0x17c);
LAYOUT_OFFSET_GUARD(FieldWork_Event, struct FieldWork, event, 0x182);
LAYOUT_OFFSET_GUARD(FieldWork_Mode, struct FieldWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(FieldWork_Transition, struct FieldWork, transition, 0x1c0);
LAYOUT_OFFSET_GUARD(FieldWork_TransitionActive, struct FieldWork,
                    transitionActive, 0x1c6);
LAYOUT_OFFSET_GUARD(FieldWork_TransitionFrames, struct FieldWork,
                    transitionFrames, 0x1c8);
LAYOUT_OFFSET_GUARD(FieldWork_Busy, struct FieldWork, busy, 0xcb6);
LAYOUT_OFFSET_GUARD(FieldWork_RevealActive, struct FieldWork, revealActive,
                    0xcb8);
LAYOUT_OFFSET_GUARD(FieldWork_RevealFrames, struct FieldWork, revealFrames,
                    0xcba);
LAYOUT_OFFSET_GUARD(FieldWork_RevealCenterX, struct FieldWork, revealCenterX,
                    0xcbc);
LAYOUT_OFFSET_GUARD(FieldWork_RevealCenterY, struct FieldWork, revealCenterY,
                    0xcbe);
LAYOUT_OFFSET_GUARD(FieldWork_EffectTimerRunning, struct FieldWork,
                    effectTimerRunning, 0xcc0);
LAYOUT_OFFSET_GUARD(FieldWork_ResolvingAction, struct FieldWork,
                    resolvingAction, 0xcc6);
LAYOUT_OFFSET_GUARD(FieldWork_CastStarted, struct FieldWork, castStarted,
                    0xcc7);
LAYOUT_OFFSET_GUARD(FieldWork_QueuedSound, struct FieldWork, queuedSound,
                    0xcc8);
LAYOUT_SIZE_GUARD(FieldPosition_Size, struct FieldPosition, 12);

extern const struct SceneRecord gField_SceneTable[];
extern struct SceneEntries gOverlayArea;

#endif
