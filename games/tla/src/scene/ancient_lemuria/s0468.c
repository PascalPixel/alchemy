#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern u8 MsgLemuriaStayExploring[];
extern u8 MsgLemuriaStayExploringAlternate[];

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
void ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_02000080(s32 id);
void Func_02001914(void);
void Func_0200191c(void);
void Func_0200195c(s32 id, s32 x, s32 z);
void Func_02001994(u8 *message);
void Func_020019a4(s32 id, s32 a, s32 b);
void Func_020019ec(s32 value);
void Func_020019fc(s32 value);
void Func_02001a0c(s32 id, s32 a, s32 b, s32 c);
void Func_02001a2c(s32 value);

/// Once flag 0x96c is set and 0x96d is not: sets 0x96d, clears 0x96e, launches
/// object 12 from 22 pixels up at half size and turns it an eighth of a turn a
/// frame for 24 frames while it grows back to full size, then leaves it facing
/// 0x3000.
void Func_02000468(void) {
  struct ObjectRuntime *object;
  s32 i;
  struct ObjectRuntime *p;

  if (Value1(GameFlag_Test, 0x96d))
    return;
  if (!GameFlag_Test(0x96c))
    return;
  object = Object_GetById(12);
  Call1(GameFlag_SetBit, 0x96d);
  GameFlag_ClearBit(0x96e);
  Func_02000080(13);
  Call3(Func_0200195c, 12, 270 << 16, 184 << 16);
  object->y = 22 << 16;
  ObjectMotion_Launch(12, 11, 0);
  object->scaleY = 0x8000;
  object->scaleX = 0x8000;
  Call3(ObjectMotion_SetSpeedParameters, 12, 0x23333, 0x11999);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 12, 312, 184);
  for (i = 23; i >= 0; i--) {
    {
      /* FAKEMATCH(temporary): the turn in its own register; as a plain
       * 0x2000, 24 of 85 instructions differ in tla-en. */
      s32 turn = 0x2000;

      object->angle += turn;
    }
    /* FAKEMATCH(temporary): the object read again through p after the
     * turn, so the scale is reloaded after its test; as object throughout, 13
     * of 85 instructions differ in tla-en. */
    p = object;
    if (p->scaleX <= 0xffff)
      p->scaleX += 0xa00;
    p->scaleY = p->scaleX;
    WaitTicks(1);
  }
  p->angle = 0x3000;
  p->scaleY = 0x10000;
  p->scaleX = 0x10000;
  ObjectMotion_CommitCurrentPositionAndActivate(12);
}
