/* Declarations shared by the Venus lighthouse summit scene's sources (resource_3c9). */
#ifndef CHOJO_H
#define CHOJO_H

#include "TYPES.H"
#include "VINASU.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SOUND_IDS.H"

#define NULL ((void *)0)

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj_02000400;

typedef struct {
    u8 filler0[0x50];
    Obj_02000400 *obj;
} Spr_02000400;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr_02000424;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj_020004bc;

typedef struct {
    u8 filler0[0x50];
    Obj_020004bc *obj;
} Spr_020004bc;

struct SceneActor {
    u8 pad00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[20];
    s32 motion28;
    u8 pad2c[4];
    s32 motion30;
    s32 motion34;
    u8 pad38[29];
    u8 flags55;
};

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[0x3d];
    u8 unk55;
} Spr_020005ec;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr_0200071c;

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern unsigned char Data_0200e3d4[];
extern unsigned char Data_0200e464[];
extern unsigned char Data_0200e478[];
extern unsigned char Data_0200e6ec[];
extern Spr *Data_0200e6e8;

s32 Math_RemainderUnsigned(s32, s32);
void OverlayObject_SetValue1(void *, s32);
void SceneActor_ParkRecord(u8 *record);
s32 Object_CheckMovementCollision();
void Vector_AddPolarOffset();
void SceneActor_SetByte55ForActorZeroAnd12To17();
s32 SceneActor_FindNearestSlotOfKindF2(void);
void SceneState_ForwardByRuntimeWordBits(s32 a);
void Scene_RunScriptedActorPresentation(void);
void SceneEffect_SpawnParticlesBesideActor();
void Scene_RunPairedActorEffectSequence(void);
void Inventory_AddItem(s32, s32);
void FieldScene_RunScene3c9_02003924(void);
void FieldScene_RunBracketedSceneWithFlag282(void);
void VinasuChojo_RunActorTransition();
void FieldScene_RunMultiActorPresentation();
void BattleEffect_CleanupSceneObjects(s32);
void VinasuChojo_FaceActor();
void VinasuChojo_ShowMessage();

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position);

void FieldScene_RunScene3c9_02004b28();

#endif
