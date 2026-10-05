/*
 * Draft: BattleFx_SetObjectAlternatingWords, the head of ☀️'s EFFECT33.C.
 * differ 4: the native loads the scale table address before the tick and
 * builds the 1 mask after it; this build does the reverse. Trial
 * 2026-10-05: one-expression index and index-then-table orders both 6.
 * Links as recon/tla/raw/080d7d48.s.
 */
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"

/* Object updates of the phased radial particle sequence. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 BattleFx_PulseScales[];

void Object_Destroy();

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *table = BattleFx_PulseScales;
    u32 index = (gFrameTick >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}
