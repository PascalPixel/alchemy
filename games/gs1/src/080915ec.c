#include "battle_effect_runtime.h"

extern volatile u8 Data_03001f54;
extern volatile s32 Data_03001c94;

void Func_080915ec(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (Data_03001f54 != 0) {
        if (Data_03001c94 & 0x200) {
            runtime->mode_1cc = 0;
        }
        if (Data_03001c94 & 0x100) {
            runtime->mode_1cc = -1;
        }
    }
}
