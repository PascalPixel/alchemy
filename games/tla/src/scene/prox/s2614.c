#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "event_inline.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Actor_SetWideSprite(s32 id, s32 wide);
void Func_020018e0(void);
void Func_020027e4(void);
void Func_02002878(void);
void Func_020029c8(void);
void Func_02002a38(void);
void Func_0200440c(s32 layer);
void Func_02004414(s32 layer);
void Func_020045dc(s32 a);
void Func_0200461c(s32 a);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

s32 Func_02002614(void) {
  s32 scene;

  if (Flag_Test(0xa3d)) {
    GameFlag_SetBit(0x17c);
    GameFlag_SetBit(0x144);
  }
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov6a8Room4) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_020029c8();
  } else if (scene == (s32)&SceneId_Ov6a8Room5) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_02002a38();
  } else if (scene == (s32)&SceneId_Ov6a8Room6) {
    s32 *effect =
        &((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect;
    /* FAKEMATCH(pin): the call's argument pinned to the r0 it is passed in, so
     * it loads before the store as the native does; in the plain order the
     * store comes first; as ...->screenEffect = 0x209 and Func_020045dc(0), as
     * the other rooms write it, 2 of 147 instructions differ in tla-en. */
    register s32 skip __asm__("r0");
    s32 value = 0x209;

    skip = 0;
    *effect = value;
    Func_020045dc(skip);
  } else {
    if (!GameFlag_Test(0xa3d))
      GameFlag_SetBit(0x201);
    if (GameFlag_Test(0x201)) {
      Func_0200461c(1);
    } else {
      REG_BLDALPHA = 0xc08;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      Task_Add(Func_020018e0, 0x480);
      Func_02004414(0);
      Func_02004414(1);
      Func_0200440c(2);
      Func_0200440c(3);
      Func_0200440c(4);
      Func_0200440c(5);
      Func_0200440c(6);
    }
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov6a8Room2) {
      if (!GameFlag_Test(0x300))
        Actor_SetWideSprite(64, 1);
      Func_020027e4();
    } else if (scene == (s32)&SceneId_Ov6a8Room3) {
      Func_02002878();
    }
    if (GameFlag_Test(0x201)) {
      BattleEffect_ApplyColorToTargetBuffer(0x10004, 1);
      BattleEffect_StartBufferInterpolation(16);
      wait(16);
    }
  }
  return 0;
}
