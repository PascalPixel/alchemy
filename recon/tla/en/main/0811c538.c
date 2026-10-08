#include "types.h"
#include "scene.h"
#include "btlmes.h"
#include "battle_escape.h"
#include "btldisp.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "btlpty.h"
#include "system.h"
void UiWork_ClearValueNameTablesFar(void);
extern u8 Data_03001ae8[];
s32 BattlePres_ShowDefendMessage(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    void **slot = (void **)&gCameraWork;
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
