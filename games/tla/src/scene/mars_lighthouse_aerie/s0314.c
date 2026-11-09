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
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Sets the scene up for the entrance the party arrived by (5 to 2).
s32 Func_02000314(void) {
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  Func_020001d8();
  Event_SetFlag(0x112);
  if (gMainRoutineWork.entrance == 5) {
    /* FAKEMATCH(pin): the blend word pinned to r3 as a halfword loads from
     * the pool before the register address, as native; as a plain store, 2
     * of 121 instructions differ in tla-en. */
    register u16 alpha asm("r3") = 0xc08;

    REG_BLDALPHA = alpha;
    REG_BLDCNT = 0x3f10;
    Func_02003ff4();
  } else if (gMainRoutineWork.entrance == 4) {
    Func_0200560c();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    Actor_SetPosition(19, PIXELS(498), PIXELS(320));
    Actor_SetPosition(1, PIXELS(538), PIXELS(320));
    Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
    System_Wait(1);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_MoveTo(PIXELS(512), PIXELS(96), PIXELS(312), 1);
    Func_0200558c();
    System_Wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(40);
    Func_02003658();
    Func_02005614();
  } else if (gMainRoutineWork.entrance == 3) {
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    if (Event_GetFlag(0xa3d) == 0)
      Func_020029ac();
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02001f10();
  }
  return 0;
}
