#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct EffectDescriptor *Func_0808d48c(s32, s32);
void BattleRuntime_Reset(void);
void Func_08091750(void);
void Func_08092b94(s32);
void Func_08092f84(s32, s32);

s32 BattleEffect_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    void *p;

    p = Func_0808d48c(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = FIELD_AT_OFFSET(p, s32 *, 8);
        if (val != 0) {
            if (val < 0x10000) {
                BattleRuntime_Reset();
                Func_08092b94(FIELD_AT_OFFSET(p, s32 *, 8));
                Func_08092f84(-1, 0);
                ret = 0;
                Func_08091750();
            } else {
                ((void (*)(s32))val)(arg0);
                ret = 0;
            }
        }
    }
    return ret;
}
