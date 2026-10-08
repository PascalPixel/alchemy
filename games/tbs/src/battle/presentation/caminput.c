#include "types.h"
#include "io_reg.h"
#include "heap_state.h"
#include "btlpty.h"
#include "scene.h"
#include "global_cells.h"
#include "btlwork.h"
#include "btlsys.h"
#include "btldisp.h"

extern u8 gKeysHeld[];

void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
    struct BattleCamera *cam = *(gWorkSlot + HEAP_SLOT_CAMERA);
    struct BattlePresentationTransition *trans = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    volatile u32 *keys = (volatile u32 *)gKeysHeld;

    if ((*keys & KEY_L) != 0) {
        cam->yaw += 512;
    }
    if ((*keys & KEY_R) != 0) {
        cam->yaw -= 512;
    }
    if (trans->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

void Battle_ReservedNoOp9B2C(void)
{
}
