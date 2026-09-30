#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
void UiWork_ClearValueNameTablesFar(void);
extern u8 gCameraWork[];
extern u8 Data_03001ae8[];
s32 BattlePres_ShowMessageWhenField38Positive(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(s16 *);
void BattleMotion_SetupEscapeObject(s32);

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    void **slot = (void **)((u32)&gCameraWork);
    struct BattleCamera *cam = slot[0];
    struct BattlePresentationTransition *trans = slot[32];
    volatile u32 *keys = (volatile u32 *)((u32)&Data_03001ae8);

    if ((*keys & 512) != 0) {
        cam->yaw += 512;
    }
    if ((*keys & 256) != 0) {
        cam->yaw -= 512;
    }
    if (trans->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}
