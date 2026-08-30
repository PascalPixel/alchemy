#include "types.h"

#define FieldScene_PlaySound123AndEnable Func_02002764

extern void Func_02006ce6(s32 sound_id);
extern void Func_02006c74(s32 arg0);

void FieldScene_PlaySound123AndEnable(void)
{
    Func_02006ce6(123);
    Func_02006c74(1);
}
