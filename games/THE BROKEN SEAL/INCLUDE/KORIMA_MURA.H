#ifndef ALCHEMY_KORIMA_MURA_H
#define ALCHEMY_KORIMA_MURA_H

#include "TYPES.H"

/*
 * Kori village (resource_38f): what its files share.
 */

extern u8 *gWork;

/* The falling effect's spread, owned by KORIMA_HASHI/FALLING_EFFECT.C. */
extern s32 gFallingEffectOffset;
extern s32 gFallingEffectWidth;
extern s32 gFallingEffectState;

/* The village's own work, past the overlay image; ACTOR_FX.C owns it. */
extern s32 KorimaMura_PendingPose;
extern s32 KorimaMura_LayoutFlag;
extern s32 KorimaMura_EffectActive;

/* Tables laid out after the code. */
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
