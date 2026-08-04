#include "battle_effect_runtime.h"

extern volatile u8 Data_03001f54;
extern volatile s32 Data_03001c94;

void Func_080915ec(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    if (Data_03001f54 != 0) {
        if (Data_03001c94 & 0x200) {
            runtime->unknown_1cc = 0;
        }
        if (Data_03001c94 & 0x100) {
            runtime->unknown_1cc = -1;
        }
    }
}
