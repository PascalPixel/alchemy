#ifndef ALCHEMY_UNIT_FIELD_KORIMA_KI_KI_H
#define ALCHEMY_UNIT_FIELD_KORIMA_KI_KI_H

#include "types.h"
#include "field_scene.h"

struct OrbitingPaletteEffect;
struct PaletteEffectFrame;

u8 *KorimaKi_GetEntrances(void);
u8 *KorimaKi_GetRegions(void);
u8 *KorimaKi_GetExits(void);
u8 *KorimaKi_GetPlacements(void);
void PaletteScene_Initialize(void);
u8 *KorimaKi_GetEvents(void);
void KorimaKi_RunMessageScene(void);
void KorimaKi_TalkToToreto(void);
void PaletteScene_RunActorNineBranch(void);
void PaletteScene_RunActorEightBranch(void);
void PaletteScene_RunFlaggedBranch(void);
void RunEventScript01(void);
void PaletteScene_RunActorTransitionSequence(void);
s32 KorimaKi_PrepareActors(void);
s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame);
void PaletteScene_SpawnEffect(void);
void PaletteScene_AdvanceTransition(void);
void KorimaKi_PlayGesture(s32 actor, s32 gesture);
void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect);
void KorimaKi_SpawnOrbitSparks(void);
void PaletteScene_SetRecordValue(s32 key, s32 value);
void PaletteScene_AdjustPaletteWindow(s32 adjustment);
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment);
extern s32 SceneAction_ActorOneEntry[];
extern s32 SceneAction_ActorTwoEntry[];
extern s32 SceneAction_ActorThreeEntry[];
extern s32 SceneAction_GroupFinish[];
extern struct SceneEntrance gKorimaKiEntrances[];
extern s16 gKorimaKiRegions[];
extern u32 gKorimaKiExits[];
extern struct ScenePlacement gKorimaKiPlacements[];
extern struct SceneEvent gKorimaKiEvents[];
extern s32 gKorimaKiEffectScript[];

#endif
