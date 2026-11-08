/// @file
/// Windows: expand a message into a string without a length limit.

#include "types.h"
#include "scene.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

void Message_Decode(s32 key, s32 mode);

/// Expands message into buf without an upper length bound.
void Message_DecodeRawUnbounded(s32 message, s32 buf) {
  Message_DecodeRaw(message, buf, 0x7fffffff);
}

/// Builds message key's text entries and copies them to destination, at most
/// capacity - 1 characters and a terminator; returns the characters copied.
s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity) {
  struct WindowWork *work;
  u32 count;

  work = UI_WORK;
  work->count = 0;
  Message_Decode(key, 1);
  for (count = 0;
       count < capacity - 1 && (destination[count] = work->entries[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}
