#include "PROTO.H"
#include "GLOBAL.H"
#include "RESOURCE.H"
#include "RAMAKAN.H"
#include "FIELD_EVENT.H"



struct Half {
    u16 v;
};

void RamakanSabaku_ClaimSandEffectVram(void)
{
    s32 slot;
    u16 *slotp;
    struct Half zero;

    Resource_DecodeType01(RamakanSabaku_SandEffectTiles, RamakanSabaku_SandTileBuffer);
    slotp = &RamakanSabaku_SandVramSlot;
    /* FAKEMATCH: the size temporary schedules the slot store after the call. */
    slot = Resource_FindFreeEntry();
    {
        s32 size = 0x480;

        *slotp = slot;
        Engine_VramLoad((s16)slot, size, 0);
    }
    zero.v = 0;
    RamakanSabaku_SandCounter = zero.v;
    RamakanSabaku_SandPhase = zero.v;
    Engine_TaskAddCallback(RamakanSabaku_RunSandstorm, 0xc76);
}

void RamakanSabaku_SetSandPhase(u16 value)
{
    RamakanSabaku_SandPhase = value;
}
