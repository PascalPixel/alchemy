#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_FLOODGATE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_FLOODGATE_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;

/// A log's saved record: its sprite, first map cell and orientation, then its
/// actor once created.
struct FloodgateLog {
  s16 sprite;               ///< sprite number; -1 ends the list
  s16 x;                    ///< map cells
  s16 z;                    ///< map cells
  s16 vertical;             ///< nonzero runs north to south; zero west to east
  struct FieldActor *actor; ///< created for this row; NULL in the defaults
};

STATIC_ASSERT(sizeof(struct FloodgateLog) == 12);
STATIC_ASSERT(offsetof(struct FloodgateLog, x) == 2);
STATIC_ASSERT(offsetof(struct FloodgateLog, vertical) == 6);
STATIC_ASSERT(offsetof(struct FloodgateLog, actor) == 8);

const struct SceneEntrance *KolimaForestFloodgate_GetEntrances(void);
const struct SceneRegion *KolimaForestFloodgate_GetRegions(void);
const u32 *KolimaForestFloodgate_GetExits(void);
const struct ScenePlacement *KolimaForestFloodgate_GetPlacements(void);
const struct SceneEvent *KolimaForestFloodgate_GetEvents(void);
s32 KolimaForestFloodgate_Initialize(void);
// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void KolimaForestFloodgate_StampLogCells(const struct FloodgateLog *records,
                                         s32 value);
#endif
extern struct FloodgateLog gKolimaForestFloodgate_DefaultRecords[];
extern u8 gKolimaForestFloodgate_PushAnimations[];
extern s8 gKolimaForestFloodgate_PushStepX[];
extern s8 gKolimaForestFloodgate_PushStepZ[];
extern struct SceneEntrance gKolimaForestFloodgateEntrances[];
extern u32 gKolimaForestFloodgateExits[];
extern struct ScenePlacement gKolimaForestFloodgatePlacements[];
extern struct SceneEvent gKolimaForestFloodgateEvents[];

#endif
