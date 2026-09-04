#include "types.h"

#define Resource_GetMetadataRecord Func_08185008

extern u8 Data_08185024[];

u8 *Resource_GetMetadataRecord(u32 arg0) {
    return &Data_08185024[(arg0 & 0xfff) * 20];
}
