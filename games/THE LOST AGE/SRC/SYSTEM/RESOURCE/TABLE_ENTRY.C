#include "TYPES.H"

extern u32 Data_08680000[];

u32 Resource_GetTableEntry(u32 index)
{
    return Data_08680000[index];
}
