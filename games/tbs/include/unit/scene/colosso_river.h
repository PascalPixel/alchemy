#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H

#include "types.h"
#include "field_scene.h"

typedef struct Position3 {
  s32 x;
  s32 y;
  s32 z;
} Position3;

struct SceneEntrance *ColossoRiver_GetEntrances(void);
const struct SceneRegion *ColossoRiver_GetRegions(void);
u32 *ColossoRiver_GetExits(void);
struct ScenePlacement *ColossoRiver_GetPlacements(void);
struct SceneEvent *ColossoRiver_GetEvents(void);
s32 ColossoRiver_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoRiver_ModeRecordTwo[];
extern const s16 gColossoRiver_SpanB[];
extern char gHexDigits[20];
extern s32 gColossoRiver_ScriptA[];
extern s32 gColossoRiver_ScriptB[];
extern u32 gColossoRiver_DirectionSteps[];
extern struct SceneEntrance gColossoRiver_Entrances[];
extern u32 gColossoRiver_Exits[];
extern struct ScenePlacement gColossoRiver_Placements[];
extern s32 gColossoRiver_Countdown;
extern struct SceneEvent gColossoRiver_Events[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoRiver_RoundSpans[];
extern s16 gColossoRiver_ModeRecordFour[];
extern s16 gColossoRiver_SpanA[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColossoRiver_ApproachScript[];

#endif
