#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"

s32 FixedSqrt(s32 value);

/* Object_UpdateAll is ARM code that runs from a heap copy of itself. */
extern u8 Object_UpdateAll[];
extern u8 Object_UpdateAllCodeSize[];

extern u8 gObjectSlots[];
#define TARGET_UNSET ((s32)0x80000000)

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

/*
 * Aims an object at a point. A point closer than one unit is taken at once;
 * otherwise, unless the object moves without easing, the target is pulled
 * in to where the object must start braking at its speed and acceleration.
 * The dominant axis of the move is kept at +0x56 (16 x, 17 y, 18 z).
 */
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    s32 dx;
    s32 dist;
    s32 dz;
    s32 total;
    u8 *axis;

    dx = (x - object->x) / 0x10000;
    dist = (y - object->y) / 0x10000;
    dz = (z - object->z) / 0x10000;
    total = dz * dz;
    dist = Iwram_Sqrt(dx * dx + dist * dist + total) << 16;
    if (dist < 0x100000) {
        dx = x - object->x;
        dist = y - object->y;
        dz = z - object->z;
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) + Iwram_MulQ16(dz, dz));
    }
    if (dist < 0x10000) {
        object->x = x;
        object->y = y;
        object->z = z;
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        return;
    }
    if (object->unknown_56[2] == 0) {
        s32 brake;

        brake = Iwram_RatioMulQ14(object->acceleration,
                             Iwram_MulQ16(object->speed_limit, object->speed_limit));
        if (dist > brake)
            brake = dist - brake / 2;
        else
            brake = dist / 2;
        dist = Iwram_RatioMulQ14(dist, brake);
        x = object->x + Iwram_MulQ16(x - object->x, dist);
        y = object->y + Iwram_MulQ16(y - object->y, dist);
        z = object->z + Iwram_MulQ16(z - object->z, dist);
    }
    object->target_x = x;
    object->target_y = y;
    object->target_z = z;
    dx = x - object->x;
    dist = y - object->y;
    dz = z - object->z;
    axis = &object->unknown_56[0];
    *axis = 16;
    if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
        *axis = 18;
        dx = dz;
    }
    if (object->flags == 0) {
        if (dx < 0)
            dx = -dx;
        if (dx < (dist < 0 ? -dist : dist))
            *axis = 17;
    }
}

void Object_RunUpdateAllFromHeap(void)
{
    void (*routine)(void);

    routine = (void (*)(void))Runtime_BumpAllocate((s32)Object_UpdateAllCodeSize);
    Dma_Set(Object_UpdateAll, routine, 0x84000000 | ((u32)Object_UpdateAllCodeSize >> 2),
        (volatile u32 *)0x040000d4);
    routine();
    Sys_Free(routine);
}

/* Moves every active object by its velocity each frame: position, then
   gravity and the bounce off the ground, through the IWRAM Q16 multiply. */

/* Moves each of the fourteen entries of the object table toward its target,
   applies gravity with bounce, notices when an axis target was passed and
   turns the entry to face its motion. */
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

    obj = *(struct MotionObject **)gObjectSlots;
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
                dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
                if (dist <= 0xffffff) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                }
                if (dist == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
                    vx = obj->vx + Iwram_MulQ16(dx, ratio);
                    obj->vx = vx;
                    dx = obj->vz + Iwram_MulQ16(dz, ratio);
                    obj->vz = dx;
                    dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
                    if (dist > obj->speed_limit) {
                        ratio = Iwram_RatioMulQ14(dist, obj->speed_limit);
                        obj->vx = Iwram_MulQ16(vx, ratio);
                        obj->vz = Iwram_MulQ16(dx, ratio);
                    }
                }
            } else {
                dx = obj->vx;
                dz = obj->vz;
                if ((dx | dz) != 0) {
                    dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                    if (dist != 0) {
                        if (dist - obj->acceleration < 0) {
                            obj->vx = 0;
                            obj->vz = 0;
                        } else {
                            ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
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
