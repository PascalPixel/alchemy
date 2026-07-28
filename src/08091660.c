#include "battle_effect_runtime.h"

s32 Func_08009080(s32, s32);

void Func_08091660(void)
{
    struct BattleRenderObject *object;

    object = Func_0808ba1c(Data_02000240.object_id);
    object->unknown_30 = 0x10000;
    object->unknown_34 = 0x8000;
    object->unknown_38 = 0x80000000;
    object->unknown_40 = 0x80000000;
    object->unknown_24 = 0;
    object->unknown_2c = 0;
    if (Data_02000240.mode_1f2 == 1) {
        Func_08009080((s32)object, 0xC);
        return;
    }
    Func_08009080((s32)object, 1);
}
