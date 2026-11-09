#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern u8 MsgLemuriaStayExploring[];
extern u8 MsgLemuriaStayExploringAlternate[];

void Func_02000080(s32 id);
void Func_02001914(void);
void Func_0200191c(void);

/// Once flag 0x96c is set and 0x96d is not: sets 0x96d, clears 0x96e, launches
/// object 12 from 22 pixels up at half size and turns it an eighth of a turn a
/// frame for 24 frames while it grows back to full size, then leaves it facing
/// 0x3000.
void Func_02000468(void) {
  struct Actor *object;
  s32 i;
  struct Actor *p;

  if (Event_GetFlag(0x96d))
    return;
  if (!GameFlag_Test(0x96c))
    return;
  object = Actor_GetById(12);
  GameFlag_SetBit(0x96d);
  GameFlag_ClearBit(0x96e);
  Func_02000080(13);
  Actor_SetPosition(12, 270 << 16, 184 << 16);
  object->y = 22 << 16;
  Actor_Jump(12, 11, 0);
  object->scaleY = 0x8000;
  object->scaleX = 0x8000;
  Actor_SetSpeed(12, 0x23333, 0x11999);
  Actor_WalkTo(12, 312, 184);
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
    wait(1);
  }
  p->angle = 0x3000;
  p->scaleY = 0x10000;
  p->scaleX = 0x10000;
  Actor_WaitForMoveById(12);
}
