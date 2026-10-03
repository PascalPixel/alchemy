#ifndef ALCHEMY_FIELDRUN_H
#define ALCHEMY_FIELDRUN_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "FIELD_SCENE.H"

/* One row emitted by FIELD/SCENES.S. */
struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    u16 reserved;
};

/* A scene's rectangular volume and the event it raises when entered. */
struct SceneRegion {
    s16 min_x, min_y, min_z;
    s16 max_x, max_y, max_z;
    s16 condition;
    s16 value;
};

/* ENTRY.S emits six 8-byte veneers. Each target follows its two Thumb
   instructions; this is the loaded veneer bank, not a pointer table. */
struct SceneEntries {
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

/* Field-mode step work. Its 66 actor slots cover the eight party ids and
   the 58 placements; battle and menu lifetimes keep their own views. */
struct FieldStepWork {
    u8 unknown_000[0x10];
    const struct SceneEvent *events;
    struct ObjectRuntime *actors[66];
    u8 unknown_11c[0x50];
    s16 step_event;
    s16 warp_event;
    u8 unknown_170[0xc];
    u16 unknown_17c;
    s16 unknown_17e;
    u8 unknown_180[2];
    u16 unknown_182;
    s16 fallen_count;
    s16 standing_count;
    u16 fallen[11];
    s16 mode;
    u8 encounter_full[4];
    u8 unknown_1a4[0xc];
    s32 encounter_rate;
    s32 encounter_steps;
    struct MapCell *tile;
    struct MapCell *previous_tile;
};

/* A three-coordinate 16.16 position passed to the field vector routines. */
struct FieldPosition { s32 x, y, z; };

LAYOUT_SIZE_GUARD(SceneRecord_Size, struct SceneRecord, 8);
LAYOUT_SIZE_GUARD(SceneRegion_Size, struct SceneRegion, 16);
LAYOUT_SIZE_GUARD(SceneEntries_Size, struct SceneEntries, 48);
LAYOUT_OFFSET_GUARD(SceneEntries_Exits, struct SceneEntries, exits, 0x14);
LAYOUT_OFFSET_GUARD(SceneEntries_Regions, struct SceneEntries, regions, 0x2c);
LAYOUT_SIZE_GUARD(FieldStepWork_Size, struct FieldStepWork, 0x1c0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Actors, struct FieldStepWork, actors, 0x14);
LAYOUT_OFFSET_GUARD(FieldStepWork_StepEvent, struct FieldStepWork, step_event, 0x16c);
LAYOUT_OFFSET_GUARD(FieldStepWork_Mode, struct FieldStepWork, mode, 0x19e);
LAYOUT_OFFSET_GUARD(FieldStepWork_EncounterRate, struct FieldStepWork, encounter_rate, 0x1b0);
LAYOUT_OFFSET_GUARD(FieldStepWork_Tile, struct FieldStepWork, tile, 0x1b8);
LAYOUT_SIZE_GUARD(FieldPosition_Size, struct FieldPosition, 12);

extern const struct SceneRecord Field_SceneTable[];
extern struct SceneEntries gOverlayArea;

#endif
