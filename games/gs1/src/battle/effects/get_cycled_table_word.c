#include "types.h"

#define BattleEffect_GetCycledTableWord Func_08097b54

extern u32 Data_03001ae8;
extern u16 Data_0809f0f8[];

u16 BattleEffect_GetCycledTableWord(void) {
    return Data_0809f0f8[(Data_03001ae8 >> 4) & 15];
}
