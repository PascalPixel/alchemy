#include "TYPES.H"

/* Object updates of the phased radial particle sequence. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u32 Data_03001800;
extern u32 Data_0809f0a4[];

void Object_Destroy();

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *table = Data_0809f0a4;
    u32 index = (Data_03001800 >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}

void BattleFx_AdvanceObjectField6WithRamp(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFE40;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFE40);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}
