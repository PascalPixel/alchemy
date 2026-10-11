#include "edition.h"
#ifndef TOREBI_MACHI_MACHI_H
#define TOREBI_MACHI_MACHI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
extern u8 gWindowWork[];

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#define SCENE_OBJECT_ID (*(s32 *)((u8 *)gEventWork + 0x1f4))

struct TorebiSceneWork {
#if EDITION_INTERNATIONAL
    u8 unknown_00[0xea4];
#else
    u8 unknown_00[0xf34];
#endif
    u8 widen_proximity;
};

struct SceneSetup {
    u16 *commands;
    u16 first;
    u16 second;
};

struct SceneSprite {
    u8 unknown_00[0x15];
    u8 unknown_15_low : 2;
    u8 priority_15 : 2;
    u8 unknown_15_high : 4;
};

struct Actor {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};
extern u8 gTorebiMachiActor16Action[];
extern u8 gTorebiMachiActor17Action[];
extern u8 gTorebiMachiEntrances[];
extern u8 gTorebiMachiExits[];
extern u8 gTorebiMachiPlacements[];
extern u8 gTorebiMachiEvents3[];
extern u8 gTorebiMachiEvents2[];
extern u8 gTorebiMachiEvents[];
extern struct SceneSetup gTorebiMachiCellSteps[];

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

s32 SceneActor_UpdatePlayerProximity();
void SetMapCellCollision();
void TorebiMachi_OpenActorEightPassage();
void FieldScene_ResetActor9AndDrawTiles();
s32 UiWindow_CreateWithSideObject();
void UiText_OpenMessageWindow();

void SceneState_SetValues31_2_4(void);

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b);

#include "types.h"

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
    u8 unk_5c[8];
    u16 proximity_flags;
};

s32 SceneActor_GetPositionDistance(s32 *, s32 *);
s32 ArcTan2(s32, s32);

s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor, struct SceneActor *target, s32 range, s32 force);

s32 SceneActor_UpdatePartnerProximity(u8 *self);

u8 *TorebiMachi_GetEntrances(void);

s32 SceneData_ReturnZero(void);

u8 *TorebiMachi_GetExits(void);

u8 *TorebiMachi_GetPlacements(void);

void TorebiMachi_OpenActorEightPassage(void);

void ConfigureAndPlaceActorOneHundredTwo(void);

void HideActorOneHundredTwo(void);

void SceneDialogue_RunMessage0e36(void);

void SceneDialogue_RunMessage0e37(void);

void FieldScene_RunSupplementalSequenceTwo(void);

void FieldScene_RunSiblingsTalk(void);

u8 *TorebiMachi_SelectEvents(void);

void SceneDialogue_RunActor15Message1f92(void);

void SceneDialogue_RunActor24Message1f9d(void);

void FieldScene_RunPatientTalk(void);

void TorebiMachi_AskForSweets(void);

void SceneDialogue_RunActor27Message1fa3(void);

void SceneDialogue_RunActor24Message235f(void);

void TorebiMachi_ReceiveLeftovers(void);

void SceneScript_SetupActors(void);

void SceneActor_CopyPlayerModeToActor(union FieldObject *object);

s32 TorebiMachi_ApplyEntryState(s32 a0);

void FieldScene_RunScene3b5SequenceA(void);

void SceneState_SetValue30ThenCall(void);

void TorebiMachi_RequestTouchedExit(void);

void FieldScene_RunPrimarySequence(void);

void FieldScene_ResetActor9AndDrawTiles(void);

#endif
