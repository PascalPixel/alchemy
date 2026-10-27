#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "macros.h"

void Event_ShowCounterAtPosition();

void EventRuntime_ShowCounter(s32 a, s32 b, s32 c, s32 d) {
  Event_ShowCounterAtPosition(a, b, c, d);
}

void Event_ShowCounterAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  s32 x0 = x;
  struct EventWork *state = Ram_HeapSlots->eventWork;
  s32 py = y;
  s32 px = x0;
  s32 minX = 8;
  s32 minY = 20;
  s32 ret;

  if (py > 119)
    py += 32;
  else
    py -= 32;

  if (x0 < minX)
    px = minX;
  if (px > 312)
    px = 312;
  CLAMP(py, minY, 220);

  ret = UiText_OpenMessageWindowFar((s16)state->messageId, px, py, 1);
  while (UiWork_IsIdleFar(ret) == 0)
    WaitTicks(1);
  state->messageId++;
}
