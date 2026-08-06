#include "types.h"
/* Two-placement scripted camera owner at 0x0200040c, 92 bytes. The preceding
 * eight ranked bytes belong to the prior pool and are deliberately unclaimed. */
  
  
 
extern u8 * Func_02001624();
extern void Func_020016e6();
extern void Func_020015dc();
extern void Func_020016f8();
extern void Func_02001704();
extern void Func_02001620();
extern void Func_020015fe();
extern void Func_020016b8();
extern void Func_02000908(void);
extern void Func_02001678();
void Func_0200040c(void)
{
    u8 *record;
    s32 permuted_2;
    permuted_2 = Func_02001624(); record[0x55] = 0;
    Func_020016e6(); Func_020015dc(-1, -1, -1, 0); Func_020016f8(1);
    Func_02001704(0x00a40000, 0x00400000, 0x01410000, 0);
    record  = permuted_2;
    Func_02001620(); Func_020015fe(1); Func_020016b8(0, 0, 0);
    Func_02000908(); Func_02001678();
}
