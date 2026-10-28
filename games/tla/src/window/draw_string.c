#include "heap_release.h"
#include "text_render.h"

void Text_DrawWideStringAtOffset(void *text, s32 work, s32 x, s32 y);

void Text_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3) {
  u16 *buffer = Heap_AllocEwram(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
    /* FAKEMATCH: ⚓️ steps the output before it reads the next byte. */
    __asm__("");
  }
  *output = 0;
  Text_DrawWideStringAtOffset(buffer, arg1, arg2, arg3);
  Heap_Release(buffer);
}
