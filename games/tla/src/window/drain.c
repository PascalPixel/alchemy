#include "window.h"
#include "system.h"
#include "ram_buffer.h"

void Window_CloseAll(void) {
  struct WindowWork *state;
  struct UiChannelSlot *slot;
  struct Window *direct;
  u32 done;
  struct Window *work;
  struct Window *pollWork;
  s32 index;

  state = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  slot = state->channels;
  direct = state->windows;
  index = 0;
  do {
    work = slot->work;
    if (work != 0 && work->flags != 0)
      Window_Close(work, 0);
    index++;
    slot++;
  } while (index != 3);

  for (;;) {
    done = 1;
    slot = state->channels;
    index = 0;
    do {
      pollWork = slot->work;
      if (pollWork != 0) {
        if ((*(s32 *)&pollWork->frame) == 0) {
          if (pollWork->flags == 0)
            slot->work = NULL;
          else
            done = 0;
        } else {
          done = 0;
        }
      }
      index++;
      slot++;
    } while (index != 3);
    index = 0;
    if (done)
      break;
    wait(1);
  }
  while (index != UI_WINDOW_COUNT) {
    if (direct->flags != 0)
      Window_Close(direct, 0);
    direct++;
    index++;
  }
}
