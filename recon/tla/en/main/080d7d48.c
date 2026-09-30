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
