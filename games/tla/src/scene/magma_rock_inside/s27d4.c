#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "local.h"

s32 Func_02000658(s32 kind, s32 x, s32 z);
void Func_0200257c(void);
void Func_020026a4(s32 x, s32 y);

/// Reads what the leader stands on (Func_02000658, kind 2): on cells 1 and 7
/// it only redraws the marker when flag 0x200 is set; elsewhere it clears
/// (Func_0200257c), redraws the marker in the row the event work's counter
/// gives, sets flag 0x200 and the counter's flag (312 on).
void Func_020027d4(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  cell = Func_02000658(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_0200257c();
    Func_020026a4(0x108, row * 8 + 10);
    if (!Event_GetFlag(0x200))
      Event_SetFlag(0x200);
    Event_SetFlag(*(s16 *)work + 312);
  } else if (Event_GetFlag(0x200)) {
    Func_020026a4(0x108, row * 8 + 10);
  }
}

void Func_02002868(void) {
  if (!Event_GetFlag(0x305)) {
    Actor_SetSpriteColor(Actor_Get(11), 15);
    Actor_SetPosition(11, PIXELS(136), PIXELS(360));
    Actor_Get(11)->scaleX = Actor_Get(11)->scaleY = 0x1000;
  }
}
