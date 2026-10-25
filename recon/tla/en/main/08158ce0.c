// Near miss of Func_08158ce0, ported from The Broken Seal's Camera_ApplyShake
// (listing 61 instructions, draft 61, differ 4, one reordered pair): the
// native copies shake_range (adds r7, r1, #0) before it starts the 0x77a8
// offset (movs r0, #238) and loads the effect work between the offset's first
// two instructions; here the offset starts first. sched2's priorities give
// exactly the native order if a load's latency is 3 cycles instead of
// arm9tdmi's 2. Tried: the heap base read into a local held by
// asm volatile("" : "+l"(slots)) before the work load (differ 2: lsls r3
// then falls after the parameter copies), a one-pass block there (differ 4),
// barriers or "+l" asms on work, shake_range or remainingFrames (differ
// 4 to 8), alchemy permute 4 minutes (no better).
// 2026-10-24: 238 pinned to r0 and held by an asm whose input is the work
// pointer, with the offset built as (238 << 7) + 168 and then added to the
// work, gives the native's prologue exactly (the parameter copies, movs r0,
// #238, the work load, then the shift). It costs the decrement's reload
// register: reload picks r2 but the final code reads the pointer back
// through r0 (mov r0, r8), where the native uses r2 (differ 6). Without the
// pin the offset takes r3 and the heap base r2 (26); a second asm holding
// 238 alone, or the sum held by a third, change nothing.
#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

void Func_08158ce0(s32 randomMask, u32 shakeRange) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (randomMask - 1) & Random16();
    s32 halfRange = (s32)shakeRange / 2;
    s32 offsetY = ((shakeRange - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + 32;
    gCameraSceneParameters[3] = 120 - offsetX;
    gCameraSceneParameters[4] = 120 - offsetY;
    (*remainingFrames)--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
}
