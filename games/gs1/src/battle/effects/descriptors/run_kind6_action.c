#include "types.h"
#include "scene.h"
#include "abi/battle/effects/descriptors/run_kind6_action.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct EffectDescriptor *Battle_Run(s32, s32);
void Battle_Reset(void);

s32 BattleFx_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    void *p;

    p = Battle_Run(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = FIELD_AT_OFFSET(p, s32 *, 8);
        if (val != 0) {
            if (val < 0x10000) {
                Battle_Reset();
                Battle_Do(FIELD_AT_OFFSET(p, s32 *, 8));
                Battle_Apply(-1, 0);
                ret = 0;
                Battle_Run2();
            } else {
                ((void (*)(s32))val)(arg0);
                ret = 0;
            }
        }
    }
    return ret;
}
