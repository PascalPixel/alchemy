#include "room.h"
#include "call.h"
#include "io_reg.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02000df0(void);
void Func_02004d00(s32 id, s32 a1, s32 a2);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);

void Func_02000fe0(const u8 *event) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 z = leader->z;
  // The leader's map column and, later, the frame count share one variable,
  // as the native's register shows.
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Func_02004ca8();
  Func_02004da0(0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  leader->update = 0;
  Func_02004d00(gPartyState.currentOwner, 0x100, 0);
  Func_02004bc0(leader, 27);
  ObjectDispatch_SetAnimationSpeed(leader, 48);
  WaitTicks(30);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  if ((event[3] & 64) == 0) {
    {
      /* FAKEMATCH(temporary): the window mask is built in a word register, as
       * the native builds it; as REG_WINOUT = 0xf1f, 18 of 130 instructions
       * differ in tla-en. */
      u16 winout = 0xf1f;

      REG_WINOUT = winout;
      /* FAKEMATCH(one-pass): the display control's address is built after
       * the window store, as in the native; without it, 2 of 130
       * instructions differ in tla-en (an empty asm there: 14). */
      do {
      } while (0);
    }
    REG_DISPCNT |= 0x8000;
    Func_02000df0();
  }
  Object_SetModeById(gPartyState.currentOwner, 28);
  FieldEvent_WaitTicks(16);
  i = 0;
next:
  if (i == 5)
    Func_02004de0(204);
  leader->scaleX -= 0x400;
  leader->scaleY -= 0x300;
  leader->y -= 0x9999;
  WaitTicks(1);
  i++;
  if (i <= 39)
    goto next;
  Object_GetById(gPartyState.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d30(11);
}
