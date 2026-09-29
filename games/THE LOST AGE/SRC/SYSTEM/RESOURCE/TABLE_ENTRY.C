#include "TYPES.H"

extern u32 Resource_DirectoryTable[];

u32 Resource_GetTableEntry(u32 index)
{
    return Resource_DirectoryTable[index];
}
