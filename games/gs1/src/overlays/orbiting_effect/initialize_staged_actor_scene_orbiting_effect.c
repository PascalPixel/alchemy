#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_0200bb38();
extern void Func_0200bad2(OrbitingSceneObject *, s32);
extern u8 *Func_0200ba98(s32, s32);
extern void Func_0200bb60(s32);
extern s32 Func_0200bac8(u8, s32, u8 *);
extern void Func_0200babe(s32);
extern u8 Value_0200dae1;

#define GetOrbitingSceneObject Func_0200bb38
#define NormalizeOrbitingSceneObject Func_0200bad2
#define IsGameFlagSet Func_0200bb38
#define AllocateEffectTransfer Func_0200ba98
#define LoadEffectResource Func_0200bb60
#define ConfigurePaletteTransfer Func_0200bac8
#define ReleaseEffectTransfer Func_0200babe
#define UpdateOrbitingSceneObject Value_0200dae1
#define InitializeStagedActorSceneOrbitingEffect Func_02005b48

void InitializeStagedActorSceneOrbitingEffect(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
