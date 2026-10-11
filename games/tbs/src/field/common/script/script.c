//--------------------------------------------------------------------------
// script.c
// オブジェクトスクリプト：位置の移動と byte_54 の命令
//--------------------------------------------------------------------------

#include "script.h"
#include "object_runtime.h"
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
#include "global_cells.h"
#include "iwram_call.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "dma.h"
#include "ramcode.h"
#include "mem.h"
#include "objdisp.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "io_reg.h"
#include "btlsys.h"
#include "fixed_math.h"
#include "keystate.h"
#include "edition.h"
#include "scene.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "object_dispatch.h"
#include "animspr.h"

//--------------------------------------------------------------------------
// スクリプトの読む位置を移し、1 を返して次の命令を続けて実行する
//--------------------------------------------------------------------------
#if GAME_TLA
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc)
#else
s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc)
#endif
{
	s32 zero;

#if GAME_TLA
	// 命令の次の語が指すスクリプトへ飛ぶ
	sc->script = (const s32 *)sc->script[sc->cursor + 1];
#else
	// 命令の次の語を新しい先頭にする
	sc->script = sc->script + sc->cursor + 1;
#endif
	zero = 0;
	sc->cursor = zero;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 0 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_ClearByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 0;
	sc->cursor++;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 1 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_SetByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 1;
	sc->cursor++;
	return 1;
}

//--------------------------------------------------------------------------

// motion_set_position_and_reset_motion.c

// オブジェクト：位置の設定と移動の取り消し

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 位置を置き、移動先を「なし」に、速度を 0 にする
// obj     : オブジェクト
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y, s32 z)
{
	obj->z = z;
	obj->target_x = 0x80000000; // 移動先なし
	obj->target_y = 0x80000000;
	obj->target_z = 0x80000000;
	obj->x = x;
	obj->y = y;
	obj->velocity_x = 0;
	obj->velocity_y = 0;
	obj->velocity_z = 0;
}

//--------------------------------------------------------------------------

// script.c

// オブジェクトスクリプト：位置の移動と byte_54 の命令

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// スクリプトの読む位置を移し、1 を返して次の命令を続けて実行する
//--------------------------------------------------------------------------
#if GAME_TLA
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc)
#else
s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc)
#endif
{
	s32 zero;

#if GAME_TLA
	// 命令の次の語が指すスクリプトへ飛ぶ
	sc->script = (const s32 *)sc->script[sc->cursor + 1];
#else
	// 命令の次の語を新しい先頭にする
	sc->script = sc->script + sc->cursor + 1;
#endif
	zero = 0;
	sc->cursor = zero;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 0 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_ClearByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 0;
	sc->cursor++;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 1 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_SetByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 1;
	sc->cursor++;
	return 1;
}

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
s32 ArcTan2(s32, s32);

static __inline__ s32 Square(s32 value)
{
    return value * value;
}

void Object_UpdateAllThumb(void)
{
	s32 Map_GetTerrainHeight(s32, s32, s32);
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

//--------------------------------------------------------------------------
// motion_set_position_and_reset_motion.c
// オブジェクト：位置の設定と移動の取り消し
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 位置を置き、移動先を「なし」に、速度を 0 にする
// obj     : オブジェクト
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y, s32 z)
{
	obj->z = z;
	obj->target_x = 0x80000000; // 移動先なし
	obj->target_y = 0x80000000;
	obj->target_z = 0x80000000;
	obj->x = x;
	obj->y = y;
	obj->velocity_x = 0;
	obj->velocity_y = 0;
	obj->velocity_z = 0;
}

void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *, s32, s32, s32);

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
    if (object->snap_to_target == 0) {
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
    axis = &object->arrival_axis;
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

    RamCode_Load(routine, Object_UpdateAll, Object_UpdateAllCodeSize);
    routine();
    MemFree(routine);
}

void Object_UpdateAllMotion(void)
{
    struct ScriptMotionObject *obj;
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

    obj = gObjectSlots;
    for (cnt = 13; cnt >= 0; cnt--, obj++) {
        if (obj->script == 0)
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
                    vx = obj->velocity_x + Iwram_MulQ16(dx, ratio);
                    obj->velocity_x = vx;
                    dx = obj->velocity_z + Iwram_MulQ16(dz, ratio);
                    obj->velocity_z = dx;
                    dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
                    if (dist > obj->speed_limit) {
                        ratio = Iwram_RatioMulQ14(dist, obj->speed_limit);
                        obj->velocity_x = Iwram_MulQ16(vx, ratio);
                        obj->velocity_z = Iwram_MulQ16(dx, ratio);
                    }
                }
            } else {
                dx = obj->velocity_x;
                dz = obj->velocity_z;
                if ((dx | dz) != 0) {
                    dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
                    if (dist != 0) {
                        if (dist - obj->acceleration < 0) {
                            obj->velocity_x = 0;
                            obj->velocity_z = 0;
                        } else {
                            ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
                            obj->velocity_x = Iwram_MulQ16(dx, ratio);
                            obj->velocity_z = Iwram_MulQ16(dz, ratio);
                        }
                    } else {
                        obj->velocity_x = 0;
                        obj->velocity_z = 0;
                    }
                }
            }
            if (obj->motion_flags & 2) {
                if (y > obj->terrain_height) {
                    obj->velocity_y -= obj->gravity;
                } else if (obj->velocity_y < 0) {
                    y = obj->terrain_height;
                    obj->velocity_y = -Iwram_MulQ16(obj->velocity_y, obj->vertical.bounce);
                    if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
                        obj->velocity_y = 0;
                }
            }
        }
        x += obj->velocity_x;
        y += obj->velocity_y;
        z += obj->velocity_z;
        if (obj->arrival_axis != 0) switch (obj->arrival_axis) {
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
            if (obj->snap_to_target) {
                obj->velocity_x = 0;
                obj->velocity_z = 0;
                x = obj->target_x;
                z = obj->target_z;
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
        if (obj->steering_flags & 1) {
            x = obj->velocity_x;
            z = obj->velocity_z;
            if (x != 0 || z != 0) {
                turn = (s16)(ArcTan2(z, x) - obj->facing);
                if (turn > 0x1000)
                    turn = 0x1000;
                if (turn < -0x1000)
                    turn = -0x1000;
                obj->facing += turn;
            }
        }
    }
}

