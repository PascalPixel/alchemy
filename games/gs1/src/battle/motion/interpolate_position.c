#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 Data_03001cd4;

s32 FixedPoint_Ratio(s32, s32);
void Object_SetPosition(void *, s32, s32, s32);
void Object_SetMode(s32, s32);

void BattleMotion_InterpolatePosition(void **start_slot, void **end_slot, s32 progress) {
    s32 z_step;
    s32 start_x;
    s32 x;
    s32 start_z;
    s32 end_z;
    void *start;
    void *end;

    start = *start_slot;
    end = *end_slot;
    start_x = FIELD_AT_OFFSET(start, s32, 8);
    x = start_x + FixedPoint_Ratio(progress * (FIELD_AT_OFFSET(end, s32, 8) - start_x), 0x64);
    end_z = FIELD_AT_OFFSET(end, s32, 0x10);
    start_z = FIELD_AT_OFFSET(start, s32, 0x10);
    z_step = FixedPoint_Ratio(progress * (end_z - start_z), 0x64);
    *(s16 *)0x04000050 = 0;
    FIELD_AT_OFFSET(start, s32, 0x34) = 0x20000;
    FIELD_AT_OFFSET(start, s32, 0x30) = 0x80000;
    FIELD_AT_OFFSET(start, s32, 0x28) = 0x40000;
    FIELD_AT_OFFSET(start, s32, 0x48) = 0xAB85;
    FIELD_AT_OFFSET(start, s32, 0x44) = 0;
    FIELD_AT_OFFSET(start, s8, 0x5A) = 1;
    Object_SetPosition(start, x, 0, start_z + z_step);
    Object_SetMode((s32) start, 2);
}
