#include "types.h"

s32 Func_080042c8(u32 value);
extern u8 Data_0808f52d;
extern u8 Data_0808f499;

void BattleEffect_EnableTwoCallbacks(void) {
    Func_080042c8((u32)&Data_0808f52d);
    Func_080042c8((u32)&Data_0808f499);
}
