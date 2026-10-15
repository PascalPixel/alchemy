#include "types.h"
#include "field_event.h"

void Func_02000c48();
void Func_02000ca8(s32 actor, s32 x, s32 z);

static inline void MapCopyAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: as in the scene layer of The Broken Seal, the inline parameters
   * keep the native registers (direct: the second stack constant reuses r3
   * after the first store) */
  Func_02000c48(a, b, c, d, e, f);
}

void Func_02000924(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(10, 19, 1, 1, 11, 19);
  Func_02000ca8(22, 0, 0);
  Engine_GameFlagSet(0x210);
  Engine_EventEnd();
}

void Func_02000960(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(25, 20, 1, 1, 26, 20);
  Func_02000ca8(23, 0, 0);
  Engine_GameFlagSet(0x211);
  Engine_EventEnd();
}

void Func_020009a0(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(28, 23, 1, 1, 29, 23);
  Func_02000ca8(24, 0, 0);
  Engine_GameFlagSet(0x212);
  Engine_EventEnd();
}

void Func_020009e0(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(17, 11, 1, 1, 18, 11);
  Func_02000ca8(25, 0, 0);
  Engine_GameFlagSet(0x213);
  Engine_EventEnd();
}

void Func_02000a20(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(30, 17, 1, 1, 31, 17);
  Func_02000ca8(26, 0, 0);
  Engine_GameFlagSet(0x214);
  Engine_EventEnd();
}

void Func_02000a5c(void) {
  Engine_EventBegin();
  Engine_EventResolvePendingActions(0);
  MapCopyAttr(16, 23, 1, 1, 17, 23);
  Func_02000ca8(31, 0, 0);
  Engine_GameFlagSet(0x215);
  Engine_EventEnd();
}
