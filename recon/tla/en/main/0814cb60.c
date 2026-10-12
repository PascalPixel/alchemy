/* Near miss: listing 44 instructions, draft 44, common 41, differ 6; the rest differs in instruction order. Tried 2026-10-12: a barrier after the work load orders the first row but swaps r2 and r3 through the two stores; the handler table held in a local behind a barrier orders the second row but indexes it with add, sub, ldr [r3] in place of sub, ldr [r2, r3]. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "io_reg.h"
#include "iwram_call.h"

struct FxRunWork {
    u8 unknown_0000[0x78e4];
    s32 active;
    u8 unknown_78e8[8];
    s32 *script;
};

struct FxPartyList {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    s16 ids[8];
};

struct FxUnit {
    u8 unknown_00[56];
    s16 hp;
};

struct FxBgScroll {
    u8 unknown_00[6];
    u16 y;
};

extern void (*Data_08197aa0[])(s32 *script);
extern struct FxBgScroll gBgScroll[];
extern u32 gCameraSceneParameters[];

void *Runtime_AllocateHeapBlock(s32 block, s32 size);
void Runtime_ReleaseHeapBlock(s32 block);
void *UnitGet(s32 owner);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleBackground_LoadFar(s32, s32, s32);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

void Func_0814cb60(s32 *script)
{
    struct FxRunWork *work;

    Runtime_AllocateHeapBlock(100, 0x302);
    Runtime_AllocateHeapBlock(92, 0x7b7c);
    Runtime_AllocateHeapBlock(96, 0x4000);
    work = Ram_HeapSlots->battle_fx_work[0];
    work->script = script;
    work->active = 1;
    Data_08197aa0[*script - 1](script);
    Runtime_ReleaseHeapBlock(96);
    Runtime_ReleaseHeapBlock(92);
    Runtime_ReleaseHeapBlock(100);
}
