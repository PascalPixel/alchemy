#include "scene.h"
#include "abi/battle/runtime/update_mode_from_shoulder_buttons.h"
#include "battle_effect_runtime.h"

extern volatile u8 gIw;
extern volatile s32 gIw2;

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = gWork;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (gIw != 0) {
        if (gIw2 & 0x200) {
            runtime->mode_1cc = 0;
        }
        if (gIw2 & 0x100) {
            runtime->mode_1cc = -1;
        }
    }
}
