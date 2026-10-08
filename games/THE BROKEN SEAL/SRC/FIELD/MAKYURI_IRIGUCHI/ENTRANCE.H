#include "MEM.H"
#ifndef ALCHEMY_ENTRANCE_H
#define ALCHEMY_ENTRANCE_H

#include "TYPES.H"
#include "MAKYURI.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "STAGED_ACTOR.H"

struct Src_39c {
    u8 pad00[8];
    s32 f8;
    u8 pad0c[4];
    s32 f16;
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;
    s32 f12;
    s32 f16;
    u8 pad14[4];
    s32 f24;
    s32 f28;
    u8 pad20[68];
    s16 f100;
    u8 pad66[2];
    struct Src_39c *f104;
};

struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};


extern const u8 MakyuriIriguchi_Actor8Path1[];
extern const u8 MakyuriIriguchi_Actor8Path2[];
extern const u8 MakyuriIriguchi_Actor8Path3[];
extern const s32 MakyuriIriguchi_RiseScript[];
extern const s32 MakyuriIriguchi_SparkBurstScript[];
extern const s32 Makyuri_ScaleCounterScript[];
extern u8 MakyuriIriguchi_SceneTableA[];
extern u8 MakyuriIriguchi_SceneTableB[];
extern u8 MakyuriIriguchi_SceneTableC[];
extern u8 MakyuriIriguchi_SceneTableD[];
void Makyuri_CyclePalette(void);
void Effect_Spawn();

void AnimationObjects_SelectAnimation();
void BattleFx_PlayQueuedSound();
s32 ArcTan2(s32 deltaZ, s32 deltaX);
void Makyuri_TickSpawnTimer(void);
void Makyuri_RunActorMove(void);
void SceneState_ClearCurrentRecordAndReleaseTarget(void);
void MakyuriIriguchi_RaisePillar(void);
void SceneActor_RunActorZeroHandledMotion(int page);

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0);
s32 SceneAudio_PlayCue118AndReturnZero(void);
s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object);
void MakyuriIriguchi_TickSpawnTimer(void);
void MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget(void);
void FieldScene_RunSingleStep(void);
void MakyuriIriguchi_TriggerPillarRise(void);
void FieldScene_RunStepWithValue1632(void);
void MakyuriIriguchi_DropLeaderToColumn(s32 a0);
void FieldScene_RunIndexedStep17(void);
void FieldScene_RunIndexedStep18(void);
void FieldScene_RunIndexedStep19(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void FieldScene_RunIndexedStep63(void);
void MakyuriIriguchi_InstallActorEightPath(void);
void MakyuriIriguchi_SendActor8ByLeaderColumn(void);
void MakyuriIriguchi_ArriveWithSparks(void);
void MakyuriIriguchi_RunDoorScene(void);
void OverlayObject_SpawnKind24AtObject(struct FieldActor *src);
s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work);
s32 OverlayObject_RampWords24And28Over16Frames(struct FieldActor *o);
s32 OverlayObject_AdvanceScaleCounter(struct FieldActor *o);
void OverlayObject_ReleasePublishedAttachment(void);
void MakyuriIriguchi_CrossDoorway(void);

#endif
