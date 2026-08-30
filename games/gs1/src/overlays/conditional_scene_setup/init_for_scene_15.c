#include "types.h"

extern s16 Data_02000240[];

#define ConditionalSceneSetup_InitForScene15 Func_02000054

s32 Func_020000dc(void);

s32 ConditionalSceneSetup_InitForScene15(void)
{
    if (Data_02000240[225] == 15) {
        Func_020000dc();
    }
    return 0;
}
