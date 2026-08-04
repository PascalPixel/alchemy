#include "types.h"

extern u16 Data_02000240[];
extern u8 Value_00000038;
extern u8 Value_0000003a;
extern u8 Value_0000003c;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000039;

void Func_0808b2b0(s32 arg0) {
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u16) (u32) &Value_00000038;
        break;
    case 1:
        value = (u16) (u32) &Value_0000003a;
        break;
    case 2:
        value = (u16) (u32) &Value_0000003c;
        break;
    case 3:
    case 6:
        value = (u16) (u32) &Value_00000036;
        break;
    case 4:
    case 5:
        value = (u16) (u32) &Value_00000037;
        break;
    default:
        value = (u16) (u32) &Value_00000039;
        break;
    }
    Data_02000240[235] = value;
}