s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter)
{
    s16 cursor = interpreter->cursor;

    interpreter->lookup_result = interpreter->script[cursor + 1] - 1;
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 0;
}

s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
    if ((u32)interpreter->delay > 0x3B) {
        interpreter->delay = 0;
        interpreter->cursor = (u16)interpreter->cursor + 1;
        return 1;
    }
    if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0) {
        interpreter->cursor = (u16)interpreter->cursor + 1;
        return 1;
    }
    return 0;
}

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
    s16 initial = interpreter->cursor;
    ScriptCommand callback =
        (ScriptCommand)interpreter->script[initial + 1];

    if (callback(interpreter) != 0)
        return 0;
    if (interpreter->cursor == initial)
        interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
    u32 *entries;
    s32 index;
    s16 *field = &interpreter->lookup_result;
    s32 zero = 0;

    *field = zero;
    if (key == 0) {
        return 0;
    }

    key &= 0xBFFFFFFF;
    entries = (u32 *)interpreter->script;
    index = 0;
    do {
        if (*entries++ == key) {
            return index + 1;
        }
        index++;
    } while (index <= 0x3FF);
    return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
    const s32 *arguments;
    s32 repeat_limit;
    s32 jump_key;
    u8 *repeat_count;
    s32 next_count;

    arguments = &interpreter->script[interpreter->cursor + 1];
    repeat_limit = *arguments++;
    jump_key = *arguments;
    if (repeat_limit == 0xFFFF) {
        interpreter->cursor = Script_FindLabel(interpreter, jump_key);
    } else {
        repeat_count = &interpreter->repeat_count;
        next_count = *repeat_count + 1;
        *repeat_count = next_count;
        if ((s32)(u8)next_count < (s32)(s16)repeat_limit) {
            interpreter->cursor = Script_FindLabel(interpreter, jump_key);
        } else {
            *repeat_count = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_FindLabel(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result != 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result == 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Script_MainScript;
    interpreter->cursor = (result = 0);
    return result;
}

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
    interpreter->condition_result =
        GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_TestFar(value);
    GameFlag_SetBitFar(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_TestFar(value);
    GameFlag_ClearBitFar(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 result;

    value = interpreter->script[interpreter->cursor + 1];
    result = GameFlag_TestFar(value);
    interpreter->condition_result = result;
    if (((u32)result << 0x18) == 0x01000000) {
        GameFlag_ClearBitFar(value);
    } else {
        GameFlag_SetBitFar(value);
    }
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter)
{
    ObjectDispatch_ApplyArgumentToChildren(interpreter,
        interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter)
{
    ObjectDispatch_Release((struct DispatchObject *)interpreter);
    return 0;
}

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
    s32 index;
    u8 *flags;
    struct ObjectRuntime *entry = (struct ObjectRuntime *)gObjectSlots;

    flags = &entry->unknown_59;
    for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
        if (entry->script != NULL && (1 & *flags)
            && entry != (struct ObjectRuntime *)object) {
            if (Runtime_CheckRadiusOverlap(&entry->x,
                    ((struct FieldActor *)entry)->radius - 2,
                    values, ((struct FieldActor *)object)->radius - 2) >= 0)
                return -1;
        }
    }
    return 0;
}

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
    struct ScriptObjectEntry *object, s32 *values)
{
    s32 index;
    u8 *flags;
    struct ObjectRuntime *entry = (struct ObjectRuntime *)gObjectSlots;

    flags = &entry->unknown_59;
    for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
        if (entry->script != NULL && (1 & *flags)
            && entry != (struct ObjectRuntime *)object) {
            if (Runtime_CheckRadiusOverlap(&entry->x,
                    ((struct FieldActor *)entry)->radius - 2,
                    values, ((struct FieldActor *)object)->radius - 2) >= 0)
                return (struct ScriptObjectEntry *)entry;
        }
    }
    return NULL;
}

