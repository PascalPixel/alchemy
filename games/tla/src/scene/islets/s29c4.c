#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"
#include "edition.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct ObjectRuntime *)Value1(Object_GetById, (id)))

extern const u8 gData_02004508[];
extern const u8 gData_02004554[];
extern const u8 MsgWellHereWe[];
s32 Object_GetById();
void Func_02003eec();
void Func_02003edc();
void WaitFrames();
void Object_SetPartAttribute();
void Object_SetModeById();
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetSpeedParameters();
void Func_02003dec();
void ObjectMotion_EnableActionAndSetCallback();
void Func_02003f3c();
void Func_02003fac();
void Func_02003f24();
void Func_02003f2c();
void ObjectMotion_Launch();
void ObjectMotion_SetPositionAndCommit();
void Func_02003fb4(void);
void Func_020034d8(void);
void Func_02001278(void);
void Func_02000df8(void);

/// The Islets' arrival by entrance 4: actor 9 joins the leader and actor 10
/// is placed, the screen fades in, both run their action scripts
/// (Func_02000df8 updates) until actor 9's byte 0x63 is set, then actor 9
/// speaks (MsgWellHereWe) and the leader is launched up to (152, 742) and
/// turned; ends with Func_020034d8. The Japanese edition does not set the
/// event work's transition word 0x1ac.
void Func_020029c4(void) {
  struct ObjectRuntime *leader = OBJECT(gAdventure.currentOwner);
  struct ObjectRuntime *guide = OBJECT(9);
  u8 *done;

  Call2(Func_02003eec, 9, gAdventure.currentOwner);
  Call3(Func_02003edc, 10, 0xf00000, 0x21c0000);
  Call1(WaitFrames, 1);
  guide->angle = 0x4000;
  leader->update = Func_02001278;
  Call2(Object_SetPartAttribute, Value1(Object_GetById, gAdventure.currentOwner),
        15);
  Call2(Object_SetModeById, 9, 3);
  Call1(WaitFrames, 1);
#if (LANGUAGE != LANGUAGE_JAPANESE) /* The Japanese edition leaves the transition word. */
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3(ObjectMotion_SetSpeedParameters, 9, 0xcccc, 0x6666);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
  OBJECT(10)->update = Func_02000df8;
  guide->update = Func_02000df8;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Call2(Func_02003dec, (s32)guide, gData_02004508);
  Call2(ObjectMotion_EnableActionAndSetCallback, 10, gData_02004554);
  do
    Call1(WaitFrames, 1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Call2(Func_02003f3c, 9, 0xc000);
  Call2(Func_02003fac, gAdventure.currentOwner, 9);
  Call1(Func_02003f24, MsgWellHereWe);
  Call2(Func_02003f2c, 9, 0);
  Call2(Object_SetModeById, 9, 1);
  Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0x19999,
        0xcccc);
  Call2(Object_SetPartAttribute, Value1(Object_GetById, gAdventure.currentOwner),
        0);
  Call3(ObjectMotion_Launch, gAdventure.currentOwner, 6, 0);
  Call3(ObjectMotion_SetPositionAndCommit, gAdventure.currentOwner, 152, 742);
  Call2(Func_02003f3c, gAdventure.currentOwner, 0x4000);
  Call2(Func_02003f2c, 9, 0);
  Func_02003fb4();
  Call2(Func_02003f3c, 9, 0x4000);
  Func_020034d8();
#if (LANGUAGE != LANGUAGE_JAPANESE)
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
#endif
}
