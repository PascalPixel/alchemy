#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_msg.h"
#include "battle_escape.h"
#include "battle_presentation.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"

/* battle/presentation/cam/shoulder.c */
extern u16 Value_fffffe00;

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    void **slot = (void **)ADDR_03001E80;
    u8 *cam = slot[0];
    u8 *trans = slot[32];
    volatile u32 *keys = (volatile u32 *)ADDR_03001AE8;

    if ((*keys & 512) != 0) {
        *(u16 *)(cam + 54) += 512;
    }
    if ((*keys & 256) != 0) {
        *(u16 *)(cam + 54) += (u16)(u32)&Value_fffffe00;
    }
    if (*(u32 *)(trans + 20) == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}