s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter)
{
    s32 first;
    s32 second;
    s32 third;
    const s32 *argument;

    argument = interpreter->script + interpreter->cursor;
    argument++;
    first = *argument;
    argument++;
    second = *argument;
    argument++;
    third = *argument;
    Object_SetPositionAndResetMotion((struct ObjectRuntime *)interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter)
{
    s32 first;
    s32 second;
    s32 third;
    const s32 *argument;

    argument = interpreter->script + interpreter->cursor;
    argument++;
    first = *argument;
    argument++;
    second = *argument;
    argument++;
    third = *argument;
    Object_SetMoveTarget((struct ObjectRuntime *)interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    u8 *entry = (u8 *)(object->script + (s16)object->script_cursor);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    object->script_value = ArcTan2(
        (s32)((u32)target->z - (u32)object->z),
        (s32)((u32)target->x - (u32)object->x));
    object->script_cursor++;
    return 1;
}

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Object_SetMoveTarget((struct ObjectRuntime *)object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}

s32 Script_ApplyLocalOffsetPosition(u8 *arg0)
{
    s32 offset[3];

    Object_SetMoveTarget(
        (struct ObjectRuntime *)arg0,
        *(s32 *)(arg0 + 8) + offset[0],
        *(s32 *)(arg0 + 12) + offset[1],
        *(s32 *)(arg0 + 16) + offset[2]
    );
    *(u16 *)(arg0 + 4) += 3;
    return 1;
}

struct MapScrollBounds {
    u8 unknown_00[0xec];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj)
{
    struct ObjectRuntime *leader;
    struct MapScrollBounds *bounds;
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

    bounds = gMapWork[0];
    leader = obj->linked_object;
    min_x = bounds->left + 0x780000;
    min_z = bounds->top + leader->y + 0x600000;
    max_x = bounds->right - 0x780000;
    max_z = bounds->bottom + leader->y - 0x400000;
    obj->flags = 0;
    if (leader != NULL && leader->script != NULL) {
        x = leader->x;
        y = leader->y;
        z = leader->z;
        obj->target_x = TARGET_UNSET;
        obj->target_y = TARGET_UNSET;
        obj->target_z = TARGET_UNSET;
        if (x < min_x)
            x = min_x;
        if (z < min_z)
            z = min_z;
        if (x > max_x)
            x = max_x;
        if (z > max_z)
            z = max_z;
        if (obj->action != 0) {
            obj->x = x;
            obj->y = y;
            obj->z = z;
        } else {
            // XZ 平面の距離: dy は Object_SetMoveTarget と同じ形で求めるが距離には使わない
            dx = (x - obj->x) / 0x10000;
            /* FAKEMATCH: dead code: this dy is overwritten below and the compiler drops it, but its references rank y above the bounds in global allocation, keeping y in r5 as native; without it 114 rows differ in the English build (y in r9). The form is the sibling Object_SetMoveTarget's three-axis delta. */
            dy = (y - obj->y) / 0x10000;
            dz = (z - obj->z) / 0x10000;
            dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
            dx = x - obj->x;
            dy = y - obj->y;
            dz = z - obj->z;
            if (dist < 0x400000)
                dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
            if ((step = dist / 8) > obj->speed_limit)
                step = obj->speed_limit;
            if (dist < 0x4000) {
                obj->x = x;
                obj->z = z;
            } else {
                if (dist > step) {
                    dx = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dx), step);
                    dz = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dz), step);
                }
                obj->x += dx;
                obj->z += dz;
            }
            if ((dy < 0 ? -dy : dy) > 0x8000)
                dy /= 4;
            obj->y += dy;
        }
    }
    obj->step++;
    return 1;
}

extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *object, s32 argument);

s32 Object_ApproachLinkedObject(struct ObjectRuntime *object)
{
  s32 len;
  s32 mz;
  s32 dx;
  s32 dz;
  s32 dzh;
  s32 dxh;
  s32 n;
  s32 mx;
  s32 dx2;
  struct ObjectRuntime *link;

  link = object->linked_object;
  object->speed_limit = (s32)(link->speed_limit);
  object->acceleration = (s32)(link->acceleration);
  dx = (link->x) - (object->x);
  dz = (link->z) - (object->z);
  dxh = dx >> 0x10;
  dzh = dz >> 0x10;
  len = _call_via_r3((dxh *dxh) + (dzh *dzh), dx, dzh, (u32)IwramSqrt);
  if (len > 0x10)
  {
    dx2 = dx;
    n = len - 0x10;
    mx = dx2 *n / len;
    mz = dz *n / len;
    Object_SetMoveTarget(object, (object->x) + mx, object->y, (object->z) + mz);
    ObjectDispatch_ApplyArgumentToChildren(object, 2);
    object->step = (u16)((object->step) + 1);
    return 1;
  }
  ObjectDispatch_ApplyArgumentToChildren(object, 1);
  return 0;
}

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct FieldPosition *position);
s32 ArcTan2(s32 y, s32 x);

s32 Object_Wander(struct ScriptObjectRuntime *object)
{
    struct FieldPosition pos;
    struct FieldPosition probe;
    s32 base;
    const s32 *args;
    s32 range;
    s32 limit;
    s32 radius;
    s32 heading;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    /* FAKEMATCH: needed to match: the retry label and one-pass distance block preserve the 32-byte frame and division/load registers. */
    tries = 0;
    limit = limit * limit;
retry:
    tries++;
    if (tries <= 7) {
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        radius = base + Iwram_MulQ16(Random16(), range);
        heading = object->script_value + (Random16() >> 2) - (Random16() >> 2);
        Vector_AddPolarOffset(radius, heading, &pos);
        if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
            goto retry;
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
            goto retry;
        radius += 0x80000;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading, &probe);
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
        if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        do {
            dx = pos.x / 0x10000 - object->home_x;
            dz = pos.z / 0x10000 - object->home_z;
        } while (0);
        if (dx * dx + dz * dz > limit)
            goto retry;
        goto found;
    }
    object->script_value += 0x8000;
    object->turned_back = 1;
    return 0;
