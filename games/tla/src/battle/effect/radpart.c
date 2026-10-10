#include "types.h"
#include "motobj.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 gFrameTick;
extern u32 Data_080f0bfc[];

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *tick = &gFrameTick;
    u32 *table;
    u32 index;
    u32 value;

    asm volatile("" : "+l"(tick)); /* FAKEMATCH: load the tick address before the table. */
    table = Data_080f0bfc;
    /* FAKEMATCH: keep the tick extraction in its own block; without this
     * boundary the table address loads after the tick (two reordered rows). */
    do {
        index = (*tick >> 2) & 1;
    } while (0);
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
