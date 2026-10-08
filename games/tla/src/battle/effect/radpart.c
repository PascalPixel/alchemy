#include "types.h"
#include "motobj.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 Data_080f0bfc[];

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *tick = &gFrameTick;
    register u32 *table asm("r1") = Data_080f0bfc; /* FAKEMATCH: ⚓️ keeps the table in r1 */
    u32 index;
    u32 value;

    asm volatile("" : "+l"(table), "+l"(tick)); /* FAKEMATCH: ⚓️ loads both addresses before the tick */
    index = (*tick >> 2) & 1;
    value = index[table];
    ((struct MotionObject *)object)->scale_x = value;
    ((struct MotionObject *)object)->scale_y = value;
}

void BattleFx_SpinUpAngle(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}
