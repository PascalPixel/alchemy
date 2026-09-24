/* Draft, not exact (2026-09-24): 784-byte candidate for the 788-byte owner.
   Control flow, stores, calls and the stack frame agree. Residual: the
   reference reloads r1 (adds r1, r4) before the second of each pair of
   Iwram_MulQ16 calls sharing one ratio; IWRAM_CALL.H declares r1 an input
   the asm leaves intact, so GCC reuses it and the three missing moves
   shift the alignment nops and the mid-function pool. Declaring the factor
   "+r" restores the reloads but costs the register allocation (320 differ),
   so the header was left alone. Also one scheduling pair at entry (y is
   loaded into r3 here, r4 there) and the bounce magnitude lives in r0
   instead of r1. */

#include "TYPES.H"
#include "IWRAM_CALL.H"

/* Moves each of the fourteen entries of the object table toward its target,
   applies gravity with bounce, notices when an axis target was passed and
   turns the entry to face its motion. */

#define Object_UpdateAllMotion Func_0800d340

#define TARGET_UNSET ((s32)0x80000000)

#define IwramLength ((s32 (*)(s32))0x030001d8)
#define IwramRatio ((s32 (*)(s32, s32))0x0300013c)

struct MotionObject {
    s32 active;          /* 0x00 */
    u16 unk_04;
    u16 angle;           /* 0x06 */
    s32 x;               /* 0x08 */
    s32 y;               /* 0x0c */
    s32 z;               /* 0x10 */
    s32 floor;           /* 0x14 */
    u8 unk_18[0x0c];
    s32 vx;              /* 0x24 */
    s32 vy;              /* 0x28 */
    s32 vz;              /* 0x2c */
    s32 speed_limit;     /* 0x30 */
    s32 acceleration;    /* 0x34 */
    s32 target_x;        /* 0x38 */
    s32 target_y;        /* 0x3c */
    s32 target_z;        /* 0x40 */
    s32 bounce;          /* 0x44 */
    s32 gravity;         /* 0x48 */
    u8 unk_4c[0x09];
    u8 flags;            /* 0x55 */
    u8 arrive_axis;      /* 0x56 */
    u8 unk_57;
    u8 snap;             /* 0x58 */
    u8 unk_59;
    u8 turn;             /* 0x5a */
    u8 unk_5b[0x06];
    u8 frozen;           /* 0x61 */
    u8 unk_62[0x0e];
};

s32 ArcTan2(s32, s32);

void Object_UpdateAllMotion(void)
{
    struct MotionObject *obj;
    s32 cnt;
    s32 y;
    s32 arrived;
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;
    s32 vx;
    s32 dist;
    s32 ratio;
    s32 vy;
    s32 turn;

    obj = *(struct MotionObject **)0x03001e64;
    for (cnt = 13; cnt >= 0; cnt--, obj++) {
        if (obj->active == 0)
            continue;
        y = obj->y;
        x = obj->x;
        z = obj->z;
        if (obj->frozen == 0) {
            arrived = 0;
            if (obj->target_x != TARGET_UNSET) {
                dx = (obj->target_x - x) / 65536;
                dz = (obj->target_z - z) / 65536;
                dist = IwramLength(dx * dx + dz * dz) << 16;
                if (dist <= 0xffffff) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    dist = IwramLength(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                }
                if (dist == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    ratio = IwramRatio(dist, obj->acceleration);
                    vx = obj->vx + Iwram_MulQ16(dx, ratio);
                    obj->vx = vx;
                    dx = obj->vz + Iwram_MulQ16(dz, ratio);
                    obj->vz = dx;
                    dist = IwramLength(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
                    if (dist > obj->speed_limit) {
                        ratio = IwramRatio(dist, obj->speed_limit);
                        obj->vx = Iwram_MulQ16(vx, ratio);
                        obj->vz = Iwram_MulQ16(dx, ratio);
                    }
                }
            } else {
                dx = obj->vx;
                dz = obj->vz;
                if ((dx | dz) != 0) {
                    dist = IwramLength(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                    if (dist != 0) {
                        if (dist - obj->acceleration < 0) {
                            obj->vx = 0;
                            obj->vz = 0;
                        } else {
                            ratio = IwramRatio(dist, dist - obj->acceleration);
                            obj->vx = Iwram_MulQ16(dx, ratio);
                            obj->vz = Iwram_MulQ16(dz, ratio);
                        }
                    } else {
                        obj->vx = 0;
                        obj->vz = 0;
                    }
                }
            }
            if (obj->flags & 2) {
                if (y > obj->floor) {
                    obj->vy -= obj->gravity;
                } else if (obj->vy < 0) {
                    y = obj->floor;
                    vy = Iwram_MulQ16(obj->vy, obj->bounce);
                    obj->vy = -vy;
                    dist = -vy;
                    if (dist < 0)
                        dist = vy;
                    if (dist <= obj->gravity)
                        obj->vy = 0;
                }
            }
        }
        y += obj->vy;
        x += obj->vx;
        z += obj->vz;
        if (obj->arrive_axis != 0) switch (obj->arrive_axis) {
        case 16:
            if (x == obj->target_x || ((obj->x - obj->target_x) ^ (x - obj->target_x)) < 0)
                arrived = 1;
            break;
        case 17:
            if (y == obj->target_y || ((obj->y - obj->target_y) ^ (y - obj->target_y)) < 0)
                arrived = 1;
            break;
        case 18:
            if (z == obj->target_z || ((obj->z - obj->target_z) ^ (z - obj->target_z)) < 0)
                arrived = 1;
            break;
        }
        if (arrived) {
            if (obj->snap) {
                obj->vx = 0;
                obj->vz = 0;
                x = obj->target_x;
                z = obj->target_z;
                if (obj->flags == 0) {
                    y = obj->target_y;
                    obj->vy = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            obj->arrive_axis = 0;
        }
        obj->x = x;
        obj->y = y;
        obj->z = z;
        if (obj->turn & 1) {
            x = obj->vx;
            z = obj->vz;
            if (x != 0 || z != 0) {
                turn = (s16)(ArcTan2(z, x) - obj->angle);
                if (turn > 0x1000)
                    turn = 0x1000;
                if (turn < -0x1000)
                    turn = -0x1000;
                obj->angle += turn;
            }
        }
    }
}
