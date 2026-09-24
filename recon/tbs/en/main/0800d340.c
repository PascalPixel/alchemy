/* Draft (2026-09-24): exact (788 bytes, 0 differ) once the reviewed
   Iwram_Call2 in IWRAM_CALL.H lists r1 as clobbered and ends its template
   in "\n\t"; with the header as reviewed it is 4 bytes short. Both changes
   keep TBS and TLA byte-identical (checked with make compare-all), but the
   body is pinned by IWRAM_CALL_BODY_SHA256 in compiler/no_asm.rs, so it
   waits for that review. Why: the ROM sets r1 again before the second of
   each pair of calls sharing a factor, which GCC only does when the asm
   clobbers r1, and the extra template line is what places the literal
   pool mid-function. The proposed body:

       __asm__ volatile(
           ".align 2\n\t"
           "mov ip, pc\n\t"
           "bx %2\n\t"
           : "+r"(result)
           : "r"(factor), "r"(routine)
           : "r1", "r2", "ip", "cc");

   Other facts that closed it: load x, y, z and add the velocities in that
   order; the bounce is obj->vy = -Iwram_MulQ16(...) then an ABS test of
   obj->vy. */

#include "TYPES.H"
#include "IWRAM_CALL.H"

/* Moves each of the fourteen entries of the object table toward its target,
   applies gravity with bounce, notices when an axis target was passed and
   turns the entry to face its motion. */

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
        x = obj->x;
        y = obj->y;
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
                    obj->vy = -Iwram_MulQ16(obj->vy, obj->bounce);
                    if ((obj->vy < 0 ? -obj->vy : obj->vy) <= obj->gravity)
                        obj->vy = 0;
                }
            }
        }
        x += obj->vx;
        y += obj->vy;
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
