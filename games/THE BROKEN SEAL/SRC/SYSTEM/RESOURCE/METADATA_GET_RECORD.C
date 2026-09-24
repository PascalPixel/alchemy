#include "TYPES.H"
#include "SCENE.H"

extern u8 Character_DescriptorTable[];

u8 *Resource_GetMetadataRecord(u32 arg0)
{
    return &Character_DescriptorTable[(arg0 & 0xfff) * 20];
}
