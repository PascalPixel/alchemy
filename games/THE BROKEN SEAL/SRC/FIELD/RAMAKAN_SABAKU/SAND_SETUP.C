#include "RAMAKAN.H"

extern u8 RamakanSabaku_SandEffectTiles[];
extern u8 RamakanSabaku_SandTileBuffer[];
extern u16 RamakanSabaku_SandVramSlot;
extern u16 RamakanSabaku_SandCounter;
extern u16 RamakanSabaku_SandPhase;

s32 Resource_DecodeType01();
s32 Resource_FindFreeEntry();
s32 Engine_VramLoad();
s32 Engine_TaskAddCallback();
void Func_020018a4(void);

/* A one-halfword struct keeps the zero a register copy, not a halfword pool
 * constant out of reach. */
struct Half {
    u16 v;
};

/* Claim a VRAM slot for the sand effect, load its tiles, clear two
 * counters and start the effect task. */
void RamakanSabaku_ClaimSandEffectVram(void)
{
    s32 slot;
    u16 *slotp;
    struct Half zero;

    Resource_DecodeType01(RamakanSabaku_SandEffectTiles, RamakanSabaku_SandTileBuffer);
    slotp = &RamakanSabaku_SandVramSlot;
    /* FAKEMATCH: the do-while wrap keeps the slot address in r5 across the
     * call, and the size temporary schedules the slot store after it. */
    do {
        slot = Resource_FindFreeEntry();
    } while (0);
    {
        s32 size = 0x480;

        *slotp = slot;
        Engine_VramLoad((s16)slot, size, 0);
    }
    zero.v = 0;
    RamakanSabaku_SandCounter = zero.v;
    RamakanSabaku_SandPhase = zero.v;
    Engine_TaskAddCallback(Func_020018a4, 0xc76);
}

void SceneState_SetHalfwordB030(u16 value)
{
    RamakanSabaku_SandPhase = value;
}
