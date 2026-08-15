/* Placement query followed by the tile-(10,20) scene transition. */
#include "types.h"
void Func_0808a018(void); void Func_0808a020(void);
struct PlacementResult_02000a68 {
    s32 words[6];
};
s32 Func_02000474(struct PlacementResult_02000a68 *result);
void Func_02000608(struct PlacementResult_02000a68 result);
void Func_0808a100(s32,s32); void Func_0808a0e0(s32,s32,s32);
void Func_0808a010(s32); void Func_080f9010(s32);
u8 *Func_0808a080(s32); void Func_080091c0();
s32 Func_02000244();
void Func_080770c8(s32); void Func_080091e0(u8 *,s32);
void Func_02000a68(void)
{
    struct PlacementResult_02000a68 result;
    Func_0808a018();

    if (Func_02000474(&result)) {
        Func_02000608(result);
        if (result.words[1] == 10 && (result.words[2] >> 20) == 20) {
            u8 *actor;
            s32 zero;

            Func_0808a100(10, 3);
            Func_0808a0e0(10, -18, 6);
            Func_0808a010(30);
            Func_080f9010(240);
            Func_0808a100(10, 8);
            Func_0808a080(10)[35] = 2;
            Func_080091c0(0, 17, 2, 4, 19, 17);
            zero = 0;
            Func_02000244(2, 20, 17, 1, 4, zero);
            Func_080770c8(0x200);
            actor = Func_0808a080(10);
            Func_080091e0(actor, 0);
        }
    }

    Func_0808a020();
}
