#include "types.h"

struct BattleFxDefinition {
    u16 id;
    u8 value;
    u8 flags;
};

struct BattleFxDefinition *Func_080d1e60(s32 id);

s32 BattleFx_GetAnimationValue(void);

u8 BattleFx_GetFlags(void)
{
    return Func_080d1e60(BattleFx_GetAnimationValue())->flags;
}
