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

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

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
    /* FAKEMATCH: the native loads the call's argument before the store; the
     * plain order stores first (one reordered pair, differ 2). The argument is
     * pinned to the r0 it is passed in. */
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
      /* FAKEMATCH: the value pinned to r3 loads from the pool before the
       * register address starts, as the native does; as a plain store, one
       * row is reordered in tla-en. */
      register u16 alpha __asm__("r3") = 0xc08;

      REG_BLDALPHA = alpha;
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
