#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_2_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_2_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ColosseumFinal2_Initialize(void);
// The native call precedes this definition; see MATCHING.md.
#ifdef UBFIX
void FieldScene_BuildDescriptorAndInstallTask(s32 leaderId, s32 rivalId,
                                              s32 segments, s32 finishX,
                                              s32 finishZ, s32 firstItem,
                                              s32 secondItem);
#endif
extern const u8 gColosseumFinal2_RollLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosseumFinal2_ModeRecordTwo[];
extern const s16 gColosseumFinal2_ModeRecordThreeAlt[];
extern char gHexDigits[20];
extern s32 gColosseumFinal2_SparkScript[];
extern s32 gColosseumFinal2_RaiseScript[];
extern u32 gColosseumFinal2_DirectionSteps[];
extern struct SceneEntrance gColosseumFinal2Entrances[];
extern u32 gColosseumFinal2Exits[];
extern struct ScenePlacement gColosseumFinal2Placements[];
extern s32 gColosseumFinal2_LiftPhase;
extern s32 gColosseumFinal2_LiftTimer;
extern struct SceneEvent gColosseumFinal2Events[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumFinal2_ModeRecordDefault[];
extern s16 gColosseumFinal2_ModeRecordFour[];
extern s16 gColosseumFinal2_ModeRecordThree[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColosseumFinal2_ApproachScript[];

#endif
