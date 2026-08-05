#include "types.h"

/*
 * Resource 371 owner at 0x02000598 (284 bytes, 4 distinct call targets).
 *
 * Complete owner: the two-stage prologue at 0x02000598 (`push {r5, r6, r7,
 * lr}`, then r9/sl/fp copied into r5/r6/r7 and pushed, then r8 copied into r7
 * and pushed, then `sub sp, #8`) and its exact mirror image at 0x0200068a
 * ending in `pop {r0} ; bx r0`.  r0 holds the popped return address, so the
 * owner returns nothing.  Bytes 0x0200069c-0x020006b3 are the literal pool
 * (0x02000240, 0x03001ebc, 0xff600000, 0xfed40000, 0x03001f54, 0x163).
 *
 * A per-frame proximity sweep over actors 8..65 (`cmp r6, #65 ; bls`, so the
 * bound is unsigned and inclusive).  Each actor is first tested against an
 * axis-aligned box around the reference object taken from the workspace at
 * +480, using the two negative pool constants as the low corners
 * (0xff600000 = -0xa00000 in X, 0xfed40000 = -0x12c0000 in Z) and 160 << 16 /
 * 200 << 16 as the high ones.  In-box actors get their +0x54 flag set to 1,
 * others cleared to 0.
 *
 * The second test is a Manhattan-distance touch: |dx| + |dz| against the sum
 * of the two objects' radius products, where a radius product is
 * `*(u16 *)(object + 0x20) * *(s32 *)(*(void **)(object + 0x50) + 0x18)`.
 * The subject's product is computed once before the loop and kept in the
 * first of the two stack slots; the box's X low corner is kept in the second.
 * On a touch that is not vetoed by 0x020048fe, the workspace halfword at
 * +364 is set to the actor index plus 100.
 *
 * The subject is looked up through `Data_02000240[250]` read as a 32-bit
 * handle, which is exactly what the byte-exact sibling
 * `assets/code/resource_371_c_020004a0.c` does with the same element; the
 * workspace pointer 0x03001ebc and the `Data_02000240` spelling come from
 * `assets/code/resource_371_c_02000030.c`.
 *
 * UNCERTAINTY: the original uses r4 as scratch for |dx| without saving it —
 * the same call-clobbered-r4 convention seen at 0x0200011c, 0x020001c4 and
 * 0x02002768.  Nothing observable depends on it.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern u8 Data_03001f54;

u8 *Func_0808a080();
u8 *Func_0808a400();
s32 Func_080770c0();

static s32 RadiusProduct_02000598(u8 *object)
{
    return (s32)*(u16 *)(object + 0x20) *
           *(s32 *)(*(u8 **)(object + 0x50) + 0x18);
}

void Func_02000598(void)
{
    s16 *table = Data_02000240;
    u8 *subject = Func_0808a080(*(s32 *)&table[250]);
    u8 *workspace = Data_03001ebc;
    s32 subject_radius = RadiusProduct_02000598(subject);
    u8 *reference = *(u8 **)(workspace + 480);
    s32 x_high = *(s32 *)(reference + 0x08) + 0xa00000;
    s32 x_low = *(s32 *)(reference + 0x08) - 0xa00000;
    s32 z_low = *(s32 *)(reference + 0x10) - 0x12c0000;
    s32 z_high = *(s32 *)(reference + 0x10) + 0xc80000;
    u32 index;

    for (index = 8; index <= 65; index++) {
        u8 *actor = Func_0808a400(index);
        s32 x;
        s32 z;
        s32 dx;
        s32 dz;

        if (actor == 0) {
            continue;
        }

        x = *(s32 *)(actor + 0x08);
        z = *(s32 *)(actor + 0x10);
        actor[0x54] = 1;
        if (x < x_low || x > x_high || z < z_low || z > z_high) {
            actor[0x54] = 0;
            continue;
        }

        if (Data_03001f54 != 0 && Func_080770c0(0x163) != 0) {
            continue;
        }

        if (dx < 0) {
            dx = *(s32 *)(subject + 0x08) - x;
        }
        dz = z - *(s32 *)(subject + 0x10);
        if (dz < 0) {
            dz = *(s32 *)(subject + 0x10) - z;
        }
        dx = x - *(s32 *)(subject + 0x08);
        if (dx + dz >= subject_radius + RadiusProduct_02000598(actor)) {
            continue;
        }

        if (Func_080770c0(260) != 0) {
            continue;
        }
        *(u16 *)(workspace + 364) = (u16)(index + 100);
    }
}
