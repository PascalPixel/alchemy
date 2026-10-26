// Draft of overlay 671 Func_02005428, differ 9 of 61 (was 38); overlay 6a0's
// Func_02001a48 is the same function with Func_02001acc for Func_020054ac.
// Entered at its test through a goto into the do-while, as the native enters
// it, and with the object pinned to r6 (as a plain local, 27), the loop has the
// native's shape and registers. Left: the native starts the end mark 0xffff
// (movs r1, #255) before the entry jump and again at the end of each pass and
// finishes it at the test, loading the word between the halves; it reads the
// list word before the id; and it copies the count to r10 after the mode. An
// end mark pinned to r1 and set before the jump and at the end of the body
// loses the first set and swaps r2 and r3 (18). The native's entry is a
// first test (if (*list == 0xffff) return; before a goto loop, or a loop with
// its exit test copied) that jump2's cross-jumping merged into the loop's own
// test after sched2 had moved the table pointer load between its first two
// rows: written so, this draft gets the same entry jump, but gcse then hoists
// the id load out of the loop (ldrh before it, the sign extension apart,
// 27); with the id read through a volatile pointer it stays inside (25, its
// extension still apart).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020054ac(s32 id, s32 a, s32 flag, s32 terrain);

// Finds the current target's table row and shrinks it.
void Func_02005428(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  goto test;
  do {
    s32 id = work->objectId;

    if (id == *list) {
      /* FAKEMATCH(pin): the object in r6, as the native keeps it; as a plain
       * local, 27 of 61 instructions differ in tla-en. */
      register struct ObjectRuntime *object asm("r6") = Object_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_020054ac(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}
