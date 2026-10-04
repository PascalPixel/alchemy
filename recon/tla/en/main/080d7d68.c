/* Trial 2026-10-04: this draft compiles to the listing exactly (no halfword
   differs outside relocated words) with the TLA options plus -mtune=arm9tdmi,
   which turns on GCC's Thumb load scheduling; with the approved options the
   scheduler places one load or its first use later. Evidence for a K3
   question, not a routing: the build never uses that option. */
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"

/* Object updates of the phased radial particle sequence. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 BattleFx_PulseScales[];

void Object_Destroy();

void BattleFx_AdvanceObjectField6WithRamp(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}
