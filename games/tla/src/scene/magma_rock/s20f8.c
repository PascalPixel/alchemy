#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "magma_rock.h"
#include "macros.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_020020d0(struct ObjectRuntime *object);
void Func_02002024(s32 x, s32 z);
void Func_02003820(s32 x, s32 y, s32 z, s32 kind);
void Func_020038dc(void);
void Func_02003998(void);
void Func_02003a94(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

/// Func_020014bc's counterpart: on cue 0 it hands the target to Func_020020d0;
/// modes 2 and 3 run Func_02002024 at the target's cell, and mode 3 also clears
/// its position and copies the cells at columns 32 and 96.
void Func_020020f8(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0) {
    struct ObjectRuntime *object;

    Func_020038dc();
    object = work->target;
    BYTE_AT(object, 99) = 4;
    object->update = (void *)Func_020020d0;
  }
  if (mode == 2) {
    Func_02002024(work->target->x, work->target->z);
    Func_02003820(work->target->x, work->target->y + 0xf0000, work->target->z,
                  1);
  }
  if (mode == 3) {
    struct ObjectRuntime *target;
    /* FAKEMATCH(temporary): the attribute copies' last argument as a
     * local set first keeps it in r6; as the constant 8, 12 of 104
     * instructions differ in tla-en. */
    s32 last = 8;

    Func_02002024(work->target->x, work->target->z);
    Func_02002024(work->target->x, work->target->z);
    Func_02002024(work->target->x, work->target->z - 0x200000);
    Func_02003820(work->target->x, work->target->y + 0xf0000, work->target->z,
                  16);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
    Func_02003aa4(0, 32, 5, 8, 42, 17);
    Func_02003aa4(0, 96, 5, 8, 42, 81);
    Func_02003a94(0, 32, 42, 17, 5, last);
    Func_02003a94(0, 96, 42, 81, 5, last);
  }
  if (cue == 0x3e7)
    Func_02003998();
}
