#include "RESOURCE.H"
#include "TYPES.H"

extern void *Resource_DirectoryTable[];

void *Resource_GetTableEntry(s32 index)
{
    return Resource_DirectoryTable[index];
}
