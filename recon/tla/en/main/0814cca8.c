// Near miss, differ 2 of 59 (2026-10-25, under -mthumb-slow-loads; 6 as plain
// C). The native loads the fill routine's entry before its arguments, held by
// the tagged device below; the DISPCNT and BLDALPHA pins of 2026-10-19 are
// no longer needed, and with the device the battle work also loads before the
// canvas, as the native does. Left: the first fill sets r0 from the canvas
// (adds r0, r6) before it starts the size in r1, where the native starts the
// size first. An empty asm in place of the entry's one-pass block puts every
// row in the native's order, but its four estimated bytes push the halfword
// pool's 64-byte range, and the pool lands before the second fill (differ 10).
// Tried there: the entry passed to the forwarder or pinned to r5 (differ 2,
// or the size shared in r6, 27 to 31), the forwarder's parameters in another
// order (2), the second destination behind a block (2), alchemy permute 4
// minutes (2).
// 2026-10-25: an empty asm after or before the entry, also with the second
// fill called directly, still moves the pool (differ 14 or more).
// Earlier (differ 18): the two pool values pinned behind barriers and the
// fills through a forwarder; the entry in a local behind a one-pass block
// without the forwarder shares the size (differ 41); the two heap reads in
// TBS's slots form, battle first (21), canvas first (43); the reads after
// REG_DISPCNT (34); the fills without barriers (more registers saved).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "io_reg.h"
#include "iwram_call.h"

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct FxPartyList {
  u8 unk00[20];
  s32 count;
  u8 unk18[12];
  s16 ids[8];
};

struct FxUnit {
  u8 unk00[56];
  s16 hp;
};

struct FxBgScroll {
  u8 unk00[6];
  u16 y;
};

extern void (*gData_08197aa0[])(s32 *script);
extern struct FxBgScroll gBgScroll[];
extern u32 gCameraSceneParameters[];

void *Runtime_AllocateHeapBlock(s32 block, s32 size);
void Runtime_ReleaseHeapBlock(s32 block);
void *Unit_Get(s32 owner);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleBackground_LoadFar(s32, s32, s32);
void WaitTicks(s32 ticks);
void Audio_PlayCue(s32 cue);

typedef s32 (*FillWordsFunc)(void *destination, s32 size, u32 value);

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  FillWordsFunc fill;

  /* FAKEMATCH(one-pass): the entry taken behind a one-pass block, so it loads
   * before the arguments; as Iwram_FillWords(destination, size, value), 6 of
   * 59 instructions differ in tla-en (the entry loads after the size). */
  do {
    fill = Iwram_FillWords;
  } while (0);
  fill(destination, size, value);
}

void Func_0814cca8(void) {
  u8 *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleFxWork[1];

  REG_DISPCNT = 1;
  gBgScroll[0].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  FillWords_(buffer, 0x4000, 0);
  FillWords_((void *)0x06004000, 0x4000, 0);
  WaitTicks(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  Audio_PlayCue(390);
}
