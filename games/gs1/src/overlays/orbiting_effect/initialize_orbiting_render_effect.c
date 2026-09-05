#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_02007664();
extern void Func_0200762e(OrbitingSceneObject *, s32);
extern u8 *Func_02007604(s32, s32);
extern void Func_02007694(s32);
extern s32 Func_0200762c(u8, s32, u8 *);
extern void Func_0200762a(s32);
extern u8 Value_0200b9d5;
#define GetOrbitingSceneObject Func_02007664
#define NormalizeOrbitingSceneObject Func_0200762e
#define IsGameFlagSet Func_02007664
#define AllocateEffectTransfer Func_02007604
#define LoadEffectResource Func_02007694
#define ConfigurePaletteTransfer Func_0200762c
#define ReleaseEffectTransfer Func_0200762a

#define UpdateOrbitingSceneObject Value_0200b9d5

void InitializeOrbitingRenderEffect(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
