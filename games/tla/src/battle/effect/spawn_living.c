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
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

/// Spawns the battle objects of each unit in the list that has HP left.
void BattleEffect_SelectLivingTargets(struct FxPartyList *party) {
  s16 list[14];
  s32 count = 0;
  s32 i;

  for (i = 0; i != party->count; i++) {
    if (((struct FxUnit *)Unit_Get(party->ids[i]))->hp > 0)
      list[count++] = party->ids[i];
  }
  list[count] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
}
