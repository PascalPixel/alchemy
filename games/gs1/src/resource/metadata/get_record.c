#include "types.h"
#include "scene.h"

extern u8 gUnk[];

u8 *Resource_GetMetadataRecord(u32 arg0)
{
    return &gUnk[(arg0 & 0xfff) * 20];
}
