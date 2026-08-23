#include "types.h"
#include "resource.h"

extern s32 Data_08320000[];

void *GetResource(s32 index)
{
    return (void *)Data_08320000[index];
}
