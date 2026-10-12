/* Near miss: differ 4 of 41 (two reordered rows). The native loads the key register address from the pool before it finishes the heap base (movs; ldr r0; lsls) and reads the keys before it builds 0x200. A one-pass block on the key read forces sub sp ahead and swaps r0 and r4 (differ 12): the block holds the prologue. Declaring the keys first gives them r4 (differ 10). */
#include "types.h"
#include "scene.h"
#include "btlmes.h"
#include "battle_escape.h"
#include "btldisp.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "btlpty.h"
#include "system.h"
#include "ram_buffer.h"
void UiWork_ClearValueNameTablesFar(void);
extern u8 Data_03001ae8[];
s32 BattlePres_ShowDefendMessage(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

void BattlePres_AdjustCameraByShoulderKeys(void)
{
    struct BattleCamera *cam = Ram_HeapSlots->camera_work;
    struct BattlePresentationTransition *trans = Ram_HeapSlots->transition_work;
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