found:
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
    object->script_cursor += 4;
    return 1;
}

s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object)
{
    struct FieldPosition pos;
    struct FieldPosition probe;
    s32 base;
    const s32 *args;
    s32 range;
    s32 limit;
    s32 radius;
    u16 heading;
    u16 angle;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    angle = (s16)object->script_value;
    limit = limit * limit;
    tries = 0;
    dx = object->x / 0x10000 - object->home_x;
    dz = object->z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto home;
roam:
    tries++;
    if (tries > 7)
        goto home;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
        goto roam;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    radius += 0x80000;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    dx = pos.x / 0x10000 - object->home_x;
    dz = pos.z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto roam;
    object->flags_59 |= 2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
    goto done;
home:
    tries = 0;
    angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
    tries++;
    if (tries > 7)
        goto done;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
        goto back;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
        goto back;
    object->flags_59 &= ~2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
done:
    object->script_cursor += 4;
    return 1;
}

enum ScriptOperandOperation {
    SCRIPT_OPERAND_SET,
    SCRIPT_OPERAND_ADD,
    SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Script_OperandHandlerTable[];

static __inline__ void StoreHalfword(s16 *address, s32 value)
{
    *address = value;
}

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->script_address = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->script_address += (u32)value * 4;
        return;
    }
    state->comparison_result = state->script_address == (u32)value;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->cursor = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->cursor =
            (u16)((u32)(s32)(s16)state->cursor + (u32)value);
    } else {
        state->comparison_result = (s16)state->cursor == (s16)value;
    }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->unsigned_halfword = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        state->comparison_result = state->unsigned_halfword == (u16)value;
    }
}

void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_08 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_08 += value;
        return;
    }
    state->comparison_result = state->word_08 == value;
}

void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_0c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_0c += value;
        return;
    }
    state->comparison_result = state->word_0c == value;
}

void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_10 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_10 += value;
        return;
    }
    state->comparison_result = state->word_10 == value;
}

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->halfword_20 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
        return;
    }
    state->comparison_result = state->halfword_20 == (s16)value;
}

void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_18 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_18 += value;
        return;
    }
    state->comparison_result = state->word_18 == value;
}

void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_1c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_1c += value;
        return;
    }
    state->comparison_result = state->word_1c == value;
}

void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_24 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_24 += value;
        return;
    }
    state->comparison_result = state->word_24 == value;
}

void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_28 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_28 += value;
        return;
    }
    state->comparison_result = state->word_28 == value;
}

void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_2c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_2c += value;
        return;
    }
    state->comparison_result = state->word_2c == value;
}

void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_30 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_30 += value;
        return;
    }
    state->comparison_result = state->word_30 == value;
}

void Script_SetOrCompareAcceleration(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_34 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_34 += value;
        return;
    }
    state->comparison_result = state->word_34 == value;
}

void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_38 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_38 += value;
        return;
    }
    state->comparison_result = state->word_38 == value;
}

void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_3c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_3c += value;
        return;
    }
    state->comparison_result = state->word_3c == value;
}

void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_40 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_40 += value;
        return;
    }
    state->comparison_result = state->word_40 == value;
}

void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_44 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_44 += value;
        return;
    }
    state->comparison_result = state->word_44 == value;
}

void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_48 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_48 += value;
        return;
    }
    state->comparison_result = state->word_48 == value;
}

void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_14 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_14 += value;
        return;
    }
    state->comparison_result = state->word_14 == value;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_4c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_4c += value;
        return;
    }
    state->comparison_result = state->word_4c == value;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->address_50 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->address_50 += (u32)value * 4;
        return;
    }
    state->comparison_result = state->address_50 == (u32)value;
}

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_54 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_54 = (u8)((u32)state->byte_54 + (u32)value);
    } else {
        state->comparison_result = state->byte_54 == (u8)value;
    }
}

void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_55 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_55 = (u8)((u32)state->byte_55 + (u32)value);
    } else {
        state->comparison_result = state->byte_55 == (u8)value;
    }
}

void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_56 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_56 = (u8)((u32)state->byte_56 + (u32)value);
    } else {
        state->comparison_result = state->byte_56 == (u8)value;
    }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->comparison_result = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->comparison_result =
            (u8)((u32)state->comparison_result + (u32)value);
    } else {
        state->comparison_result = state->comparison_result == (u8)value;
    }
}

void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_58 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_58 = (u8)((u32)state->byte_58 + (u32)value);
    } else {
        state->comparison_result = state->byte_58 == (u8)value;
    }
}

void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_59 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_59 = (u8)((u32)state->byte_59 + (u32)value);
    } else {
        state->comparison_result = state->byte_59 == (u8)value;
    }
}

void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_5a = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_5a = (u8)((u32)state->byte_5a + (u32)value);
    } else {
        state->comparison_result = state->byte_5a == (u8)value;
    }
}

