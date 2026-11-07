#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ScreenWork {
  u8 unk000[0x5a4];
  struct WindowSprite *cursor;
};

struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                  s32 style);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void Func_08108048(void *cursor, s32 x, s32 y);

struct RenderInput *Func_080409f0(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;

  window = Window_Create(7, 3, 15, 9, 2);
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     OUTPUT_SIZE_16X16, window, 0, 0);
  {
    /* FAKEMATCH(pin): the cursor's address held in r0 for the barrier to
     * read; as &work->cursor read by the barrier directly, 4 of 39
     * instructions differ in tla-en. */
    register struct WindowSprite **cursor asm("r0") = &work->cursor;
    /* FAKEMATCH(barrier): the empty asm reads r0 and rewrites window, so
     * the window's loads wait for the address copy; as the plain call with
     * no asm, 4 of 39 instructions differ in tla-en. */
    asm("" : "+r"(window) : "r"(cursor));
    Func_08108048(&work->cursor, ((s16)window->x << 3) - 4,
                  ((s16)window->y << 3) + 12);
  }
  return window;
}
