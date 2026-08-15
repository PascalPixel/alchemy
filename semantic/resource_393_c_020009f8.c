/*
 * Placement query followed by the tile-(10,12) scene transition.
 *
 * The six-word result is one aggregate, including the two-word tail forwarded
 * by value to Func_02000608.  This is the same source shape witnessed by the
 * resource_392 query wrapper.  The inline six-argument draw wrapper is also
 * witnessed by this overlay's exact 0x02000bf8 sibling: it preserves the ROM's
 * r2-before-r3 stacked-literal order.  Keeping zero live across that draw lets
 * the compiler reuse the dead r5 result pointer for the following stack slot.
 * Together these natural source lifetimes reproduce all 180 bytes.
 */
#include "types.h"
void Func_0808a018(void); void Func_0808a020(void);
struct PlacementTail {
    s32 fifth;
    void (*callback)(void);
};

struct PlacementResult {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
    struct PlacementTail tail;
};

s32 Func_02000474(struct PlacementResult *result);
void Func_02000608(struct PlacementResult result);
void Func_0808a100(s32,s32); void Func_0808a0e0(s32,s32,s32);
void Func_0808a010(s32); void Func_080f9010(s32);
u8 *Func_0808a080(s32); void Func_080091c0(s32,s32,s32,s32,s32,s32);
s32 Func_02000244(s32,s32,s32,s32,s32,s32);
void Func_080770c8(s32); void Func_080091e0(u8 *,s32);

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    Func_080091c0(left, top, width, height, tile, palette);
}

void Func_020009f8(void)
{
    struct PlacementResult result;
    Func_0808a018();
    if (Func_02000474(&result)) {
        Func_02000608(result);
        if (result.second == 10 && (result.third >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Func_0808a100(10, 3);
            Func_0808a0e0(10, -18, 6);
            Func_0808a010(30);
            Func_080f9010(240);
            Func_0808a100(10, 8);
            Func_0808a080(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            Func_02000244(2, 12, 16, 1, 4, zero);
            Func_080770c8(0x201);
            actor = Func_0808a080(10);
            Func_080091e0(actor, 0);
        }
    }
    Func_0808a020();
}
