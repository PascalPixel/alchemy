/* Draft, differ 4 of 21: the native stores the entry (strh) before it starts the size (movs r1, #128); here the size comes first. Tried: a block pointer, a struct with the entry first, a size temporary, an inline VramLoad as overlay 69b uses; permute 1 minute found nothing. */
#include "../../../../games/tla/src/field/overlay_679/local.h"

void Func_020017bc(void)
{
    Iwram_ClearWords(Data_02002aa0, 0x1004);
    Data_02002aa0[0] = Resource_FindFreeEntry();
    VramBlock_LoadCached(Data_02002aa0[0], 0x200, 0);
    Scheduler_AddOrUpdateCallback(Func_020015f8, 0x480);
}
