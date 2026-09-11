#include "types.h"
#include "scene.h"

/* battle/effects/set/set_special_from_table.c */
extern s16 gCell[];
extern s32 gRom[];

void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1)
{
    s32 target = gCell[224];
    s32 *table = gRom;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    gCell[235] = (s16)result;
}

/* battle/effects/data/select_result_pointer.c */
extern u16 gCell[];
extern u8 Value_00000038;
extern u8 Value_0000003a;
extern u8 Value_0000003c;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000039;

void BattleFx_SelectResultPointer(s32 arg0)
{
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u16)(u32)&Value_00000038;
        break;
    case 1:
        value = (u16)(u32)&Value_0000003a;
        break;
    case 2:
        value = (u16)(u32)&Value_0000003c;
        break;
    case 3:
    case 6:
        value = (u16)(u32)&Value_00000036;
        break;
    case 4:
    case 5:
        value = (u16)(u32)&Value_00000037;
        break;
    default:
        value = (u16)(u32)&Value_00000039;
        break;
    }
    gCell[235] = value;
}
