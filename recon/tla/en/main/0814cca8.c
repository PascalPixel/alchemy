/* Near miss: listing 57 instructions, draft 57, common 48, differ 18; the rest differs in instruction order. */
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

void Func_0814cca8(void)
{
    u8 *battle = Ram_HeapSlots->battle_work;
    void *buffer = Ram_HeapSlots->battle_fx_work[1];

    REG_DISPCNT = 1;
    gBgScroll[0].y = 32;
    BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
    {
        s32 size = 0x4000;

        /* FAKEMATCH: ⚓️ builds the fill size anew for each fill */
        asm("" : "+l"(size));
        Iwram_FillWords(buffer, size, 0);
    }
    {
        s32 size = 0x4000;

        /* FAKEMATCH: ⚓️ builds the fill size anew for each fill */
        asm("" : "+l"(size));
        Iwram_FillWords((void *)0x06004000, size, 0);
    }
    WaitFrames(1);
    REG_BLDALPHA = 0x100e;
    REG_BLDCNT = 0x3f46;
    REG_DISPCNT = 0x7741;
    gCameraSceneParameters[4] = 120;
    Audio_PlayCue(390);
}
