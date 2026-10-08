#ifndef ALCHEMY_KORIMA_MURA_H
#define ALCHEMY_KORIMA_MURA_H

#include "TYPES.H"


extern s32 gFallingEffectOffset;
extern s32 gFallingEffectWidth;
extern s32 gFallingEffectState;

extern s32 KorimaMura_PendingPose;
extern s32 KorimaMura_LayoutFlag;
extern s32 KorimaMura_EffectActive;

extern u8 KorimaMura_ActionTable1[];
extern u8 KorimaMura_ActionTable2[];
extern u8 KorimaMura_ActionTable3[];
extern u8 KorimaMura_ActionTable4[];
extern u8 KorimaMura_ActionTable5[];
extern u8 KorimaMura_ActionTable6[];
extern u8 KorimaMura_ActionTable7[];
extern u8 KorimaMura_Object26Script[];

void SceneEffect_SpawnObject26EveryEightFrames(void);
void FieldScene_UpdateFallingEffect(void);

#endif
