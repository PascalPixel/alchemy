/* Draft, differ 47 of 32: the native keeps the counter address in r5 only, decrements through r2 and r1, and builds the six arguments in argument registers directly; this form saves r6 and schedules the arithmetic differently. */
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

void Func_02000ca0(void)
{
    s32 timer = --Data_0200345c;

    Func_02002838_(47 - ((timer & 1) << 1), 64 - timer / 8, 2, timer / 8, 29, 15 - timer / 8);
    if (Data_0200345c == 0)
        Scheduler_RemoveCallbackFar(Func_02000ca0);
}
