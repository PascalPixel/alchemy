#include "types.h"
  
  
extern u8 * Func_02002ba2(int);
extern void Func_02002bba(int,int,int);
extern void Func_02002b40(u8 *,int);
extern void Func_02002bea(int,int,int);
extern void Func_02002c1c(int,int,int);
extern void Func_02002b24(int);
extern void Func_02002b7c(u8 *,int);
void Func_020011b0(int actor, int x, int z, int field40)
{
    u8 *record = Func_02002ba2(actor); int frames;
    Func_02002bba(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_02002b40(record, 0);
    Func_02002bea(actor, x, z); Func_02002c1c(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Func_02002b24(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_02002b7c(record, 1); *(s32 *)(record + 72) = 0x10000;
}
