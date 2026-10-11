#ifndef ALCHEMY_FUNE_H
#define ALCHEMY_FUNE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum
{
	ITEM_ANCHOR_CHARM = 232
};

extern struct SceneEntrance	 FuneHobashira_SceneTableA[];
extern s16					 FuneHobashira_SceneTableB[];
extern u32					 FuneHobashira_SceneTableC[];
extern struct ScenePlacement FuneHobashira_SceneTableD[];
extern struct SceneEvent	 FuneHobashira_SceneTableE[];
extern s32					 FuneHobashira_DriftActions[];
extern u32					 FuneHobashira_FlagValues[];

extern s32					 FuneHobashira_FollowCameraActions[];
extern s32					 FuneHobashira_LookoutActions[];
extern s32					 FuneHobashira_WaveActions[];
extern struct ScenePlacement FuneHobashira_StagingObjects[];
extern struct ScenePlacement FuneHobashira_EnsembleObjects[];
extern struct ScenePlacement FuneHobashira_LandingObjects[];

extern s32 FuneHobashira_DriftY;
extern s32 FuneHobashira_DriftX;
extern s32 FuneHobashira_SwayY;
extern s32 FuneHobashira_SwayUnused;
extern s32 FuneHobashira_CameraCenter[2];
extern s32 FuneHobashira_CameraOrigin[2];
extern s32 FuneHobashira_SwayX;

enum
{
	FLAG_MAST_923 = 0x923,
	FLAG_MAST_922 = 0x922,
	FLAG_MAST_924 = 0x924
};

void FieldEffect_UpdateGridPlacement(void);
void FuneHobashira_AnnounceClearCourse(void);
void FuneHobashira_GrowActorScale(struct FieldActor *o);
s32	 Battle_GetWorkObject1e0();
void Event_CallWithLastActiveObjectId(s32 value);
void Object_CommitPosition();

#endif
