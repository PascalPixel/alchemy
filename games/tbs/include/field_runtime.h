#ifndef ALCHEMY_FIELD_RUNTIME_H
#define ALCHEMY_FIELD_RUNTIME_H

#include "types.h"
#include "layout_guard.h"
#include "object_runtime.h"
#include "field_scene.h"

struct MapCell;

struct SceneRecord
{
	s16 resource_id;
	s8	group;
	s8	variant;
	u16 map_index;
	u16 reserved;
};

struct SceneRegion
{
	s16 min_x, min_y, min_z;
	s16 max_x, max_y, max_z;
	s16 condition;
	s16 value;
};

struct SceneEntries
{
	u32 initialize_veneer;
	s32 (*initialize)(void);
	u32 entrances_veneer;
	const struct SceneEntrance *(*entrances)(void);
	u32 exits_veneer;
	const u32 *(*exits)(void);
	u32 placements_veneer;
	const struct ScenePlacement *(*placements)(void);
	u32 events_veneer;
	const struct SceneEvent *(*events)(void);
	u32 regions_veneer;
	const struct SceneRegion *(*regions)(void);
};

struct ObjectSlotTable
{
	u8					  unknown_00[0x14];
	struct ObjectRuntime *slots[0];
};

struct FieldStepWork
{
	const struct ScenePlacement *tables[4];
	const struct SceneEvent		*events;
	struct ObjectRuntime		*actors[66];
	u8							 unknown_11c[0x50];
	s16							 step_event;
	s16							 warp_event;
	u8							 unknown_170[0xc];
	u16							 unknown_17c;
	s16							 psynergy_request;
	u8							 unknown_180[2];
	u16							 raised_trigger;
	s16							 fallen_count;
	s16							 standing_count;
	u16							 fallen[11];
	s16							 mode;
	u8							 encounter_full[4];
	u8							 unknown_1a4[0xc];
	s32							 encounter_rate;
	s32							 encounter_steps;
	struct MapCell				*tile;
	struct MapCell				*previous_tile;
};

struct FieldPosition
{
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
LAYOUT_OFFSET_GUARD(FieldStepWork_StepEvent, struct FieldStepWork, step_event, 0x16c);
LAYOUT_OFFSET_GUARD(FieldStepWork_Mode, struct FieldStepWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(FieldStepWork_EncounterRate, struct FieldStepWork, encounter_rate, 0x1b0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Tile, struct FieldStepWork, tile, 0x1b8);
LAYOUT_SIZE_GUARD(FieldPosition_Size, struct FieldPosition, 12);

extern const struct SceneRecord Field_SceneTable[];
extern struct SceneEntries		gOverlayArea;

#endif
