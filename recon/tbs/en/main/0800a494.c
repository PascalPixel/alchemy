/* 0800a494: ARM, compiled C (push starts at r5: -fcall-used-r4). Score in ARM mode: listing 307, draft 398, differ 633 */
#include "TYPES.H"
#include "SCRIPT_MOTION.H"
#include "IWRAM_CALL.H"

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern struct ScriptMotionObject *gObjectSlots;
extern ScriptCommandFn ScriptObject_CommandTable[];

s32 ArcTan2(s32, s32);

static __inline__ s32 MulQ16(s32 a, s32 b)
{
    return (s32)(((s64)a * b) >> 16);
}

void Object_UpdateAll(void)
{
    struct ScriptMotionObject *obj;
    struct ScriptMotionObject *next;
    s32 i;
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dz;
    s32 vx;
    s32 len;
    s32 rem;
    s32 reached;
    u32 cmd;

    i = 0;
    obj = gObjectSlots;
loop:
    i++;
    next = obj + 1;
    {
        if (obj->script == NULL)
            goto skip;
        if (obj->hook != NULL)
            obj->hook(obj);
        if (obj->script == NULL)
            goto skip;
        if (obj->paused != 0)
            goto skip;

        if (obj->wait != 0) {
            obj->wait--;
        } else {
            for (;;) {
                cmd = (u32)obj->script->word[(s16)obj->script_cursor];
                if (cmd > 63) {
                    obj->script_cursor++;
                    continue;
                }
                if (ScriptObject_CommandTable[cmd](obj) == 0)
                    break;
            }
            if (obj->script == NULL)
                goto skip;
        }

        x = obj->x;
        y = obj->y;
        z = obj->z;

        if (obj->frozen == 0) {
            reached = 0;
            if (obj->target_x != TARGET_UNSET) {
                dx = (obj->target_x - x) / 65536;
                dz = (obj->target_z - z) / 65536;
                len = Iwram_Sqrt(dx * dx + dz * dz) << 16;
                if (len < 0x01000000) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    len = Iwram_Sqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                }
                if (len == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    len = Iwram_RatioMulQ14(len, obj->acceleration);
                    vx = obj->velocity_x += MulQ16(len, dx);
                    dz = obj->velocity_z += MulQ16(len, dz);
                    len = Iwram_Sqrt(MulQ16(vx, vx) + MulQ16(dz, dz)) << 8;
                    if (len > obj->speed_limit) {
                        len = Iwram_RatioMulQ14(len, obj->speed_limit);
                        obj->velocity_x = MulQ16(len, vx);
                        obj->velocity_z = MulQ16(len, dz);
                    }
                }
            } else {
                dx = obj->velocity_x;
                dz = obj->velocity_z;
                len = Iwram_Sqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                if (len != 0) {
                    rem = len - obj->acceleration;
                    if (rem < 0) {
                        obj->velocity_x = reached;
                        obj->velocity_z = reached;
                    } else {
                        len = Iwram_RatioMulQ14(len, rem);
                        obj->velocity_x = MulQ16(len, dx);
                        obj->velocity_z = MulQ16(len, dz);
                    }
                } else {
                    obj->velocity_x = reached;
                    obj->velocity_z = reached;
                }
            }

            if ((obj->motion_flags & 2) != 0) {
                if (y > obj->terrain_height) {
                    obj->velocity_y -= obj->gravity;
                } else if (obj->velocity_y < 0) {
                    y = obj->terrain_height;
                    obj->velocity_y = -MulQ16(obj->vertical.bounce, obj->velocity_y);
                    if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
                        obj->velocity_y = 0;
                }
            }
        }

        x += obj->velocity_x;
        y += obj->velocity_y;
        z += obj->velocity_z;

        switch (obj->arrival_axis) {
        case 16:
            if (x == obj->target_x ||
                ((obj->x - obj->target_x) ^ (x - obj->target_x)) < 0)
                reached = 1;
            break;
        case 17:
            if (y == obj->target_y ||
                ((obj->y - obj->target_y) ^ (y - obj->target_y)) < 0)
                reached = 1;
            break;
        case 18:
            if (z == obj->target_z ||
                ((obj->z - obj->target_z) ^ (z - obj->target_z)) < 0)
                reached = 1;
            break;
        }

        if (reached != 0) {
            if (obj->snap_to_target != 0) {
                x = obj->target_x;
                obj->velocity_x = 0;
                z = obj->target_z;
                obj->velocity_z = 0;
                if (obj->motion_flags == 0) {
                    y = obj->target_y;
                    obj->velocity_y = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            obj->arrival_axis = 0;
        }

        obj->x = x;
        obj->y = y;
        obj->z = z;

        if ((obj->steering_flags & 1) != 0) {
            x = obj->velocity_x;
            z = obj->velocity_z;
            if (z != 0 || x != 0) {
                dx = (s16)((u16)ArcTan2(z, x) - obj->facing);
                if (dx >= 0x1000)
                    dx = 0x1000;
                if (dx < -0x1000)
                    dx = -0x1000;
                obj->facing += dx;
            }
        }
    }
skip:
    obj = next;
    if (i <= 7)
        goto loop;
}
