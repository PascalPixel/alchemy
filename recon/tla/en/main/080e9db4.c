// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 90 in tla-en. First difference: this issues `movs r2, #0` earlier (row
// 18) than the native does (row 24). No single empty asm or one-pass block
// reaches 0.
// Near miss of Func_080e9db4 (first written 2026-10-12): differ 4 of 90 since
// 2026-10-19 (6 on 2026-10-18), two reordered places. The permuter found a
// one-pass block from the render mode store to the gravity store (6 -> 4);
// ending it before the y or the gravity store is 6, an empty asm before the
// render mode store inside it 6, the zero taken from i 29. The first wait loop
// is entered at its height test with a goto, as the native branches to the test
// at the bottom (13 -> 6; a while loop with the count inside is 19). What
// remains: agscc sets the render mode's 0 before the motion flags' store,
// where the native sets it after the mode's offset, and starts the work
// offset 0x102c after the y store, where the native starts it before.
// Measured before the block: an empty asm before the render mode
// store (8), the adventure through a pointer behind a one-pass block (24)
// or a non-volatile asm (6), the pointer pinned to r3 behind a volatile asm
// (6, the zero then moves up).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "callback_scheduler.h"

s32 EventRuntime_GetControlledOwner(void);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void WaitFrames(s32 frames);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_080e9940(void);

void Func_080e9db4(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  struct ObjectRuntime *object =
      Object_GetById(EventRuntime_GetControlledOwner());
  s32 i;

  Scheduler_RemoveCallback(Func_080e9940);
  Object_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  do {
    gAdventure.renderMode = 0;
    i = 0;
    goto test;
    do {
      WaitFrames(1);
      if (++i > 59)
        break;
    test:;
    } while (object->y > object->terrainHeight);
    Sound_PlayFar(132);
    WaitFrames(1);
    for (i = 0; i < 20; i++) {
      if (object->y <= object->terrainHeight)
        break;
      WaitFrames(1);
    }
    WaitFrames(1);
    for (i = 0; i < 20; i++) {
      if (object->y <= object->terrainHeight)
        break;
      WaitFrames(1);
    }
    WaitFrames(1);
    object->bounce = 0;
    object->velocityY = 0;
    object->y = object->terrainHeight;
    object->gravity = 0x10000;
  } while (0);
  Resource_ResetEntry(*(s16 *)(work + 0x102c));
  Runtime_ReleaseHeapBlock(92);
}
