#ifndef ALCHEMY_UNIT_MAP_COLOSSEUM_SATRAGE_MATCH_H
#define ALCHEMY_UNIT_MAP_COLOSSEUM_SATRAGE_MATCH_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
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
extern struct MapWarpDest gColosseumSatrageMatchWarpDests[];
extern u32 gColosseumSatrageMatchWarps[];
extern struct ObjectEventTemplate gColosseumSatrageMatchObjectEventTemplates[];
extern s32 gColosseumSatrageMatch_LiftPhase;
extern s32 gColosseumSatrageMatch_LiftTimer;
extern struct MapEvent gColosseumSatrageMatchEvents[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumSatrageMatch_LetteringTitle[];
extern s16 gColosseumSatrageMatch_LetteringGo[];
extern s16 gColosseumSatrageMatch_LetteringWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumSatrageMatch_ApproachScript[];

#endif
