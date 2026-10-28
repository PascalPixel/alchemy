#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

extern char SceneId_Ov6A6Room1;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Object_SetWideSprite(s32 id, s32 wide);
void ObjectMotion_EnableActionAndSetCallback(s32 id, s32 callback);
void Func_02001080(void);
void Func_020010a0(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_02001350(s32 id, s32 x, s32 z, u16 angle);

static __inline__ void Func_02001350_(s32 id, s32 x, s32 z, u16 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, so each call builds its angle
   * first; as plain calls, 14 of 121 instructions differ in tla-en. */
  Func_02001350(id, x, z, angle);
}

s32 Func_020010c0(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov6A6Room1) {
    if (!GameFlag_Test(0x300))
      Object_SetWideSprite(64, 0);
    if (!GameFlag_Test(0x301))
      Object_SetWideSprite(65, 1);
    if (!GameFlag_Test(0x302))
      Object_SetWideSprite(66, 1);
    Func_02001080();
  } else {
    Func_020010a0();
  }
  if (GameFlag_Test(0xa5f)) {
    Func_02001348(20, 0, 0);
    Func_02001350_(14, 0xf80000, 0x1e00000, 0);
    if (!GameFlag_Test(0x90a)) {
      Func_02001348(17, 0, 0);
      Func_02001348(16, 0, 0);
      Func_02001348(18, 0, 0);
    } else {
      Func_02001350_(16, 0x1cb0000, 0xfe0000, 0xe000);
      Func_02001350_(18, 0x1ba0000, 0xcb0000, 0x8000);
      Func_02001350_(17, 0x1ea0000, 0xbc0000, 0x2000);
      Object_GetById(17)->action = Object_GetById(17)->x >> 16;
      Object_GetById(17)->unk66 = Object_GetById(17)->z >> 16;
      ObjectMotion_EnableActionAndSetCallback(17, 2);
    }
  }
  return 0;
}
