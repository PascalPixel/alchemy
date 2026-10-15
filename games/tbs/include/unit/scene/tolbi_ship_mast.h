#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H

#include "types.h"
#include "field_scene.h"

u8 *FuneHobashira_GetEntrances(void);
u8 *FuneHobashira_GetRegions(void);
u8 *FuneHobashira_GetExits(void);
u8 *FuneHobashira_GetPlacements(void);
u8 *FuneHobashira_GetEvents(void);
s32 FuneHobashira_ApplyEntryState(void);
extern s32 FuneHobashira_FollowCameraActions[];
extern s32 FuneHobashira_LookoutActions[];
extern s32 FuneHobashira_DriftActions[];
extern s32 FuneHobashira_WaveActions[];
extern s32 FuneHobashira_EnsembleWalk9[];
extern s32 FuneHobashira_EnsembleWalk10[];
extern s32 FuneHobashira_EnsembleWalk11[];
extern s32 FuneHobashira_EnsembleWalk12[];
extern s32 FuneHobashira_EnsembleWalk13[];
extern s32 FuneHobashira_EnsembleWalk14[];
extern u32 FuneHobashira_EnsembleWalk15[];
extern struct SceneEntrance FuneHobashira_SceneTableA[];
extern s16 FuneHobashira_SceneTableB[];
extern u32 FuneHobashira_SceneTableC[];
extern struct ScenePlacement FuneHobashira_SceneTableD[];
extern struct ScenePlacement FuneHobashira_StagingObjects[];
extern struct ScenePlacement FuneHobashira_EnsembleObjects[];
extern struct ScenePlacement FuneHobashira_LandingObjects[];
extern struct SceneEvent FuneHobashira_SceneTableE[];
extern u32 FuneHobashira_FlagValues[];

#endif
