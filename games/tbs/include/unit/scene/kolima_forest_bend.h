#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

/// A log's saved record: its sprite, first map cell and orientation, then its
/// actor once created.
struct BendLog {
  s16 sprite;               ///< sprite number; -1 ends the list
  s16 x;                    ///< map cells
  s16 z;                    ///< map cells
  s16 vertical;             ///< nonzero runs north to south; zero west to east
  struct FieldActor *actor; ///< created for this row; NULL in the defaults
};

STATIC_ASSERT(sizeof(struct BendLog) == 12);
STATIC_ASSERT(offsetof(struct BendLog, x) == 2);
STATIC_ASSERT(offsetof(struct BendLog, vertical) == 6);
STATIC_ASSERT(offsetof(struct BendLog, actor) == 8);

const struct SceneEntrance *KolimaForestBend_GetEntrances(void);
const struct SceneRegion *KolimaForestBend_GetRegions(void);
const u32 *KolimaForestBend_GetExits(void);
const struct ScenePlacement *KolimaForestBend_GetPlacements(void);
const struct SceneEvent *KolimaForestBend_GetEvents(void);
s32 KolimaForestBend_Initialize(void);
// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void State_StampRecordCells(const struct BendLog *records, s32 value);
#endif
extern struct BendLog gKolimaForestBend_DefaultRecords[];
extern u8 gKolimaForestBend_PushAnimations[];
extern s8 gKolimaForestBend_PushStepX[];
extern s8 gKolimaForestBend_PushStepZ[];
extern struct SceneEntrance gKolimaForestBendEntrances[];
extern u32 gKolimaForestBendExits[];
extern struct ScenePlacement gKolimaForestBendPlacements[];
extern struct SceneEvent gKolimaForestBendEvents[];

#endif
