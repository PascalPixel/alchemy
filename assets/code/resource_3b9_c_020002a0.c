#include "types.h"

extern void Func_02002e98(void);
extern s32 Func_02002e66(s32 flagId);
extern void Func_02002f48(s32 dialogueId);
extern void Func_02002f60(s32 id, s32 arg1);
extern void Func_02002f58(s32 dialogueId);
extern void Func_02002f80(s32 id, s32 arg1);
extern void Func_02002ecc(void);

void Func_020002a0(void)
{
    Func_02002e98();
    if (Func_02002e66(0x962)) {
        Func_02002f48(0x2251);
        Func_02002f60(10, 0);
    } else {
        Func_02002f58(0x2057);
        Func_02002f80(10, 0);
    }
    Func_02002ecc();
}