void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_5b = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_5b = (u8)((u32)state->byte_5b + (u32)value);
    } else {
        state->comparison_result = state->byte_5b == (u8)value;
    }
}

void Script_SetOrCompareRepeatCount(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_5d = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_5d += value;
    } else {
        work->comparison_result = work->byte_5d == (u8)value;
    }
}

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->halfword_5e = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->halfword_5e = work->halfword_5e + value;
        return;
    }
    work->comparison_result = (s16)work->halfword_5e == (s16)value;
}

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->signed_halfword_64 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->signed_halfword_64 =
            (u16)work->signed_halfword_64 + value;
    } else {
        work->comparison_result = work->signed_halfword_64 == (s16)value;
    }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->signed_halfword_66 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->signed_halfword_66 =
            (u16)work->signed_halfword_66 + value;
    } else {
        work->comparison_result = work->signed_halfword_66 == (s16)value;
    }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->word_68 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->word_68 = work->word_68 + (u32)value;
        return;
    }
    work->comparison_result = work->word_68 == (u32)value;
}

void Script_SetOrCompareUpdateAddress(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->word_6c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->word_6c = work->word_6c + (u32)value;
        return;
    }
    work->comparison_result = work->word_6c == (u32)value;
}

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_62 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_62 += value;
    } else {
        work->comparison_result = work->byte_62 == (u8)value;
    }
}

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_63 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_63 += value;
    } else {
        work->comparison_result = work->byte_63 == (u8)value;
    }
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_SET, command[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_ADD, command[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_COMPARE, command[1]);
    work->cursor += 3;
    return 1;
}

void ObjectDispatch_SetUpdateValue(void *raw, s32 value)
{
    struct ObjectRuntime *object = raw;

    if (object != NULL)
        *(s32 *)object->unknown_6c = value;
}

#if EDITION_INTERNATIONAL

#define ASSIGNED_KEYS gKeyState
#else

#define ASSIGNED_KEYS gKeysHeld
#endif

u32 Field_StoreAssignedKeyValue(u32 value)
{
    u32 selector = value >> 14;
    u32 request = 0x3FFF & value;
    struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

    if (GameFlag_TestFar(0x107) != 0) {
        StoreHalfword((s16 *)&work->raised_trigger, 0xFA);
    } else if (work->mode == 3) {
        if (ASSIGNED_KEYS & 0x100) {
            StoreHalfword((s16 *)&work->raised_trigger, 0xFC88);
        } else if (ASSIGNED_KEYS & 0x200) {
            StoreHalfword((s16 *)&work->raised_trigger, 0xFC87);
        }
    } else {
        switch (selector) {
        case 0:
            StoreHalfword(&work->psynergy_request, request);
            break;
        case 1:
            StoreHalfword((s16 *)work->unknown_180, request);
            break;
        }
    }

    return request;
}

s32 Field_CheckConfiguredKeys(void)
{
    struct EventRuntime *work = gEventWork;
    s32 result = 0;

    if (work == NULL)
        return 0;
    if (gKeyState & gGameState.select_keys) {
        work->unknown_172[0] = 1;
        result = 1;
    } else if (gKeyState & gGameState.a_keys) {
        work->unknown_172[1] = 1;
        result = 1;
    } else if (gKeyState & gGameState.start_keys) {
        work->unknown_172[2] = 1;
        result = 1;
    } else if (gKeyState & gGameState.first_shortcut_keys) {
        result = Field_StoreAssignedKeyValue(gGameState.first_shortcut);
    } else if (gKeyState & gGameState.second_shortcut_keys) {
        result = Field_StoreAssignedKeyValue(gGameState.second_shortcut);
    }
    return result;
}

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
    const struct FieldPosition *first = (const struct FieldPosition *)a;
    const struct FieldPosition *second = (const struct FieldPosition *)b;
    const s32 *first_word = &first->x;
    const s32 *second_word = &second->x;
    s32 dx = (*first_word++ - *second_word++) >> 16;
    s32 dy = (*first_word++ - *second_word++) >> 16;
    s32 dz = (*first_word - *second_word) >> 16;
    s32 radius = radius_a + radius_b;

    if (!(dx > 0x400000) && !(dz > 0x400000) &&
        dx * dx + dy * dy + dz * dz < radius * radius)
        return 0;
    return -1;
}

#undef gObjectSlots

struct KeyMoveEventWork {
    u8 unknown_000[0x19c];
    u16 blocked_steps;
};

struct WorldMapSprite {
    struct FieldSprite sprite;
    u8 unknown_28[4];
    u8 *shadow;
};
extern volatile u8 gDebugMode;
void ObjectDispatch_ApplyArgumentToChildren(void *object, s32 value);
s32 AnimationObjects_SelectAnimation(struct AnimationObject *, s32);
void AudioCommand_PlayFar(s32 cue);
extern u8 ResourceBlockOwners[];

