#include "fixed_math.h"
#include "types.h"

#define BattleEffect_SetRandomTableValueOnObject Func_0809ad70

extern s8 Data_0809f160[];

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Func_08009240(s32, s8);

void BattleEffect_SetRandomTableValueOnObject(s32 arg0) {
    s8 *table = Data_0809f160;
    s32 index = Rand();
    Func_08009240(arg0, table[(u32)(index * 8) >> 16]);
}
