#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_SATRAGE_MATCH_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_SATRAGE_MATCH_H

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
s32 ColosseumSatrageMatch_Initialize(void);
// The native call precedes this definition; see MATCHING.md.
#ifdef UBFIX
void FieldScene_BuildDescriptorAndInstallTask(s32 leaderId, s32 opponentId,
                                              s32 segments, s32 finishX,
                                              s32 finishZ, s32 firstItem,
                                              s32 secondItem);
#endif
extern const u8 gColosseumSatrageMatch_RollLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosseumSatrageMatch_LetteringDigit[];
extern const s16 gColosseumSatrageMatch_LetteringLose[];
extern char gHexDigits[20];
extern s32 gColosseumSatrageMatch_SparkScript[];
extern s32 gColosseumSatrageMatch_RaiseScript[];
extern u32 gColosseumSatrageMatch_DirectionSteps[];
extern struct SceneEntrance gColosseumSatrageMatchEntrances[];
extern u32 gColosseumSatrageMatchExits[];
extern struct ScenePlacement gColosseumSatrageMatchPlacements[];
extern s32 gColosseumSatrageMatch_LiftPhase;
extern s32 gColosseumSatrageMatch_LiftTimer;
extern struct SceneEvent gColosseumSatrageMatchEvents[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumSatrageMatch_LetteringTitle[];
extern s16 gColosseumSatrageMatch_LetteringGo[];
extern s16 gColosseumSatrageMatch_LetteringWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumSatrageMatch_ApproachScript[];

#endif
