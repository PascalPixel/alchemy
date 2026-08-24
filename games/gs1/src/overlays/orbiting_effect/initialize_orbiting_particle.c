#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_02001af4(void);
extern void Func_02001ae6(OrbitingSceneObject *, s32);
extern s32 Func_02001b0c(s32);
extern u8 *Func_02001aec(s32, s32);
extern void Func_02001b3c(s32);
extern s32 Func_02001b14(u8, s32, u8 *);
extern void Func_02001b12(s32);
extern u8 Value_02008c4d;

#define GetOrbitingSceneObject Func_02001af4
#define NormalizeOrbitingSceneObject Func_02001ae6
#define IsGameFlagSet Func_02001b0c
#define AllocateEffectTransfer Func_02001aec
#define LoadEffectResource Func_02001b3c
#define ConfigurePaletteTransfer Func_02001b14
#define ReleaseEffectTransfer Func_02001b12
#define UpdateOrbitingSceneObject Value_02008c4d
#define InitializeOrbitingParticle Func_02000cb4

void InitializeOrbitingParticle(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
