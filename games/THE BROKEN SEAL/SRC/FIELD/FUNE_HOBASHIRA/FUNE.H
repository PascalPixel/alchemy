#ifndef ALCHEMY_FUNE_H
#define ALCHEMY_FUNE_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    /* Message 0x182 + 232. */
    ITEM_ANCHOR_CHARM = 232
};

/* A record's four-byte field at the given byte offset. */
#define RECORD_S32(rec, off) (*(s32 *)((rec) + (off)))

/* The mast's scene tables and the actor tables its scripts hand the engine,
   where the overlay's data lies. */
extern u8 FuneHobashira_SceneTableA[];
extern u8 FuneHobashira_SceneTableB[];
extern u8 FuneHobashira_SceneTableC[];
extern u8 FuneHobashira_SceneTableD[];
extern u8 FuneHobashira_SceneTableE[];
extern s32 FuneHobashira_DriftActions[];
extern s32 FuneHobashira_FlagValues[];

/* The action scripts the mast's actors run and the object lists its scenes
   place, where the overlay's data lies. The lookout's script turns it once
   and then runs the drift actions after it. */
extern u8 FuneHobashira_FollowCameraActions[];
extern u8 FuneHobashira_LookoutActions[];
extern u8 FuneHobashira_WaveActions[];
extern s32 FuneHobashira_StagingObjects[];
extern s32 FuneHobashira_EnsembleObjects[];
extern s32 FuneHobashira_LandingObjects[];

/* The mast's own work past its image (SWAY.C): the sway's angles and drift
   totals, and the camera centre and origin the staging saves. */
extern s32 FuneHobashira_DriftY;
extern s32 FuneHobashira_DriftX;
extern s32 FuneHobashira_SwayY;
extern s32 FuneHobashira_SwayUnused;  /* nothing here reads it */
extern s32 FuneHobashira_CameraCenter[2];
extern s32 FuneHobashira_CameraOrigin[2];
extern s32 FuneHobashira_SwayX;

enum {
    FLAG_MAST_923 = 0x923,
    FLAG_MAST_922 = 0x922,
    FLAG_MAST_924 = 0x924
};

void FieldEffect_UpdateGridPlacement(void);
void FieldScene_RunScene3b0_020004b0(void);
void OverlayObject_Add160ToFields18And1c(u8 *o);
s32 Battle_GetWorkObject1e0();
void Event_CallWithLastActiveObjectId(s32 value);
void Object_SetPosition();
void Object_CommitPosition();
u32 Random16(void);

#endif
