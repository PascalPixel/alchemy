#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "battle.h"
#include "system.h"

s32 Func_080237c8(u8 *object)
{
    u8 *track = *(u8 **)(object + 0x50);

    if (object != NULL && track != NULL && track != (u8 *)-0x28)
        return **(s16 **)(track + 0x28);
    return 0;
}

void Animation_WaitForTrackEnd(void *track);

void Func_080237e8(u8 *object)
{
    if (object != NULL)
        Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

void Func_0802386c(void);
void Func_08023e18(void);

#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// スペイン語版とイタリア語版のビルドでは、この 2 つの far call の名前が違う（shop.c と同じ）
void Func_080c8368(s32, s32);
void BattleFx_ApplyColorToSourceBufferFar(s32);
#endif

void Graphics_EnableObjLayerAndCallbacks(void)
{
    u16 dispcnt;

    Scheduler_EnableCallbacks(Func_0802386c);
    Scheduler_EnableCallbacks(Func_08023e18);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Func_080c8368(0x10000, 1);
    BattleFx_ApplyColorToSourceBufferFar(1);
#else
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
#endif
    WaitFrames(1);
    dispcnt = REG_DISPCNT;
    /* FAKEMATCH: ⚓️ reads DISPCNT before it builds the layer mask; the plain statement starts the mask first (one pair of instructions swapped). */
    asm volatile("");
    REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    u16 dispcnt;

    Scheduler_DisableCallbacks(Func_0802386c);
    Scheduler_DisableCallbacks(Func_08023e18);
    dispcnt = REG_DISPCNT;
    /* FAKEMATCH: ⚓️ reads DISPCNT before it builds the layer mask; the plain statement starts the mask first (one pair of instructions swapped). */
    asm volatile("");
    REG_DISPCNT = dispcnt & 0xe1ff;
}

s32 ObjectDispatch_ReturnTrue(void)
{
    return 1;
}
