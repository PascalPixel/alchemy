#include "local.h"
#include "edition.h"

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room9;

static __inline__ void Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020020c0(id, x, z);
}

s32 Func_02000b18(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov690Room9) {
    if (Flag_Test(0x205)) {
      CopyCells2(9, 11, 1, 1, 11, 12);
      Func_02002018(12, 32, 11, 15, 1, 1);
      Func_02002018(77, 15, 75, 15, 1, 1);
    }
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (Flag_Test(0x205))
      Func_02002030(9, 11, 1, 1, 11, 11);
#else
    if (GameFlag_Test(0x204)) {
      Func_020020c0(9, 0, 0);
      Place(21, 0xb80000, 0xb80000);
      Object_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  /* FAKEMATCH: the native reads the scene again here, address and all; without
   * the barrier the address is kept in a register across the first block
   * (differ 11). */
  __asm__("" : : : "memory");
  if (gPartyState.scene == (s32)&SceneId_Ov690Room1) {
    if (GameFlag_Test(0xa60)) {
      Func_02002018(5, 46, 5, 47, 1, 1);
      Func_02002018(4, 25, 5, 17, 1, 1);
      CopyCells2(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}
