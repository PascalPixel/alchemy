#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "global_cells.h"
#include "iwram_call.h"
#include "script_motion.h"
#include "script_object_entry.h"

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Object_IsTargetUnset((struct ObjectRuntime *)arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second != (s32)0x80000000)
            return 0;
        first = object->target_y;
    } else {
        first = object->target_x;
        second = (s32)0x80000000;
    }
    if (first != second || object->target_z != first)
        return 0;
    return 1;
}

#define gObjectSlots (*(struct ScriptMotionObject * *)&gObjectSlots)

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern ScriptCommandFn ScriptObject_CommandTable[];

s32 Map_GetTerrainHeight(s32, s32, s32);
s32 ArcTan2(s32, s32);

static __inline__ s32 Square(s32 value)
{
    return value * value;
}

void Object_UpdateAllThumb(void)
{
    struct ScriptMotionObject *obj;
    const struct ScriptMotionWords *script;
    s32 cnt;
    s32 px;
    s32 py;
    s32 pz;
    s32 ground;
    s32 reached;
    s32 pos[3];
    s32 dx;
    s32 dy;
    s32 dz;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 dist;
    s32 len;
    s32 ratio;
    s32 rem;
    s32 half;
    s32 work;
    u32 cmd;
    u32 phase;

    obj = gObjectSlots;
    for (cnt = 63; cnt >= 0; cnt--, obj++) {
        reached = 0;

        script = obj->script;
        if (script == NULL)
            continue;

        if (obj->hook != NULL) {
            obj->hook(obj);
            script = obj->script;
        }
        if (script == NULL)
            continue;
        if (obj->paused != 0)
            continue;

        if (obj->wait != 0) {
            obj->wait--;
        } else {
            for (;;) {
                cmd = (u32)script->word[(s16)obj->script_cursor];
                if (cmd <= 63) {
                    if (ScriptObject_CommandTable[cmd](obj) == 0)
                        break;
                } else {
                    obj->script_cursor++;
                }
                script = obj->script;
            }
            if (obj->script == NULL)
                continue;
        }

        px = obj->x;
        py = obj->y;
        pz = obj->z;

        if (obj->frozen == 0) {
            if (obj->motion_flags == 0) {
                if (obj->target_x != TARGET_UNSET) {
                    dx = (obj->target_x - px) / 65536;
                    dy = (obj->target_y - py) / 65536;
                    dz = (obj->target_z - pz) / 65536;
                    dist = Iwram_Sqrt(Square(dx) + Square(dy) + Square(dz));
                    if (dist == 0) {
                        px = obj->target_x;
                        py = obj->target_y;
                        pz = obj->target_z;
                    } else {
                        ratio = Iwram_RatioMulQ14(dist << 16, obj->acceleration);
                        obj->velocity_x += dx * ratio;
                        obj->velocity_y += dy * ratio;
                        obj->velocity_z += dz * ratio;
                        vx = obj->velocity_x;
                        vy = obj->velocity_y;
                        vz = obj->velocity_z;
                        len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vy, vy) +
                                        Iwram_MulQ16(vz, vz));
                        if (len > obj->speed_limit) {
                            ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
                            obj->velocity_x = Iwram_MulQ16(vx, ratio);
                            obj->velocity_y = Iwram_MulQ16(vy, ratio);
                            obj->velocity_z = Iwram_MulQ16(vz, ratio);
                        }
                    }
                } else {
                    dx = obj->velocity_x;
                    dy = obj->velocity_y;
                    dz = obj->velocity_z;
                    len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                    Iwram_MulQ16(dz, dz));
                    if (len != 0) {
                        rem = len - obj->acceleration;
                        if (rem < 0)
                            rem = 0;
                        ratio = Iwram_RatioMulQ14(len, rem);
                        obj->velocity_x = Iwram_MulQ16(dx, ratio);
                        obj->velocity_y = Iwram_MulQ16(dy, ratio);
                        obj->velocity_z = Iwram_MulQ16(dz, ratio);
                    } else {
                        obj->velocity_x = 0;
                        obj->velocity_y = 0;
                        obj->velocity_z = 0;
                    }
                }
            } else {
                if (obj->target_x != TARGET_UNSET) {
                    dx = (obj->target_x - px) / 65536;
                    dz = (obj->target_z - pz) / 65536;
                    dist = Iwram_Sqrt(Square(dx) + Square(dz)) << 16;
                    if (dist <= 0x00ffffff) {
                        dx = obj->target_x - px;
                        dz = obj->target_z - pz;
                        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
                    }
                    if (dist == 0) {
                        px = obj->target_x;
                        pz = obj->target_z;
                    } else {
                        ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
                        obj->velocity_x += Iwram_MulQ16(dx, ratio);
                        obj->velocity_z += Iwram_MulQ16(dz, ratio);
                        vx = obj->velocity_x;
                        vz = obj->velocity_z;
                        len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vz, vz));
                        if (len > obj->speed_limit) {
                            ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
                            obj->velocity_x = Iwram_MulQ16(vx, ratio);
                            obj->velocity_z = Iwram_MulQ16(vz, ratio);
                        }
                    }
                } else {
                    dx = obj->velocity_x;
                    dz = obj->velocity_z;
                    len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
                    if (len != 0) {
                        rem = len - obj->acceleration;
                        if (rem < 0)
                            rem = 0;
                        ratio = Iwram_RatioMulQ14(len, rem);
                        obj->velocity_x = Iwram_MulQ16(dx, ratio);
                        obj->velocity_z = Iwram_MulQ16(dz, ratio);
                    } else {
                        obj->velocity_x = 0;
                        obj->velocity_z = 0;
                    }
                }

                if ((obj->motion_flags & 1) != 0) {
                    vx = px + obj->velocity_x;
                    vz = pz + obj->velocity_z;
                    ground = Map_GetTerrainHeight((s32)obj->terrain_id, vx, vz);
                    vy = ground - obj->terrain_height;
                    if (ground - py > -0x40000)
                        py += vy;
                    if (vy < 0)
                        vy = -vy;
                    half = obj->acceleration / 2;
                    if (vy > half)
                        vy = half;
                    vy = vy * 3;
                    if (vy != 0 && (obj->motion_flags & 0x10) == 0) {
                        dx = obj->velocity_x;
                        dy = obj->velocity_y;
                        dz = obj->velocity_z;
                        len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                        Iwram_MulQ16(dz, dz));
                        if (len != 0) {
                            rem = len - vy;
                            if (rem < 0)
                                rem = 0;
                            ratio = Iwram_RatioMulQ14(len, rem);
                            obj->velocity_x = Iwram_MulQ16(dx, ratio);
                            obj->velocity_y = Iwram_MulQ16(dy, ratio);
                            obj->velocity_z = Iwram_MulQ16(dz, ratio);
                        }
                    }
                    obj->terrain_height = ground;
                }

                if ((obj->motion_flags & 2) != 0) {
                    ground = obj->terrain_height;
                    if (py > ground) {
                        obj->velocity_y -= obj->gravity;
                    } else if (obj->velocity_y < 0) {
                        py = ground;
                        obj->velocity_y = -Iwram_MulQ16(obj->velocity_y, obj->vertical.bounce);
                        if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
                            obj->velocity_y = 0;
                    }
                }

                if ((obj->motion_flags & 4) != 0) {
                    phase = (u32)(obj->vertical.phase & 0x3f);
                    if ((obj->motion_flags & 8) != 0)
                        obj->velocity_y = ScriptObject_CycleTable[phase >> 1] * obj->gravity / 16;
                    else
                        obj->velocity_y = ScriptObject_CycleTable[phase >> 1] * obj->gravity / 64;
                    obj->vertical.phase++;
                }
            }
        }

        px += obj->velocity_x;
        py += obj->velocity_y;
        pz += obj->velocity_z;

        if ((obj->collision_flags & 0x80) != 0) {
            pos[0] = px;
            pos[1] = py;
            pos[2] = pz;
            if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)obj, pos) != 0) {
                obj->refused++;
                continue;
            }
            obj->refused = 0;
        }

        switch (obj->arrival_axis) {
        case 16:
            if (px == obj->target_x ||
                ((obj->x - obj->target_x) ^ (px - obj->target_x)) < 0)
                reached = 1;
            break;
        case 17:
            if (py == obj->target_y ||
                ((obj->y - obj->target_y) ^ (py - obj->target_y)) < 0)
                reached = 1;
            break;
        case 18:
            if (pz == obj->target_z ||
                ((obj->z - obj->target_z) ^ (pz - obj->target_z)) < 0)
                reached = 1;
            break;
        default:
            break;
        }

        if (reached != 0) {
            if (obj->snap_to_target != 0) {
                px = obj->target_x;
                obj->velocity_x = 0;
                pz = obj->target_z;
                obj->velocity_z = 0;
                if (obj->motion_flags == 0) {
                    py = obj->target_y;
                    obj->velocity_y = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            obj->arrival_axis = 0;
        }

        obj->x = px;
        obj->y = py;
        obj->z = pz;

        if ((obj->steering_flags & 1) != 0) {
            px = obj->velocity_x;
            pz = obj->velocity_z;
            if (px != 0 || pz != 0) {
                s32 limit;

                work = (s16)(ArcTan2(pz, px) - obj->facing);
                limit = 0x1000;
                if (work > limit)
                    work = limit;
                limit = -0x1000;
                if (work < limit)
                    work = limit;
                obj->facing = obj->facing + work;
            }
        }
    }
}
