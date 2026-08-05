#include "types.h"






/*
 * Complete 52-byte placement-query wrapper.  This is the exact reviewed
 * control-flow homolog of resource_392:0x020009f8, with this overlay's local
 * producer and consumer independently resolved at 0x02000758/0x020008ec.
 */
extern void Func_02004056(void);
extern s32 Func_02001caa(s32 result[6]);
extern void Func_02001e56(s32, s32, s32, s32, s32, s32);
extern void Func_02004082(void);
void Func_02001544(void)
{
    s32 result[6];

    Func_02004056();
    if (Func_02001caa(result) != 0)
        Func_02001e56(result[0], result[1], result[2], result[3],
                      result[4], result[5]);
    Func_02004082();
}
