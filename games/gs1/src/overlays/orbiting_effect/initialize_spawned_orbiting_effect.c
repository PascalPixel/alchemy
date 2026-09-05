#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_0200324c(void);
extern void Func_02003216(OrbitingSceneObject *, s32);
extern s32 Func_02003254(s32);
extern u8 *Func_02003214(s32, s32);
extern void Func_02003284(s32);
extern s32 Func_0200323c(u8, s32, u8 *);
extern void Func_0200323a(s32);
extern u8 Value_020097d9;
#define GetOrbitingSceneObject Func_0200324c
#define NormalizeOrbitingSceneObject Func_02003216
#define IsGameFlagSet Func_02003254
#define AllocateEffectTransfer Func_02003214
#define LoadEffectResource Func_02003284
#define ConfigurePaletteTransfer Func_0200323c
#define ReleaseEffectTransfer Func_0200323a

#define UpdateOrbitingSceneObject Value_020097d9

void InitializeSpawnedOrbitingEffect(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
