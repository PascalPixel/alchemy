#include "types.h"

struct ResourceMetadataRecord {
  u8 unk00[20];
};

extern struct ResourceMetadataRecord gResource_Data012[];

struct ResourceMetadataRecord *Resource_GetMetadataRecordFar(u32 id) {
  struct ResourceMetadataRecord *table;

  id &= 0xfff;
  if (id >= 716)
    id = 0;
  table = gResource_Data012;
  return &table[id];
}
