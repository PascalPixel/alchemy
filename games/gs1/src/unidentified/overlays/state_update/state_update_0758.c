#include "types.h"

extern s32 Data_0200bb68;
extern s32 Data_0200bb00;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb70;
extern s32 Data_0200bac0[];

void Func_02001084(void) {
    s32 *slotState;
    u32 slotIndex;

    Data_0200bb68 = 63;
    Data_0200bb00 = 0;
    Data_0200bb6c = 0;
    Data_0200bb70 = 120;
    slotState = Data_0200bac0;
    for (slotIndex = 0; slotIndex < 16; slotIndex++) {
        *slotState++ = 0;
    }
}
