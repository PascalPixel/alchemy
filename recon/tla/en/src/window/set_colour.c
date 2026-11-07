#include "types.h"
#include "window.h"
#include "ram_buffer.h"

void Func_08041f70(s32 param) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  work->colour = param & 15;
}
