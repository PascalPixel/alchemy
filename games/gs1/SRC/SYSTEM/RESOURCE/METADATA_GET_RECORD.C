#include "TYPES.H"
#include "SCENE.H"

extern u8 gUnk[];

u8 *Resource_GetMetadataRecord(u32 arg0)
{
    return &gUnk[(arg0 & 0xfff) * 20];
}
