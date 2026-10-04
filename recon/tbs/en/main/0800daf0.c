/* 2026-10-04, third wave (finisher 3): still 35. A -dp/-dg compile maps
   the stretch to insns 168-197: obj->x reload r2 (168), x reload r4 (170),
   0xffff reload r2 (177), obj reload r4 (183), z reload r2 (185), 0xffff
   r3 (192), sl reload r4 (197). The native has r2, r4, r2, r2, r4, r2, r3:
   from 183 on every choice is the next spill register, consistent with
   the 177 constant reload not advancing last_spill_reg (or r4 being busy
   at 183 only). find_equiv_reg cannot supply the constant there (the bge
   at 173 ends its backward scan), so the native 177 is likely not a plain
   constant reload. Explicit "if (d < 0) d += 0xffff; d >>= 16" (with the
   literal or a variable) scores 216, plain >> 16 78. */
/* 2026-10-04, second wave: alchemy diff 35 (was 120), 225 instructions
   against 224. Greg dumps show why y lost r5: global allocation orders by
   refs*log2(refs)/live length, and y (3 refs over 41 insns) came after the
   four bounds (3 refs over 22-28), so min_z/min_x took r5/r6. One tagged
   extra use of y (the asm below) puts it ahead; y then takes r5, the bounds
   r6/r7 and the 0xff880000 reload r5, all as the listing has them.
   Plain reshapes kept: no tmp for the distance or the y magnitude (the
   product then forms in r0), dz declared before dy, and dz recomputed
   before dy after the square root. Everything outside one stretch matches.
   Left: from the second 0xffff fixup to the square root call, the reload
   registers are one step off the listing's round robin (listing obj r2,
   z r4, 0xffff r2, multiply operand r3; here r4, r2, r3, r4), plus one extra
   obj reload before dy. Reload rotates through spill regs {2,3,4,5}; the
   listing behaves as if the two 0xffff loads did not advance it. Tried
   without effect: "+r"/"=r" forms of the device, statement orders of dx,
   dy, dz (dx,dy,dz and dy,dx,dz score 66). A device search from here
   reached 33 only with scheduling barriers. */
/* 2026-10-04: fresh EN baseline 895; ordinary temporary reuse and step
   assignment improve to 715 (50 register-only, one reordered, four
   inserted instructions), still 500/492 bytes. Two 55-second searches
   evaluated 19,060 candidates without another improvement. Explicit
   bindings of bounds/y to r7/r6/r5 worsened to 4102 and were discarded.
   Existing resident IWRAM entries now use their shared header names. */
/* Draft, not exact. Written from the listing. The clamped x is in fp and z
   in a stack slot as in the ROM, once the whole-pixel z distance reuses dz
   as the x distance reuses dx. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct FollowObject {
    u32 active;
    u16 frame;
    u16 unk_06;
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[0x1c];
    s32 speed_limit;
    u8 unk_34[4];
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unk_44[0x11];
    u8 motion_flags;
    u8 unk_56[0xe];
    s16 snap;
    u8 unk_66[2];
    struct FollowObject *leader;
};

struct FieldBounds {
    u8 unk_00[0xec];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

extern struct FieldBounds *gMapWork;

s32 FixedSqrt(s32 value);

s32 Camera_FollowLeaderInBounds(struct FollowObject *obj)
{
    struct FollowObject *leader;
    struct FieldBounds *bounds;
    s32 min_x;
    s32 min_z;
    s32 max_x;
    s32 max_z;
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    s32 dy;
    s32 dx;
    s32 dist;
    s32 step;
    s32 (*div)(s32, s32);

    bounds = gMapWork;
    leader = obj->leader;
    min_x = bounds->left + 0x780000;
    min_z = bounds->top + leader->y + 0x600000;
    max_x = bounds->right - 0x780000;
    max_z = bounds->bottom + leader->y - 0x400000;
    obj->motion_flags = 0;
    if (leader != NULL && leader->active != 0) {
        x = leader->x;
        y = leader->y;
        /* y takes r5 because one more use lifts it above the lower bounds. */
        /* FAKEMATCH: global allocation order, as the native allocates y. */
        __asm__("" : : "r"(y));
        z = leader->z;
        obj->target_x = 0x80000000;
        obj->target_y = 0x80000000;
        obj->target_z = 0x80000000;
        if (x < min_x)
            x = min_x;
        if (z < min_z)
            z = min_z;
        if (x > max_x)
            x = max_x;
        if (z > max_z)
            z = max_z;
        if (obj->snap != 0) {
            obj->x = x;
            obj->y = y;
            obj->z = z;
        } else {
            dx = (x - obj->x) / 0x10000;
            dz = (z - obj->z) / 0x10000;
            dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
            dx = x - obj->x;
            dz = z - obj->z;
            dy = y - obj->y;
            if (dist < 0x400000)
                dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
            if ((step = dist / 8) > obj->speed_limit)
                step = obj->speed_limit;
            if (dist < 0x4000) {
                obj->x = x;
                obj->z = z;
            } else {
                if (dist > step) {
                    div = Iwram_RatioMulQ14;
                    dx = Iwram_MulQ16(div(dist, dx), step);
                    dz = Iwram_MulQ16(div(dist, dz), step);
                }
                obj->x += dx;
                obj->z += dz;
            }
            if ((dy < 0 ? -dy : dy) > 0x8000)
                dy /= 4;
            obj->y += dy;
        }
    }
    obj->frame++;
    return 1;
}
