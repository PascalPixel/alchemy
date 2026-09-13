#include "types.h"

extern u32 Data_08320000[];

void RuntimeDispatch_NoOpHook(void)
{
}

u32 Resource_GetTableEntry(u32 index)
{
    return Data_08320000[index];
}
