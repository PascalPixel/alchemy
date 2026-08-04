/* Placement query followed by the tile-(10,12) scene transition. */
#include "types.h"
void Func_0808a018(void); void Func_0808a020(void);
s32 Func_02000474(s32 *result);
void Func_02000608(s32,s32,s32,s32,s32,void (*)(void));
void Func_0808a100(s32,s32); void Func_0808a0e0(s32,s32,s32);
void Func_0808a010(s32); void Func_080f9010(s32);
u8 *Func_0808a080(s32); void Func_080091c0(s32,s32,s32,s32,s32,s32);
s32 Func_02000244(s32,s32,s32,s32,s32,s32);
void Func_080770c8(s32); void Func_080091e0(u8 *,s32);
void Func_020009f8(void)
{
    s32 result[6];
    Func_0808a018();
    if (Func_02000474(result)) {
        Func_02000608(result[0], result[1], result[2], result[3], result[4],
                      (void (*)(void))result[5]);
        if (result[1] == 10 && (result[2] >> 20) == 12) {
            u8 *actor;
            Func_0808a100(10, 3);
            Func_0808a0e0(10, -18, 6);
            Func_0808a010(30);
            Func_080f9010(240);
            Func_0808a100(10, 8);
            actor = Func_0808a080(10);
            actor[35] = 2;
            Func_080091c0(32, 20, 2, 4, 11, 16);
            Func_02000244(2, 12, 16, 1, 4, 0);
            Func_080770c8(0x201);
            actor = Func_0808a080(10);
            Func_080091e0(actor, 0);
        }
    }
    Func_0808a020();
}
