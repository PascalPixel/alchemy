#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "io_reg.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Object_SetWideSprite(s32 id, s32 wide);
void Func_020018e0(void);
void Func_020027e4(void);
void Func_02002878(void);
void Func_020029c8(void);
void Func_02002a38(void);
void Func_0200440c(s32 layer);
void Func_02004414(s32 layer);
void Func_0200459c(s32 a, s32 b);
void Func_020045a4(s32 frames);
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
  scene = gPartyState.scene;
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
    if (!Flag_Test(0xa3d))
      GameFlag_SetBit(0x201);
    if (Flag_Test(0x201)) {
      Func_0200461c(1);
    } else {
      {
        /* FAKEMATCH: the native loads the blend value from the pool before it
         * starts the register address; the plain store starts the address first
         * (one reordered row). Only a halfword register loads from the pool and
         * a u16 local is widened, so the value is pinned to the r3 it already
         * takes, as in overlay 696's s43f8.c. */
        register u16 alpha __asm__("r3") = 0xc08;

        /* FAKEMATCH: the one-pass block's loop note holds the address until the
         * pinned value is loaded; the empty asm of s43f8.c would move the
         * literal pool here (it counts four estimated bytes). */
        do {
        } while (0);
        REG_BLDALPHA = alpha;
      }
      REG_BLDCNT = 0x3f10;
      Scheduler_AddOrUpdateCallback(Func_020018e0, 0x480);
      Func_02004414(0);
      Func_02004414(1);
      Func_0200440c(2);
      Func_0200440c(3);
      Func_0200440c(4);
      Func_0200440c(5);
      Func_0200440c(6);
    }
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov6a8Room2) {
      if (!Flag_Test(0x300))
        Object_SetWideSprite(64, 1);
      Func_020027e4();
    } else if (scene == (s32)&SceneId_Ov6a8Room3) {
      Func_02002878();
    }
    if (Flag_Test(0x201)) {
      Func_0200459c(0x10004, 1);
      Func_020045a4(16);
      WaitFrames(16);
    }
  }
  return 0;
}
