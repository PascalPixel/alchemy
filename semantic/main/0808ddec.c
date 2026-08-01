/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY: the routine at 0x030001d8 is not established.  It RETURNS a
 * value, unlike the other relocated helpers in this audit, and both callers
 * feed it sums of squares and use the result as a distance -- which reads
 * like a magnitude or square root.  That reading is an inference from two
 * call sites and is NOT asserted; the type records the arity and the return,
 * nothing else.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef s32 (*Resident_030001D8)(s32 arg0, s32 arg1, s32 arg2);

struct Entity_0808ddec {
    u8 pad0[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[0x59 - 0x14];
    u8 flags;
};

struct Entity_0808ddec *Func_0808ba1c(u32);
s32 Func_080022ec(s32, s32);
u16 Func_080044d0(s32, s32);

s32 Func_0808ddec(u32 arg0)
{
    struct Entity_0808ddec *self;
    struct Entity_0808ddec *other;
    s32 best;
    s32 found;
    u32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 sqx;
    s32 sqy;
    s32 sqz;
    s32 result;
    u16 angle;
    s16 diff;

    found = -1;
    best = 0x20;
    self = Func_0808ba1c(arg0);
    if (self != 0) {
        i = 0;
        do {
            if (i == arg0)
                goto next;
            other = Func_0808ba1c(i);
            if (other == 0)
                goto next;
            if (other->flags & 8)
                goto next;

            dy = other->y - self->y;
            if (dy >= 0) {
                if (dy > 0x2FFFFF)
                    goto next;
            } else if (self->y - other->y > 0x2FFFFF) {
                goto next;
            }

            dx = other->x - self->x;
            if (dx < 0)
                dx += 0xFFFF;
            dy = other->y - self->y;
            dx >>= 16;
            if (dy < 0)
                dy += 0xFFFF;
            dy >>= 16;
            dz = other->z - self->z;
            if (dz < 0)
                dz += 0xFFFF;
            dz >>= 16;

            sqx = dx * dx;
            sqy = dy * dy;
            sqz = dz * dz;
            result = ((Resident_030001D8)0x030001d8)(sqx + sqy + sqz, sqz, sqy);

            if (other->flags & 4)
                result = Func_080022ec(result * 10, 13);

            if (result >= best)
                goto next;

            angle = Func_080044d0(other->z - self->z, other->x - self->x);
            if (result > 11) {
                diff = angle - self->angle;
                if (diff < -0x2FFF || diff > 0x2FFF)
                    goto next;
            }

            found = i;
            best = result;
next:
            i += 1;
        } while ((s32) i <= 0x42);
    }
    return found;
}
