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

/* Raw overlay relocation spellings.  They are call-site evidence, so the two
 * logical actor-access calls deliberately use different names. */
void Func_02001872(void);
s32 Func_02000e7a(struct PlacementResult *result);
void Func_02001026(struct PlacementResult result);
void Func_020018e0(s32,s32); void Func_020018dc(s32,s32,s32);
void Func_020018b2(s32); u8 *Func_02001918();
void Func_02001900(s32,s32); u8 *Func_020018de(s32);
void Func_020018b8(s32,s32,s32,s32,s32,s32);
s32 Func_02000ccc(s32,s32,s32,s32,s32,s32);
void Func_020018f2(s32); void Func_020018ee(u8 *,s32);
void Func_0200191a(void);

static __inline__ void DrawPlacement(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    Func_020018b8(left, top, width, height, tile, palette);
}

void Func_020009f8(void)
{
    struct PlacementResult result;
    Func_02001872();
    if (Func_02000e7a(&result)) {
        Func_02001026(result);
        if (result.second == 10 && (result.third >> 20) == 12) {
            u8 *actor;
            s32 zero;
            Func_020018e0(10, 3);
            Func_020018dc(10, -18, 6);
            Func_020018b2(30);
            Func_02001918(240);
            Func_02001900(10, 8);
            Func_020018de(10)[35] = 2;
            zero = 0;
            DrawPlacement(32, 20, 2, 4, 11, 16);
            Func_02000ccc(2, 12, 16, 1, 4, zero);
            Func_020018f2(0x201);
            actor = Func_02001918(10);
            Func_020018ee(actor, 0);
        }
    }
    Func_0200191a();
}
