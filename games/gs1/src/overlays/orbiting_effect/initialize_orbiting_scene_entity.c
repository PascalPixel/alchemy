#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_02001474(void);
extern void Func_0200144e(OrbitingSceneObject *, s32);
extern s32 Func_0200147c(s32);
extern u8 *Func_0200146c(s32, s32);
extern void Func_020014ac(s32);
extern s32 Func_02001494(u8, s32, u8 *);
extern void Func_02001492(s32);
extern u8 Value_0200890d;
#define GetOrbitingSceneObject Func_02001474
#define NormalizeOrbitingSceneObject Func_0200144e
#define IsGameFlagSet Func_0200147c
#define AllocateEffectTransfer Func_0200146c
#define LoadEffectResource Func_020014ac
#define ConfigurePaletteTransfer Func_02001494
#define ReleaseEffectTransfer Func_02001492

#define UpdateOrbitingSceneObject Value_0200890d

void InitializeOrbitingSceneEntity(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