s32 FieldObject_UpdatePlayerControl(struct ObjectRuntime *object)
{
    struct FieldPosition position;
    struct FieldPosition test_position;
    struct FieldPosition contact;
    s16 angle_offsets[22];
    s32 collision;
    s32 pushed;
    s16 heading;
    s32 angle;
    s32 mode;
    u16 direction;
    s32 rotation;
    s32 angle_index;
    s32 count;
    struct ObjectRuntime *other;

    collision = 0;
    pushed = 0;
    if (gDebugMode != 0 && GameFlag_TestFar(350) != 0) {
        s32 free;
        s32 block;

        free = 0;
        for (block = 0; block < 512; block++) {
            if (ResourceBlockOwners[block] == 0xff)
                free++;
        }
        if (free - 136 < 0)
            AudioCommand_PlayFar(135);
    }
    if (gDebugMode != 0) {
        s32 mask;

        mask = KEY_L;
        if (gKeysHeld & mask) {
            s32 wait;

            wait = mask;
        wait_a:
            --wait;
            if (wait != 0)
                goto wait_a;
            wait = 95;
        wait_b:
            wait--;
            if (wait >= 0)
                goto wait_b;
            wait = 63;
        wait_c:
            wait--;
            if (wait >= 0)
                goto wait_c;
            wait = 63;
        wait_d:
            wait--;
            if (wait >= 0)
                goto wait_d;
        }
    }
    if (gKeysHeld & gGameState.dash_keys) {
        object->speed_limit = 0x18000;
        object->acceleration = 0x4000;
        mode = 5;
    } else {
        object->speed_limit = 0x10000;
        object->acceleration = 0x4000;
        mode = 2;
    }
    if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & KEY_B)) {
        object->speed_limit = 0x40000;
        object->acceleration = 0x10000;
        mode = 5;
    }

    angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
    if ((u16)angle == 0xffff) {
        collision |= 4;
        goto update_object;
    }

    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L)) {
        heading = angle;
        goto update_object;
    }
    if (Map_CheckObjectStep(object, &position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &test_position);
    if (Map_CheckObjectStep(object, &test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &test_position);
    if (Map_CheckObjectStep(object, &test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &test_position);
    if (Map_CheckObjectStep(object, &test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &test_position);
    if (Map_CheckObjectStep(object, &test_position) != 0)
        goto choose_direction;
    heading = angle;
    goto push_objects;

choose_direction:
    direction = angle;
    count = 0;
    angle_offsets[count++] = direction + 0x1000;
    angle_offsets[count++] = direction - 0x1000;
    angle_offsets[count++] = direction + 0x2000;
    angle_offsets[count++] = direction - 0x2000;
    angle_offsets[count++] = direction + 0x3000;
    angle_offsets[count++] = direction - 0x3000;
    for (angle_index = 0; angle_index < count; angle_index++) {
        heading = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Map_CheckObjectStep(object, &position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &test_position);
        if (Map_CheckObjectStep(object, &test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &test_position);
        if (Map_CheckObjectStep(object, &test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &test_position);
        if (Map_CheckObjectStep(object, &test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &test_position);
        if (Map_CheckObjectStep(object, &test_position) == 0)
            goto push_objects;
    }
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    collision |= 1;

push_objects:
    contact.x = object->x;
    contact.y = object->y;
    contact.z = object->z;
    Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
    other = gObjectSlots;
    for (angle_index = 63; angle_index >= 0; angle_index -= 1, other++) {
        s32 radius;

        radius = object->radius - 2;
        if (other->script == 0)
            continue;
        if ((other->unknown_59 & 1) == 0)
            continue;
        if (other == object)
            continue;
        if (Runtime_CheckRadiusOverlap(&other->x, radius, &contact.x, other->radius - 2) < 0)
            continue;
        if ((other->unknown_59 & 2) == 0 || other->movement_state != 0)
            goto blocked;
        direction = ArcTan2(other->z - object->z, other->x - object->x);
        test_position.x = other->x;
        test_position.y = other->y;
        test_position.z = other->z;
        heading = direction;
        Vector_AddPolarOffset(0x4000, direction, &test_position);
        if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)other, (s32 *)&test_position))
            goto blocked;
        test_position.x = other->x;
        test_position.y = other->y;
        test_position.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction, &test_position);
        if (Map_CheckObjectStep(other, &test_position) != 0)
            goto blocked;
        test_position.x = other->x;
        test_position.y = other->y;
        test_position.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction + 0x1000, &test_position);
        if (Map_CheckObjectStep(other, &test_position) != 0)
            goto blocked;
        if (Map_CheckObjectStep(other, &test_position) != 0)
            goto blocked;
        test_position.x = other->x;
        test_position.y = other->y;
        test_position.z = other->z;
        Vector_AddPolarOffset(0xa0000, direction - 0x1000, &test_position);
        if (Map_CheckObjectStep(other, &test_position) != 0)
            goto blocked;
        Vector_AddPolarOffset(0x4000, direction, (struct FieldPosition *)&other->x);
        other->target_x = -0x80000000;
        other->target_y = -0x80000000;
        other->target_z = -0x80000000;
        pushed |= 1;
        continue;
    blocked:
        collision |= 2;
    }
    if (collision == 0 && pushed != 0) {
        object->speed_limit = 0x4000;
        object->acceleration = 0x2000;
    }

update_object:
    if (gEventWork != NULL) {
        if (collision & 3)
            gEventWork->blocked_steps++;
        else
            gEventWork->blocked_steps = 0;
    }
    if (pushed != 0) {
        ObjectDispatch_ApplyArgumentToChildren(object, 8);
    } else if (collision != 0) {
        s32 action;

        action = 9;
        if (UnitGetFar(gGameState.selected_actor)->hp == 0)
            action = 22;
        ObjectDispatch_ApplyArgumentToChildren(object, action);
    } else {
        ObjectDispatch_ApplyArgumentToChildren(object, mode);
    }

    if (collision != 0) {
        object->target_x = -0x80000000;
        object->target_y = -0x80000000;
        object->target_z = -0x80000000;
        object->velocity_x = 0;
        object->velocity_z = 0;
        if (collision & 3) {
            s32 difference;

            difference = (u16)angle - object->angle;
            difference = (s16)difference;
            if (difference > 0x1000)
                difference = 0x1000;
            if (difference < -0x1000)
                difference = -0x1000;
            object->angle += difference;
        }
        object->action = 0;
        object->unknown_66 = 2;
    } else {
        s32 square;

        Object_SetMoveTarget(object, position.x, position.y, position.z);
        square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
                           + Iwram_MulQ16(object->velocity_z, object->velocity_z));
        object->velocity_x = 0;
        object->velocity_z = 0;
        Vector_AddPolarOffset(square, (u16)heading, (struct FieldPosition *)&object->velocity_x);
        if (object->action != 0)
            object->action--;
    }

    rotation = (u16)angle;
    if (((struct MapScrollWork *)gMapWork[0])->footprints != 0
        && object->action == 0 && collision == 0) {
        struct ObjectRuntime *effect;

        effect = FieldObject_Create(25, object->x, object->y, object->z);
        if (effect != 0) {
            struct FieldSprite *animation;

            effect->terrain_height = object->terrain_height;
            animation = effect->animation;
            ObjectDispatch_Initialize((struct DispatchObject *)effect, (u32)WorldMap_FootprintActions);
            effect->unknown_23 = 2;
            effect->flags = collision;
            if (animation != 0) {
                AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
                animation->flags = collision;
                animation->rotation = rotation + 0x4000;
                animation->priority = 3;
            }
            if (object->unknown_66 == 2) {
                AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 2);
                object->unknown_66 = collision;
            }
            if (object->unknown_66 != 0)
                effect->angle = 0x8000;
            if (mode == 5)
                object->action = 12;
            else
                object->action = 18;
            object->unknown_66 ^= 1;
        }
    }
    Field_CheckConfiguredKeys();
    object->step++;
    return 1;
}

