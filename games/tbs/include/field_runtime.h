#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

#include "types.h"
#include "layout_guard.h"
#include "object_runtime.h"
#include "field_scene.h"

struct MapCell;

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
  u16 unk17c;
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
LAYOUT_OFFSET_GUARD(FieldStepWork_Mode, struct FieldStepWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(FieldStepWork_EncounterRate, struct FieldStepWork,
                    encounterRate, 0x1b0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Tile, struct FieldStepWork, tile, 0x1b8);
LAYOUT_SIZE_GUARD(FieldPosition_Size, struct FieldPosition, 12);

extern const struct SceneRecord gField_SceneTable[];
extern struct SceneEntries gOverlayArea;

#endif
