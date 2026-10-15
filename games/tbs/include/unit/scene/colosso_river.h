#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H

#include "types.h"
#include "field_scene.h"

typedef struct Position3
{
    s32 x;
    s32 y;
    s32 z;
} Position3;

u8                          *ColossoRiver_GetEntrances(void);
s32                          get_default_result(void);
u8                          *ColossoRiver_GetExits(void);
u8                          *ColossoRiver_GetPlacements(void);
u8                          *ColossoRiver_GetEvents(void);
s32                          ColossoRiver_ApplyEntryState(void);
extern const u8              gColosso_PortraitPaletteOffsets[];
extern const s16             gColossoRiver_ModeRecordTwo[];
extern const s16             gColossoRiver_SpanB[];
extern char                  gHexDigits[20];
extern s32                   gColossoRiver_ScriptA[];
extern s32                   gColossoRiver_ScriptB[];
extern u32                   gColossoRiver_DirectionSteps[];
extern struct SceneEntrance  gColossoRiver_SceneTableA[];
extern u32                   gColossoRiver_SceneTableB[];
extern struct ScenePlacement gColossoRiver_SceneTableC[];
extern s32                   gColossoRiver_Countdown;
extern struct SceneEvent     gColossoRiver_SceneTableD[];
extern s16                   gColosso_PortraitSlot;
extern s16                   gColossoRiver_RoundSpans[];
extern s16                   gColossoRiver_ModeRecordFour[];
extern s16                   gColossoRiver_SpanA[];
extern s16                   gColosso_MarkerSlot;
extern s32                   gColosso_RivalFinishScript[];
extern s32                   gColossoRiver_ApproachScript[];

#endif
