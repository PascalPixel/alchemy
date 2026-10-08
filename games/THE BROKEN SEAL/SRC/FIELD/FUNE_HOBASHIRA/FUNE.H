#ifndef ALCHEMY_FUNE_H
#define ALCHEMY_FUNE_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    ITEM_ANCHOR_CHARM = 232
};

extern u8 FuneHobashira_SceneTableA[];
extern u8 FuneHobashira_SceneTableB[];
extern u8 FuneHobashira_SceneTableC[];
extern u8 FuneHobashira_SceneTableD[];
extern u8 FuneHobashira_SceneTableE[];
extern s32 FuneHobashira_DriftActions[];
extern s32 FuneHobashira_FlagValues[];

extern u8 FuneHobashira_FollowCameraActions[];
extern u8 FuneHobashira_LookoutActions[];
extern u8 FuneHobashira_WaveActions[];
extern s32 FuneHobashira_StagingObjects[];
extern s32 FuneHobashira_EnsembleObjects[];
extern s32 FuneHobashira_LandingObjects[];

extern s32 FuneHobashira_DriftY;
extern s32 FuneHobashira_DriftX;
extern s32 FuneHobashira_SwayY;
extern s32 FuneHobashira_SwayUnused;
extern s32 FuneHobashira_CameraCenter[2];
extern s32 FuneHobashira_CameraOrigin[2];
extern s32 FuneHobashira_SwayX;

enum {
    FLAG_MAST_923 = 0x923,
    FLAG_MAST_922 = 0x922,
    FLAG_MAST_924 = 0x924
};

void FieldEffect_UpdateGridPlacement(void);
void FuneHobashira_AnnounceClearCourse(void);
void FuneHobashira_GrowActorScale(struct FieldActor *o);
s32 Battle_GetWorkObject1e0();
void Event_CallWithLastActiveObjectId(s32 value);
void Object_CommitPosition();

#endif