#if EDITION_INTERNATIONAL
#define WORLD_PROBE_RADIUS 0x70000
#else
#define WORLD_PROBE_RADIUS 0x80000
#endif

s32 Object_MoveOnWorldMap(struct ObjectRuntime *object)
{
    struct FieldPosition position;
#if EDITION_INTERNATIONAL
    struct FieldPosition test_position;
    s16 angle_offsets[28];
#else
    struct FieldPosition left_position;
    struct FieldPosition right_position;
    s16 angle_offsets[22];
#endif
    s32 collision;
    s32 angle;
    u16 direction;
    s32 angle_index;
    s32 mode;
    s32 count;
    s32 difference;
    s32 collision_kind;
    s32 square;
    s32 rate;
    struct PerspectiveWork *work;
    s32 step;
    struct FieldSprite *sprite;
    struct ObjectRuntime *effect;
    struct FieldSprite *animation;

    mode = 2;
    collision = 0;
#if !EDITION_INTERNATIONAL
    count = 0;
#endif
    if (gKeysHeld & gGameState.dash_keys) {
        object->speed_limit = 0x10000;
        object->acceleration = 0x14000;
        mode = 5;
    } else {
        object->speed_limit = 0x8000;
        object->acceleration = 0x4000;
    }
    if ((gKeysRepeat & KEY_L) != 0)
        object->speed_limit = 0x40000;

    angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
    direction = angle;
    if (direction == 0xffff) {
        collision |= 4;
        goto update_object;
    }

    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L) != 0)
        goto update_object;
#if EDITION_INTERNATIONAL
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
        goto choose_direction;

    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    goto update_object;

choose_direction:
    count = 0;
    angle_offsets[count++] = direction + 0x1000;
    angle_offsets[count++] = direction - 0x1000;
    angle_offsets[count++] = direction + 0x2000;
    angle_offsets[count++] = direction - 0x2000;
    angle_offsets[count++] = direction + 0x3000;
    angle_offsets[count++] = direction - 0x3000;
    angle_offsets[count++] = direction + 0x4000;
    angle_offsets[count++] = direction - 0x4000;
    for (angle_index = 0; angle_index < count; angle_index++) {
        direction = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        angle = (s16)direction;
        goto update_object;
    }
    collision |= 1;

#else
    left_position.x = object->x;
    left_position.y = object->y;
    left_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
    right_position.x = object->x;
    right_position.y = object->y;
    right_position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
    if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
         | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
         | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position)) == 0)
        goto update_object;

    angle_offsets[count++] = direction + 0x1000;
    angle_offsets[count++] = direction - 0x1000;
    angle_offsets[count++] = direction + 0x2000;
    angle_offsets[count++] = direction - 0x2000;
    angle_offsets[count++] = direction + 0x3000;
    angle_offsets[count++] = direction - 0x3000;
    for (angle_index = 0; angle_index < count; angle_index++) {
        direction = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
        left_position.x = object->x;
        left_position.y = object->y;
        left_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
        right_position.x = object->x;
        right_position.y = object->y;
        right_position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
        if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
             | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
             | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position)) != 0)
            continue;
        angle = (s16)direction;
        goto update_object;
    }
    collision |= 1;
