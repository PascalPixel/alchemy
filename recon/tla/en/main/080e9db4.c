// Near miss of Func_080e9db4 (first written 2026-10-12): differ 6 of 90 since
// 2026-10-18, two reordered places. The first wait loop is entered at its
// height test with a goto, as the native branches to the test at the bottom
// (13 -> 6; a while loop with the count inside is 19). What remains: the
// native loads gPartyState's address before it builds the render mode's
// offset (ldr r3; movs r1, #128), and finishes the gravity store before the
// work offset 0x102c (movs r3; lsls r3; str; lsls r2; adds r2); agscc takes
// the other order in both. Measured: an empty asm before the render mode
// store (8), the party state through a pointer behind a one-pass block (24)
// or a non-volatile asm (6), the pointer pinned to r3 behind a volatile asm
// (6, the zero then moves up).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
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
  gPartyState.renderMode = 0;
  i = 0;
  goto test;
  do {
    WaitFrames(1);
    if (++i > 59)
      break;
  test:;
  } while (object->y > object->terrainHeight);
  Audio_PlayCue(132);
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
  Resource_ResetEntry(*(s16 *)(work + 0x102c));
  Runtime_ReleaseHeapBlock(92);
}
