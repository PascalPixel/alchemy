#include "local.h"
#include "edition.h"

extern char SceneId_Ov690Room3;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room6;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room9;

extern const u8 gData_02002272[];

void Actor_SetModeById(u32 id, s32 mode);
void Func_02000860(void);
void Func_02000e04(void);
void Func_0200125c(void);
void Func_02002140(s32 a);
void Func_02002188(const void *data);

static __inline__ void Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020020c0(id, x, z);
}

s32 Func_02000988(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room7)
    Func_02000860();
  /* FAKEMATCH: the native reads the scene again at each test; without the
   * barrier the read is moved up both paths (differ 56 over the four tests). */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    Func_02002140(0);
    Func_02002188(gData_0200226c);
    if (GameFlag_Test(0x206))
      Func_020020c0(10, 0, 0);
    Actor_GetById(10)->priorityFlags |= 2;
    Actor_SetModeById(18, 2);
    Actor_SetModeById(19, 2);
    Actor_SetModeById(21, 2);
    if (GameFlag_Test(0x205))
      Actor_SetModeById(10, 2);
    Func_02000e04();
#if (LANGUAGE != LANGUAGE_JAPANESE)
    if (GameFlag_Test(0x204)) {
      Func_020020c0(9, 0, 0);
      Place(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4) {
    if (GameFlag_Test(0x230)) {
      CopyCells2(0, 3, 1, 1, 3, 15);
      Actor_SetModeById(9, 4);
    }
    if (GameFlag_Test(0x109) && Flag_Test(0x220)) {
      Flag_Clear(0x220);
      Func_0200125c();
    }
    Func_02002188(gData_02002272);
  }
  /* FAKEMATCH: as at the second test. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room3)
    Func_02002140(0);
  /* FAKEMATCH: as at the second test. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room6)
    Func_02002140(0);
  return 0;
}
