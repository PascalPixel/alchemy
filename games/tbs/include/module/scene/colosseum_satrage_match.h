#ifndef ALCHEMY_MODULE_MAP_COLOSSEUM_SATRAGE_MATCH_H
#define ALCHEMY_MODULE_MAP_COLOSSEUM_SATRAGE_MATCH_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 ColosseumSatrageMatch_Initialize(void);
// The native call precedes this definition; see MATCHING.md.
#ifdef UBFIX
void Scene_BuildDescriptorAndInstallTask(s32 leaderId, s32 opponentId,
                                         s32 segments, s32 finishX, s32 finishZ,
                                         s32 firstItem, s32 secondItem);
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
extern struct ActorPlacement gColosseumSatrageMatchActorPlacements[];
extern s32 gColosseumSatrageMatch_LiftPhase;
extern s32 gColosseumSatrageMatch_LiftTimer;
extern struct Trigger gColosseumSatrageMatchTriggers[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumSatrageMatch_LetteringTitle[];
extern s16 gColosseumSatrageMatch_LetteringGo[];
extern s16 gColosseumSatrageMatch_LetteringWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumSatrageMatch_ApproachScript[];

#endif
