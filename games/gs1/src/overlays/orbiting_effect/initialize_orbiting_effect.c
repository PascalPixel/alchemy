#include "orbiting_scene_object.h"

extern OrbitingSceneObject *Func_020059d4(void);
extern void Func_020059a6(OrbitingSceneObject *, s32);
extern s32 Func_020059ec(s32);
extern u8 *Func_0200599c(s32, s32);
extern void Func_02005a1c(s32);
extern s32 Func_020059c4(u8, s32, u8 *);
extern void Func_020059c2(s32);
extern u8 Value_0200aba5;

#define GetOrbitingSceneObject Func_020059d4
#define NormalizeOrbitingSceneObject Func_020059a6
#define IsGameFlagSet Func_020059ec
#define AllocateEffectTransfer Func_0200599c
#define LoadEffectResource Func_02005a1c
#define ConfigurePaletteTransfer Func_020059c4
#define ReleaseEffectTransfer Func_020059c2
#define UpdateOrbitingSceneObject Value_0200aba5
#define InitializeOrbitingEffect Func_02002c0c

void InitializeOrbitingEffect(void)
{
#include "initialize_orbiting_scene_object_body.inc"
}
