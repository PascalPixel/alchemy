#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

#include "TYPES.H"

#define GetResource Func_08002f40
void *GetResource(s32 resource_id);

/* Index of a free resource table entry. */
s32 Resource_FindFreeEntry(void);

#endif
