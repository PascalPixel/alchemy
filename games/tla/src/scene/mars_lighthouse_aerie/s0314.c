#include "mars_lighthouse_aerie.h"
#include "io_reg.h"
#include "scene_tables.h"

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_020001d8(void);
void Func_02001f10(void);
void Func_020029ac(void);
void Func_02003658(void);
void Func_02003ff4(void);
void Func_0200558c(void);
void Func_0200567c(s32 id, s32 x, s32 z);
struct Actor *Func_0200573c(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);

/// Sets the scene up for the entrance the party arrived by (5 to 2).
s32 Func_02000314(void) {
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  Func_020001d8();
  Call1((void (*)())GameFlag_SetBit, 0x112);
  if (gMainRoutineWork.entrance == 5) {
    {
      /* FAKEMATCH(pin): the blend word pinned to r3 as a halfword loads from
       * the pool before the register address, as native; as a plain store, 2
       * of 121 instructions differ in tla-en. */
      register u16 alpha asm("r3") = 0xc08;

      /* FAKEMATCH(barrier): the barrier of the pin above: it holds r2 until
       * the value is loaded. */
      __asm__("" : "+l"(alpha) : : "r2");
      REG_BLDALPHA = alpha;
    }
    REG_BLDCNT = 0x3f10;
    Func_02003ff4();
  } else if (gMainRoutineWork.entrance == 4) {
    Func_0200560c();
    Call1((void (*)())Func_02005784, 0);
    Call2((void (*)())Actor_SetModeById, 20, 8);
    Call2((void (*)())Actor_SetModeById, 21, 8);
    Call2((void (*)())Actor_SetModeById, 22, 7);
    Call3((void (*)())Func_0200567c, 19, PIXELS(498), PIXELS(320));
    Call3((void (*)())Func_0200567c, 1, PIXELS(538), PIXELS(320));
    Call3((void (*)())Func_0200567c, gMainRoutineWork.currentOwner, PIXELS(506),
          PIXELS(280));
    Call1((void (*)())wait, 1);
    Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    ((struct Actor *)Value0((s32 (*)())Func_0200573c))->motionFlags = 0;
    Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(512), PIXELS(96),
          PIXELS(312), 1);
    Func_0200558c();
    Call1((void (*)())wait, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call1((void (*)())FieldEvent_Wait, 40);
    Func_02003658();
    Func_02005614();
  } else if (gMainRoutineWork.entrance == 3) {
    Call2((void (*)())Actor_SetModeById, 20, 8);
    Call2((void (*)())Actor_SetModeById, 21, 8);
    Call2((void (*)())Actor_SetModeById, 22, 7);
    if (Value1((s32 (*)())GameFlag_Test, 0xa3d) == 0)
      Func_020029ac();
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02001f10();
  }
  return 0;
}