#endif

update_object:
    if (gEventWork != NULL) {
        if ((collision & 3) != 0)
            gEventWork->blocked_steps++;
        else
            gEventWork->blocked_steps = 0;
    }
    if (collision != 0)
        ObjectDispatch_ApplyArgumentToChildren(object, 9);
    else
        ObjectDispatch_ApplyArgumentToChildren(object, mode);

    if (collision != 0) {
        object->target_x = -0x80000000;
        object->target_y = -0x80000000;
        object->target_z = -0x80000000;
        object->velocity_x = 0;
        object->velocity_z = 0;
        if ((collision & 3) != 0) {
            direction = angle;
            difference = (s16)(direction - object->angle);
            if (difference > 0x1000)
                difference = 0x1000;
            if (difference < -0x1000)
                difference = -0x1000;
            object->angle += difference;
        }
        object->action = 0;
        object->unknown_66 = 2;
        goto movement_done;
    }

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
                       + Iwram_MulQ16(object->velocity_z, object->velocity_z));
    object->velocity_x = collision;
    object->velocity_z = collision;
    Vector_AddPolarOffset(square, (u16)angle, (struct FieldPosition *)&object->velocity_x);
    if (object->action != 0)
        object->action--;

movement_done:
    work = gMapWork[0];
    rate = WorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
    step = (s16)(rate - work->yaw) / 8;
    if (step > 0x200)
        step = 0x200;
    if (step < -0x200)
        step = -0x200;
    if (step > -16 && step < 16)
        step = rate - work->yaw;
    work->yaw += step;

    if (object->animation_kind == 1) {
        sprite = object->animation;
        collision_kind = GetWorldMapCollision((struct WorldPosition *)&object->x);
        if (collision_kind == 9) {
            ((struct WorldMapSprite *)sprite)->shadow[6] = 1;
            sprite->flags = 0;
        } else {
            ((struct WorldMapSprite *)sprite)->shadow[6] = 9;
            sprite->flags = 1;
        }
        if (collision_kind == 6 && object->action == 0 && collision == 0) {
            effect = FieldObject_Create(24, object->x, object->y, object->z);
            if (effect != 0) {
                animation = effect->animation;
                ObjectDispatch_Initialize((struct DispatchObject *)effect, (u32)(WorldMap_FootprintActions + 12));
                effect->flags = collision;
                effect->terrain_id = 1;
                if (animation != 0) {
                    AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
                    animation->flags = collision;
                    animation->blend_mode = 1;
                    animation->priority = 2;
                }
                object->action = 10;
            }
        }
    }
    Field_CheckConfiguredKeys();
    object->step++;
    return 1;
}

s32 Field_CheckConfiguredKeysAndCount(void *work)
{
    struct ObjectRuntime *object = work;

    Field_CheckConfiguredKeys();
    object->step = (u16)object->step + 1;
    return 1;
}

s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
	s32 Map_GetTerrainHeight(u32 layer, s32 x, s32 z);
    struct FieldPosition pos;
    u8 unused[68];
    struct MapCell *from;
    struct MapCell *to;
    u16 angle;
    s32 motion;
    s32 blocked;
    s32 delta_z;
    s32 height;

    object->speed_limit = 0x8000;
    object->acceleration = 0x4000;
    motion = 12;
    angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
    if (angle == 0xffff) {
        blocked = 4;
    } else {
        motion = 14;
        if ((angle & 0xf000) != 0) {
            motion = 15;
            if ((angle & 0xf000) != 0x8000)
                motion = 10;
        }
        blocked = 0;
        pos.x = 0;
        pos.y = 0;
        pos.z = 0;
        Vector_AddPolarOffset(0x80000, angle, &pos);
        pos.x += object->x;
        delta_z = pos.z;
        if (delta_z < 0)
            object->angle = 0xc000;
        if (delta_z > 0)
            object->angle = 0x4000;
        pos.y = object->y - pos.z;
        pos.z = object->z;
        from = &((struct MapCell *)Ram_MapCellBuffer)
            [object->x / 0x100000 + (pos.z / 0x100000) * 128];
        to = &((struct MapCell *)Ram_MapCellBuffer)
            [pos.x / 0x100000 + (pos.z / 0x100000) * 128];
        if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0
            || from->collision_code != to->collision_code) {
            blocked = 4;
            motion = 12;
        } else if (gKeysHeld & KEY_UP) {
            height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z - 0x100000);
            if (height - object->y < 0x100000)
                goto refused;
        } else if (gKeysHeld & KEY_DOWN) {
            height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z);
            if (height - object->y > -0x80000) {
refused:
                blocked = 1;
                motion = 12;
            }
        }
    }
    if (gEventWork != NULL) {
        if (blocked & 3)
            gEventWork->blocked_steps++;
        else
            gEventWork->blocked_steps = 0;
    }
    ObjectDispatch_ApplyArgumentToChildren(object, motion);
    if (blocked != 0) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
    } else {
        Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    }
    object->step++;
    return 1;
}

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = Runtime_ByteRemapTable;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}
