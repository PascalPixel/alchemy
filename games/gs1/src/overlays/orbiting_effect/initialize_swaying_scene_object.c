#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_02005f6c(void);
extern void Func_02005f4e(OrbitingSceneObject *, s32);
extern s32 Func_02005f84(s32);
extern u8 *Func_02005f3c(s32, s32);
extern void Func_02005fb4(s32);
extern s32 Func_02005f64(u8, s32, u8 *);
extern void Func_02005f62(s32);
extern u8 Value_0200ae71;

#define GetOrbitingSceneObject Func_02005f6c
#define NormalizeOrbitingSceneObject Func_02005f4e
#define IsGameFlagSet Func_02005f84
#define AllocateEffectTransfer Func_02005f3c
#define LoadEffectResource Func_02005fb4
#define ConfigurePaletteTransfer Func_02005f64
#define ReleaseEffectTransfer Func_02005f62
#define UpdateOrbitingSceneObject Value_0200ae71
#define InitializeSwayingSceneObject Func_02002ed8

void InitializeSwayingSceneObject(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
