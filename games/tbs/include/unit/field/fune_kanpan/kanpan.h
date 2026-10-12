#ifndef ALCHEMY_UNIT_FIELD_FUNE_KANPAN_KANPAN_H
#define ALCHEMY_UNIT_FIELD_FUNE_KANPAN_KANPAN_H

#include "types.h"
#include "field_scene.h"

u8 *FuneKanpan_GetEntrances(void);
u8 *FuneKanpan_GetRegions(void);
u8 *FuneKanpan_GetExits(void);
u8 *FuneKanpan_GetPlacements(void);
u8 *FuneKanpan_GetEvents(void);
s32 FuneKanpan_ResetDeck(void);
extern s32 FuneKanpan_SlotDepth[8];
extern u32 FuneKanpan_SlotMode[8];
extern const s32 FuneKanpan_PresentationActionsA[];
extern const s32 FuneKanpan_PresentationActionsB[];
extern const s32 FuneKanpan_RandomActorActions[];
extern const s32 FuneKanpan_DeckEventActions[];
extern const s32 FuneKanpan_LeadActionsA[];
extern const s32 FuneKanpan_LeadActionsB[];
extern const s32 FuneKanpan_CrewActionsA[];
extern const s32 FuneKanpan_CrewActionsB[];
extern const s32 FuneKanpan_CrewActionsC[];
extern const s32 FuneKanpan_CrewActionsD[];
extern const s32 FuneKanpan_CrewActionsE[];
extern const s32 FuneKanpan_SailorActions[];
extern const s32 FuneKanpan_DeckActionsB[];
extern const s32 FuneKanpan_DeckActionsA[];
extern const s32 FuneKanpan_DeckActionsC[];
extern const s32 FuneKanpan_RosterActions[];
extern const s32 FuneKanpan_LeaveActions[];
extern const s32 FuneKanpan_LeadActionsC[];
extern const struct SceneEntrance FuneKanpan_SceneTableA[];
extern const s16 FuneKanpan_SceneTableB[];
extern const u32 FuneKanpan_SceneTableC[];
extern const struct ScenePlacement gFuneKanpanPlacements[];
extern const struct ScenePlacement gFuneKanpanPlacementsFlag911[];
extern const struct ScenePlacement gFuneKanpanPlacementsFlag927[];
extern const struct ScenePlacement gFuneKanpanPlacementsFlag928[];
extern const struct ScenePlacement FuneKanpan_CrewScript[];
extern const struct ScenePlacement FuneKanpan_ClosingCrewScript[];
extern const struct ScenePlacement FuneKanpan_CrewScriptB[];
extern const struct ScenePlacement FuneKanpan_CrewScriptC[];
extern const struct ScenePlacement FuneKanpan_CrewScriptD[];
extern const struct ScenePlacement FuneKanpan_CrewScriptE[];
extern const struct ScenePlacement gFuneKanpanPlacementsFlag93e[];
extern const struct SceneEvent gFuneKanpanEvents[];
extern const struct SceneEvent gFuneKanpanEventsFlag928[];
extern const struct SceneEvent gFuneKanpanEventsFlag93e[];
extern const struct SceneEvent gFuneKanpanEventsFlag8a0[];
extern const s32 FuneKanpan_FlagGroupEntries[];

#endif
