/* Trial 2026-10-04: this draft compiles to the listing exactly (no halfword
   differs outside relocated words) with the TLA options plus -mtune=arm9tdmi,
   which turns on GCC's Thumb load scheduling; with the approved options the
   scheduler places one load or its first use later. Evidence for a K3
   question, not a routing: the build never uses that option.
   2026-10-05, with -mtune=arm9tdmi landed: exact (differ 0), and ☀️'s
   EFFECT33.C body is exact too. 080d7d48 to 080d85b8 and
   MOTION_MOVE_HALFWAY_TOWARD_TARGET_COPY.C after them are ☀️'s
   EFFECT33.C; per function ☀️'s source scores 4, 0 (this one), 4, 16, 32,
   4, 12, 6, 46, 6 (gPartyState.current_owner for gGameState.selected_actor).
   Its name is taken by ⚓️'s second copy at 080e0c84
   (ADVANCE_ANGLE_WITH_RAMP.C), so it waits for 080d7d48 to start ⚓️'s
   EFFECT33.C and a name for one of the two. */
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
