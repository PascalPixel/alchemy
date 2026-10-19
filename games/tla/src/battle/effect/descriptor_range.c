#include "types.h"

struct DescriptorEntry {
  s16 id;
  u8 unk02[2];
  s16 range;
  u8 unk06[2];
};

struct DescriptorEntry *Field_GetPsynergyTargetRule(s32 id);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Returns the absolute value Func_08020310 gives for the object at position
/// when it is within the id's table range (pixels), else -1.
s32 Func_080cda84(s32 id, u8 *object, s32 *position) {
  struct DescriptorEntry *entry = Field_GetPsynergyTargetRule(id);
  s32 height = Func_08020310(position[0], position[2], object[34],
                             *(s32 *)(object + 20));

  if (ABS(height) <= entry->range << 16)
    return ABS(height);
  return -1;
}
