/* Draft, differ 2 of 60: in the first blend write the native finishes the value (adds r3, #6) before the register address (adds r5, #82); this form finishes the address first. The inline's s32 parameter builds each value with movs and lsls; a direct halfword store loads them from the pool (differ 49), and passing the register to the inline is differ 20. */
#include "../../../../games/tla/src/field/overlay_678/local.h"


struct PartyState254 {
    u8 unknown_000[0x254];
    u16 value;
};

void Func_020009c0(s32 id);
void Func_02000a94(s32 id);
void Func_020024f8(struct ObjectRuntime *object, s32 priority);
void Func_02002554(s32 *position, s32 base);

/* The accessors step the work pointer by whole entries and then take entry 0: the bytes add the base to index * 20 before the field offset. */
#define VENT_AT(work, index) ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

/* FAKEMATCH: the callers set up the arguments as for a call whose value is used (flag before index); the setter is declared void for the modules that match with that. */
#define Vent_SetActive(index, active) ((s32 (*)(s32, s32))Func_0200271c)(index, active)

static __inline__ void Blend_SetAlpha(s32 value)
{
    REG_BLDALPHA->value = value;
}

void Func_02002084(s32 fade)
{
    Func_02002828();
    if (fade) {
        Blend_SetAlpha(0xe06);
        WaitFrames(10);
        Blend_SetAlpha(0xd04);
        WaitFrames(10);
        Blend_SetAlpha(0xd02);
        WaitFrames(10);
        Blend_SetAlpha(0xc01);
        WaitFrames(10);
        Blend_SetAlpha(0xc00);
        WaitFrames(10);
    }
    Func_02001ff0_(100, 50);
    Blend_SetAlpha(0xc07);
    if (gPartyState.entrance == 6)
        Scheduler_AddOrUpdateCallback(Func_02001eb8, 0x480);
}
