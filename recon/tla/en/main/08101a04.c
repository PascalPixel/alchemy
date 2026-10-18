// First draft of Func_08101a04: the state of a packed djinni entry (owner in
// bits 8-11, element in 5-7, index in 0-4, bit 15 set): 2 when the djinni is
// neither ready nor active, otherwise whether bit 15 is set. Its fields are
// ☀️'s DJINN_OWNER and DJINN_ELEMENT (games/tbs/src/menu/menu.c). Differ 11
// of 37 (draft 34), in the last test only: the native computes bit 15 as a
// general x != 0 (mask 0x8000, negs, orrs, lsrs #31), where this draft's
// single-bit test becomes a shift and a mask. Measured: s32 or u16 entry,
// the mask -0x8000, !!, > 0 and a cast to u16 give 10 to 17.
#include "types.h"

s32 Djinn_IsReadyFar(s32 owner, s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 owner, s32 element, s32 index);

s32 Func_08101a04(u32 entry) {
  s32 owner = (entry & 0xf00) >> 8;
  s32 element = (entry & 0xe0) >> 5;
  s32 index = entry & 31;

  if (Djinn_IsReadyFar(owner, element, index) ||
      Djinn_IsActiveFar(owner, element, index))
    return (entry & 0x8000) != 0;
  return 2;
}
