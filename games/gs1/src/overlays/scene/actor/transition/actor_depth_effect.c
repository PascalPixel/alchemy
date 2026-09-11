#include "types.h"

#define FieldScene_RunActorTenDepthSequence Func_02000568
void Func_02002982(s32);
void Func_020006fc(s32, s32, s32);
void Func_02000706(s32, s32, s32);
void Func_02000712(s32, s32, s32);
void Func_020029a8(s32);
void Func_020028be(s32);
void Func_02002804(s32);
void Func_020016fc(void);

/* Three sites of one import, so three names. */

void FieldScene_RunActorTenDepthSequence(void)
{
    /* r5 = -96 (movs #0x60 / negs), live across the first import call. */
    s32 depth = -96;

    Func_02002982((s32) 0xF1);
    Func_020006fc(10, 0, depth);
    Func_02000706(10, 0, depth);
    Func_02000712(10, 0, -80);
    Func_020029a8((s32) 0x121);
    Func_020028be((s32) 0x307);
    Func_02002804(2);
    Func_020016fc();
}
