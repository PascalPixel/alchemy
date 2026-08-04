#include "types.h"

/*
 * Proximity watch on the party leader.  The leader's object is fetched
 * through its handle at 0x02000240+0x1f4, and its position is compared with
 * the target point held in the statistics block at 0x03001ebc: the x gap is
 * taken against +0xcbc and scaled by 0xd105, the depth gap against +0xcbe,
 * and the two are squared and summed.  A countdown at +0xcba ticks down once
 * per call while the enable halfword at +0xcc0 is set.  When the leader is
 * outside the 3600 unit radius, or the countdown has expired, the marker
 * halfword at +0x17e is set to 0x2090.
 *
 * `ldr r3,[pc,#104]` loads 0x03000118 - the ARM fixed-point multiply
 * relocated into IWRAM - and `mov ip, pc ; bx r3` calls it: mov ip,pc sets
 * the return address to the halfword after the bx, so control resumes in the
 * same frame with every live register intact.  It is an ordinary indirect
 * call, the shape already resolved at 0x0800ebec, and is written below as a
 * call through a function pointer.
 *
 * Reached only as a published Thumb pointer, at 0x080984bc and 0x08098584.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080982dc)(s32 a, s32 b);

/* Statistics block at 0x03001ebc; only this watch's fields are named. */
struct Stats_080982dc {
    u8 unknown_000[0x17e];
    u16 marker_17e;
    u8 unknown_180[0xb3a];
    s16 countdown_cba;
    s16 target_x_cbc;
    s16 target_z_cbe;
    s16 enable_cc0;
};

struct Object_080982dc {
    u8 unknown_00[8];
    s32 x_08;
    s32 y_0c;
    s32 z_10;
};

struct Object_080982dc *Func_0808ba1c(s32 handle);

void Func_080982dc(void)
{
    Multiply_080982dc multiply = (Multiply_080982dc)0x03000118;
    struct Stats_080982dc *stats = *(struct Stats_080982dc **)0x03001ebc;
    struct Object_080982dc *leader =
        Func_0808ba1c(*(s32 *)(0x02000240 + (250 << 1)));
    s32 gap_x;
    s32 gap_z;

    if (stats->enable_cc0 != 0 && stats->countdown_cba != 0) {
        stats->countdown_cba--;
    }

    gap_x = multiply(stats->target_x_cbc - leader->x_08 / 0x10000, 0xd105);
    /* The depth gap is taken against the object's z minus its y, not
       against z alone. */
    gap_z = stats->target_z_cbe - (leader->z_10 - leader->y_0c) / 0x10000;

    if (gap_z * gap_z + gap_x * gap_x >= 225 << 4 ||
        stats->countdown_cba == 0) {
        stats->marker_17e = 0x2090;
    }
}
