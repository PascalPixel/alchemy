typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

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
s32 Func_080072f0(s32, s32, s32, s32);
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
            result = Func_080072f0(sqx + sqy + sqz, sqz, sqy, 0x030001D8);

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
