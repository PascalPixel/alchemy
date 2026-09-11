#include "types.h"
#include "scene.h"
#include "abi/battle/clear_object_flag5b_when_mode3.h"


void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Battle_ClearObjectFlag5bWhenMode3(void)
{
    void *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        Battle_Check();
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 0;
    }
}
