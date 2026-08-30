#include "types.h"

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

extern void Func_02005722(s32 frames);
extern void Func_0200571c(void);
extern void Func_02005784(void);
extern struct SceneActor_02001334 *Func_0200573e(s32 slot);
extern void Func_0200579c(s32 slot, s32 mode);
extern void Func_020057ea(s32 slot, s32 mode);
extern void Func_02001f68(void);
extern void Func_0200213c(void);
extern void Func_02002062(void);
extern void Func_020021f6(void);
extern void Func_020023c8(s32 variant);

#define AdvanceSceneStep       Func_02005722
#define OpenSceneSection        Func_0200571c
#define CloseSceneSection       Func_02005784
#define GetSceneRecord          Func_0200573e
#define SetSceneActorMode       Func_0200579c
#define SetSceneActorModeAfterBranch Func_020057ea
#define RunFacingVariantA       Func_020021f6
#define RunFacingVariantB       Func_02001f68
#define RunFacingVariantC       Func_0200213c
#define RunFacingVariantD       Func_02002062
#define RunSceneVariant         Func_020023c8
#define FieldScene_DispatchByFacing Func_02001334

void FieldScene_DispatchByFacing(void)
{
    struct SceneActor_02001334 *record = GetSceneRecord(0);
    u16 angle;

    OpenSceneSection();
    SetSceneActorMode(0, 8);
    AdvanceSceneStep(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    SetSceneActorModeAfterBranch(0, 1);
    RunSceneVariant(1);
    CloseSceneSection();
}
