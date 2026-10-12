#include "unit/field/common/fielddat.h"
#include "unit/field/common/object/objdata.h"
#include "unit/graphics/render/facing.h"
#include "unit/graphics/tile/griddata.h"
#include "unit/lib/curvedat.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/field/common/object/update.h"
#include "unit/graphics/tile/vertices.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

//--------------------------------------------------------------------------
// スクリプトの読む位置を移し、1 を返して次の命令を続けて実行する
//--------------------------------------------------------------------------
s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc)
{
	s32 zero;

	// 命令の次の語を新しい先頭にする
	sc->script = sc->script + sc->cursor + 1;
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
	while (cnt <= 0x257 && Object_IsTargetUnset((struct ObjectRuntime *)arg0) == 0)
	{
		WaitFrames(1);
		cnt++;
	}
}

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
	s32 first;
	s32 second;

	if (object->flags == 0)
	{
		second = object->target_x;
		if (second != 0x80000000)
			return 0;
		first = object->target_y;
	}
	else
	{
		first = object->target_x;
		second = 0x80000000;
	}
	if (first != second || object->target_z != first)
		return 0;
	return 1;
}

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern ScriptCommandFn ScriptObject_CommandTable[];

static __inline__ s32 Square(s32 value)
{
	return value * value;
}

void Object_UpdateAllThumb(void)
{
	struct ScriptMotionObject	   *obj;
	const struct ScriptMotionWords *script;
	s32								cnt;
	s32								px;
	s32								py;
	s32								pz;
	s32								ground;
	s32								reached;
	s32								pos[3];
	s32								dx;
	s32								dy;
	s32								dz;
	s32								vx;
	s32								vy;
	s32								vz;
	s32								dist;
	s32								len;
	s32								ratio;
	s32								rem;
	s32								half;
	s32								work;
	u32								cmd;
	u32								phase;

	obj = gObjectSlots;
	for (cnt = 63; cnt >= 0; cnt--, obj++)
	{
		reached = 0;

		script = obj->script;
		if (script == NULL)
			continue;

		if (obj->hook != NULL)
		{
			obj->hook(obj);
			script = obj->script;
		}
		if (script == NULL)
			continue;
		if (obj->paused != 0)
			continue;

		if (obj->wait != 0)
		{
			obj->wait--;
		}
		else
		{
			for (;;)
			{
				cmd = script->word[(s16)obj->script_cursor];
				if (cmd <= 63)
				{
					if (ScriptObject_CommandTable[cmd](obj) == 0)
						break;
				}
				else
				{
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

		if (obj->frozen == 0)
		{
			if (obj->motion_flags == 0)
			{
				if (obj->target_x != TARGET_UNSET)
				{
					dx = (obj->target_x - px) / 65536;
					dy = (obj->target_y - py) / 65536;
					dz = (obj->target_z - pz) / 65536;
					dist = Iwram_Sqrt(Square(dx) + Square(dy) + Square(dz));
					if (dist == 0)
					{
						px = obj->target_x;
						py = obj->target_y;
						pz = obj->target_z;
					}
					else
					{
						ratio = Iwram_RatioMulQ14(dist << 16, obj->acceleration);
						obj->velocity_x += dx * ratio;
						obj->velocity_y += dy * ratio;
						obj->velocity_z += dz * ratio;
						vx = obj->velocity_x;
						vy = obj->velocity_y;
						vz = obj->velocity_z;
						len = FixedSqrt(
							Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vy, vy) + Iwram_MulQ16(vz, vz));
						if (len > obj->speed_limit)
						{
							ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
							obj->velocity_x = Iwram_MulQ16(vx, ratio);
							obj->velocity_y = Iwram_MulQ16(vy, ratio);
							obj->velocity_z = Iwram_MulQ16(vz, ratio);
						}
					}
				}
				else
				{
					dx = obj->velocity_x;
					dy = obj->velocity_y;
					dz = obj->velocity_z;
					len = FixedSqrt(
						Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
					if (len != 0)
					{
						rem = len - obj->acceleration;
						if (rem < 0)
							rem = 0;
						ratio = Iwram_RatioMulQ14(len, rem);
						obj->velocity_x = Iwram_MulQ16(dx, ratio);
						obj->velocity_y = Iwram_MulQ16(dy, ratio);
						obj->velocity_z = Iwram_MulQ16(dz, ratio);
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_y = 0;
						obj->velocity_z = 0;
					}
				}
			}
			else
			{
				if (obj->target_x != TARGET_UNSET)
				{
					dx = (obj->target_x - px) / 65536;
					dz = (obj->target_z - pz) / 65536;
					dist = Iwram_Sqrt(Square(dx) + Square(dz)) << 16;
					if (dist <= 0x00ffffff)
					{
						dx = obj->target_x - px;
						dz = obj->target_z - pz;
						dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
					}
					if (dist == 0)
					{
						px = obj->target_x;
						pz = obj->target_z;
					}
					else
					{
						ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
						obj->velocity_x += Iwram_MulQ16(dx, ratio);
						obj->velocity_z += Iwram_MulQ16(dz, ratio);
						vx = obj->velocity_x;
						vz = obj->velocity_z;
						len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vz, vz));
						if (len > obj->speed_limit)
						{
							ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
							obj->velocity_x = Iwram_MulQ16(vx, ratio);
							obj->velocity_z = Iwram_MulQ16(vz, ratio);
						}
					}
				}
				else
				{
					dx = obj->velocity_x;
					dz = obj->velocity_z;
					len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
					if (len != 0)
					{
						rem = len - obj->acceleration;
						if (rem < 0)
							rem = 0;
						ratio = Iwram_RatioMulQ14(len, rem);
						obj->velocity_x = Iwram_MulQ16(dx, ratio);
						obj->velocity_z = Iwram_MulQ16(dz, ratio);
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_z = 0;
					}
				}

				if ((obj->motion_flags & 1) != 0)
				{
					vx = px + obj->velocity_x;
					vz = pz + obj->velocity_z;
					ground = Map_GetTerrainHeight(obj->terrain_id, vx, vz);
					vy = ground - obj->terrain_height;
					if (ground - py > -0x40000)
						py += vy;
					if (vy < 0)
						vy = -vy;
					half = obj->acceleration / 2;
					if (vy > half)
						vy = half;
					vy *= 3;
					if (vy != 0 && (obj->motion_flags & 0x10) == 0)
					{
						dx = obj->velocity_x;
						dy = obj->velocity_y;
						dz = obj->velocity_z;
						len = FixedSqrt(
							Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
						if (len != 0)
						{
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

				if ((obj->motion_flags & 2) != 0)
				{
					ground = obj->terrain_height;
					if (py > ground)
					{
						obj->velocity_y -= obj->gravity;
					}
					else if (obj->velocity_y < 0)
					{
						py = ground;
						obj->velocity_y = -Iwram_MulQ16(obj->velocity_y, obj->vertical.bounce);
						if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y)
							<= obj->gravity)
							obj->velocity_y = 0;
					}
				}

				if ((obj->motion_flags & 4) != 0)
				{
					phase = (obj->vertical.phase & 0x3f);
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

		if ((obj->collision_flags & 0x80) != 0)
		{
			pos[0] = px;
			pos[1] = py;
			pos[2] = pz;
			if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)obj, pos) != 0)
			{
				obj->refused++;
				continue;
			}
			obj->refused = 0;
		}

		switch (obj->arrival_axis)
		{
		case 16:
			if (px == obj->target_x || ((obj->x - obj->target_x) ^ (px - obj->target_x)) < 0)
				reached = 1;
			break;
		case 17:
			if (py == obj->target_y || ((obj->y - obj->target_y) ^ (py - obj->target_y)) < 0)
				reached = 1;
			break;
		case 18:
			if (pz == obj->target_z || ((obj->z - obj->target_z) ^ (pz - obj->target_z)) < 0)
				reached = 1;
			break;
		default:
			break;
		}

		if (reached != 0)
		{
			if (obj->snap_to_target != 0)
			{
				px = obj->target_x;
				obj->velocity_x = 0;
				pz = obj->target_z;
				obj->velocity_z = 0;
				if (obj->motion_flags == 0)
				{
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

		if ((obj->steering_flags & 1) != 0)
		{
			px = obj->velocity_x;
			pz = obj->velocity_z;
			if (px != 0 || pz != 0)
			{
				s32 limit;

				work = (s16)(ArcTan2(pz, px) - obj->facing);
				limit = 0x1000;
				if (work > limit)
					work = limit;
				limit = -0x1000;
				if (work < limit)
					work = limit;
				obj->facing += work;
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
	if (dist < 0x100000)
	{
		dx = x - object->x;
		dist = y - object->y;
		dz = z - object->z;
		dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) + Iwram_MulQ16(dz, dz));
	}
	if (dist < 0x10000)
	{
		object->x = x;
		object->y = y;
		object->z = z;
		object->target_x = 0x80000000;
		object->target_y = 0x80000000;
		object->target_z = 0x80000000;
		return;
	}
	if (object->snap_to_target == 0)
	{
		s32 brake;

		brake = Iwram_RatioMulQ14(
			object->acceleration, Iwram_MulQ16(object->speed_limit, object->speed_limit));
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
	if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz))
	{
		*axis = 18;
		dx = dz;
	}
	if (object->flags == 0)
	{
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
	s32						   cnt;
	s32						   y;
	s32						   arrived;
	s32						   x;
	s32						   z;
	s32						   dx;
	s32						   dz;
	s32						   vx;
	s32						   dist;
	s32						   ratio;
	s32						   turn;

	obj = gObjectSlots;
	for (cnt = 13; cnt >= 0; cnt--, obj++)
	{
		if (obj->script == 0)
			continue;
		x = obj->x;
		y = obj->y;
		z = obj->z;
		if (obj->frozen == 0)
		{
			arrived = 0;
			if (obj->target_x != TARGET_UNSET)
			{
				dx = (obj->target_x - x) / 65536;
				dz = (obj->target_z - z) / 65536;
				dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
				if (dist <= 0xffffff)
				{
					dx = obj->target_x - x;
					dz = obj->target_z - z;
					dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
				}
				if (dist == 0)
				{
					x = obj->target_x;
					z = obj->target_z;
				}
				else
				{
					ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
					vx = obj->velocity_x + Iwram_MulQ16(dx, ratio);
					obj->velocity_x = vx;
					dx = obj->velocity_z + Iwram_MulQ16(dz, ratio);
					obj->velocity_z = dx;
					dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
					if (dist > obj->speed_limit)
					{
						ratio = Iwram_RatioMulQ14(dist, obj->speed_limit);
						obj->velocity_x = Iwram_MulQ16(vx, ratio);
						obj->velocity_z = Iwram_MulQ16(dx, ratio);
					}
				}
			}
			else
			{
				dx = obj->velocity_x;
				dz = obj->velocity_z;
				if ((dx | dz) != 0)
				{
					dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
					if (dist != 0)
					{
						if (dist - obj->acceleration < 0)
						{
							obj->velocity_x = 0;
							obj->velocity_z = 0;
						}
						else
						{
							ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
							obj->velocity_x = Iwram_MulQ16(dx, ratio);
							obj->velocity_z = Iwram_MulQ16(dz, ratio);
						}
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_z = 0;
					}
				}
			}
			if (obj->motion_flags & 2)
			{
				if (y > obj->terrain_height)
				{
					obj->velocity_y -= obj->gravity;
				}
				else if (obj->velocity_y < 0)
				{
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
		if (obj->arrival_axis != 0)
			switch (obj->arrival_axis)
			{
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
		if (arrived)
		{
			if (obj->snap_to_target)
			{
				obj->velocity_x = 0;
				obj->velocity_z = 0;
				x = obj->target_x;
				z = obj->target_z;
				if (obj->motion_flags == 0)
				{
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
		if (obj->steering_flags & 1)
		{
			x = obj->velocity_x;
			z = obj->velocity_z;
			if (x != 0 || z != 0)
			{
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
	interpreter->cursor += 2;
	return 0;
}

s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
	if (interpreter->delay > 0x3B)
	{
		interpreter->delay = 0;
		interpreter->cursor++;
		return 1;
	}
	if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0)
	{
		interpreter->cursor++;
		return 1;
	}
	return 0;
}

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
	s16			  initial = interpreter->cursor;
	ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

	if (callback(interpreter) != 0)
		return 0;
	if (interpreter->cursor == initial)
		interpreter->cursor = interpreter->cursor + 2;
	return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
	u32 *entries;
	s32	 index;
	s16 *field = &interpreter->lookup_result;
	s32	 zero = 0;

	*field = zero;
	if (key == 0)
	{
		return 0;
	}

	key &= 0xBFFFFFFF;
	entries = (u32 *)interpreter->script;
	index = 0;
	do
	{
		if (*entries++ == key)
		{
			return index + 1;
		}
		index++;
	} while (index <= 0x3FF);
	return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
	const s32 *arguments;
	s32		   repeat_limit;
	s32		   jump_key;
	u8		  *repeat_count;
	s32		   next_count;

	arguments = &interpreter->script[interpreter->cursor + 1];
	repeat_limit = *arguments++;
	jump_key = *arguments;
	if (repeat_limit == 0xFFFF)
	{
		interpreter->cursor = Script_FindLabel(interpreter, jump_key);
	}
	else
	{
		repeat_count = &interpreter->repeat_count;
		next_count = *repeat_count + 1;
		*repeat_count = next_count;
		if ((u8)next_count < (s16)repeat_limit)
		{
			interpreter->cursor = Script_FindLabel(interpreter, jump_key);
		}
		else
		{
			*repeat_count = 0;
			interpreter->cursor += 3;
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
	s32		   index = interpreter->cursor;
	const s32 *table = interpreter->script;
	u32		   value = table[index + 1];

	if (interpreter->condition_result != 0)
	{
		interpreter->cursor = Script_FindLabel(interpreter, value);
	}
	else
	{
		interpreter->cursor += 2;
	}
	return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
	s32		   index = interpreter->cursor;
	const s32 *table = interpreter->script;
	u32		   value = table[index + 1];

	if (interpreter->condition_result == 0)
	{
		interpreter->cursor = Script_FindLabel(interpreter, value);
	}
	else
	{
		interpreter->cursor += 2;
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
	interpreter->condition_result = GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

// フラグの今の値を condition_result に入れてから、change でフラグを書き換える
#define DEFINE_FLAG_AND_TEST(name, change)                       \
	s32 name(struct ScriptInterpreter *interpreter)              \
	{                                                            \
		s32 value;                                               \
                                                                 \
		value = interpreter->script[interpreter->cursor + 1];    \
		interpreter->condition_result = GameFlag_TestFar(value); \
		change(value);                                           \
		interpreter->cursor += 2;                                \
		return 1;                                                \
	}

DEFINE_FLAG_AND_TEST(Script_SetFlagAndTest, GameFlag_SetBitFar)
DEFINE_FLAG_AND_TEST(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter)
{
	s32 value;
	s32 result;

	value = interpreter->script[interpreter->cursor + 1];
	result = GameFlag_TestFar(value);
	interpreter->condition_result = result;
	if ((result << 0x18) == 0x01000000)
	{
		GameFlag_ClearBitFar(value);
	}
	else
	{
		GameFlag_SetBitFar(value);
	}
	interpreter->cursor += 2;
	return 1;
}

s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter)
{
	ObjectDispatch_ApplyArgumentToChildren(
		interpreter, interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter)
{
	ObjectDispatch_Release((struct DispatchObject *)interpreter);
	return 0;
}

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
	interpreter->cursor += 2;
	return 1;
}

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
	AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
	s32					  index;
	u8					 *flags;
	struct ObjectRuntime *entry = gObjectSlots;

	flags = &entry->unknown_59;
	for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry))
	{
		if (entry->script != NULL && (1 & *flags) && entry != (struct ObjectRuntime *)object)
		{
			if (Runtime_CheckRadiusOverlap(&entry->x, ((struct FieldActor *)entry)->radius - 2,
					values, ((struct FieldActor *)object)->radius - 2)
				>= 0)
				return -1;
		}
	}
	return 0;
}

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
	struct ScriptObjectEntry *object, s32 *values)
{
	s32					  index;
	u8					 *flags;
	struct ObjectRuntime *entry = gObjectSlots;

	flags = &entry->unknown_59;
	for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry))
	{
		if (entry->script != NULL && (1 & *flags) && entry != (struct ObjectRuntime *)object)
		{
			if (Runtime_CheckRadiusOverlap(&entry->x, ((struct FieldActor *)entry)->radius - 2,
					values, ((struct FieldActor *)object)->radius - 2)
				>= 0)
				return (struct ScriptObjectEntry *)entry;
		}
	}
	return NULL;
}

// 命令語の次の 3 語を座標として place にわたす
static __inline__ s32 ApplyPosition(struct ScriptInterpreter *interpreter,
	void (*place)(struct ObjectRuntime *object, s32 x, s32 y, s32 z))
{
	s32		   first;
	s32		   second;
	s32		   third;
	const s32 *argument;

	argument = interpreter->script + interpreter->cursor;
	argument++;
	first = *argument;
	argument++;
	second = *argument;
	argument++;
	third = *argument;
	place((struct ObjectRuntime *)interpreter, first, second, third);
	interpreter->cursor += 4;
	return 1;
}

s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter)
{
	return ApplyPosition(interpreter, Object_SetPositionAndResetMotion);
}

s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter)
{
	return ApplyPosition(interpreter, Object_SetMoveTarget);
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
	u8	*entry = (u8 *)(object->script + (s16)object->script_cursor);
	s32 *cursor = (s32 *)(entry + 4);
	s32	 first = *cursor++;
	s32	 second = *cursor++;
	s32	 third = *cursor;

	Object_SetMoveTarget(
		(struct ObjectRuntime *)object, object->x + first, object->y + second, object->z + third);
	object->script_cursor += 4;
	return 1;
}

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
	struct ScriptObjectRuntime *target;

	target = object->linked_object;
	object->script_value = ArcTan2((target->z - object->z), (target->x - object->x));
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

s32 Script_ApplyLocalOffsetPosition(struct ScriptObjectRuntime *object)
{
	s32 offset[3];

	Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + offset[0],
		object->y + offset[1], object->z + offset[2]);
	object->script_cursor += 3;
	return 1;
}

struct MapScrollBounds
{
	u8	unknown_00[0xec];
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
};

s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj)
{
	struct ObjectRuntime   *leader;
	struct MapScrollBounds *bounds;
	s32						min_x;
	s32						min_z;
	s32						max_x;
	s32						max_z;
	s32						x;
	s32						y;
	s32						z;
	s32						dz;
	s32						dy;
	s32						dx;
	s32						dist;
	s32						step;

	bounds = gMapWork[0];
	leader = obj->linked_object;
	min_x = bounds->left + 0x780000;
	min_z = bounds->top + leader->y + 0x600000;
	max_x = bounds->right - 0x780000;
	max_z = bounds->bottom + leader->y - 0x400000;
	obj->flags = 0;
	if (leader != NULL && leader->script != NULL)
	{
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
		if (obj->action != 0)
		{
			obj->x = x;
			obj->y = y;
			obj->z = z;
		}
		else
		{
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
			if (dist < 0x4000)
			{
				obj->x = x;
				obj->z = z;
			}
			else
			{
				if (dist > step)
				{
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
s32		  _call_via_r3(s32, s32, s32, s32);

s32 Object_ApproachLinkedObject(struct ObjectRuntime *object)
{
	s32					  len;
	s32					  mz;
	s32					  dx;
	s32					  dz;
	s32					  dzh;
	s32					  dxh;
	s32					  n;
	s32					  mx;
	s32					  dx2;
	struct ObjectRuntime *link;

	link = object->linked_object;
	object->speed_limit = link->speed_limit;
	object->acceleration = link->acceleration;
	dx = link->x - object->x;
	dz = link->z - object->z;
	dxh = dx >> 0x10;
	dzh = dz >> 0x10;
	len = _call_via_r3((dxh * dxh) + (dzh * dzh), dx, dzh, (u32)IwramSqrt);
	if (len > 0x10)
	{
		dx2 = dx;
		n = len - 0x10;
		mx = dx2 * n / len;
		mz = dz * n / len;
		Object_SetMoveTarget(object, object->x + mx, object->y, object->z + mz);
		ObjectDispatch_ApplyArgumentToChildren(object, 2);
		object->step = (object->step + 1);
		return 1;
	}
	ObjectDispatch_ApplyArgumentToChildren(object, 1);
	return 0;
}

s32 Object_Wander(struct ScriptObjectRuntime *object)
{
	struct FieldPosition pos;
	struct FieldPosition probe;
	s32					 base;
	const s32			*args;
	s32					 range;
	s32					 limit;
	s32					 radius;
	s32					 heading;
	s32					 tries;
	s32					 dx;
	s32					 dz;

	args = &object->script[(s16)object->script_cursor + 1];
	base = *args++;
	range = *args++;
	limit = *args / 0x10000;
	/* FAKEMATCH: needed to match: the retry label and one-pass distance block preserve the 32-byte frame and division/load registers. */
	tries = 0;
	limit = limit * limit;
retry:
	tries++;
	if (tries <= 7)
	{
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
		do
		{
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
	s32					 base;
	const s32			*args;
	s32					 range;
	s32					 limit;
	s32					 radius;
	u16					 heading;
	u16					 angle;
	s32					 tries;
	s32					 dx;
	s32					 dz;

	args = &object->script[(s16)object->script_cursor + 1];
	base = *args++;
	range = *args++;
	limit = *args / 0x10000;
	angle = object->script_value;
	limit *= limit;
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

enum ScriptOperandOperation
{
	SCRIPT_OPERAND_SET,
	SCRIPT_OPERAND_ADD,
	SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Script_OperandHandlerTable[];

// 作業領域の 1 項目を命令の値で書き換える: 代入、加算、または比較 (結果は comparison_result)
// 32 ビットの項目
#define DEFINE_OPERAND_WORD(name, field)                              \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value;                                    \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// アドレスの項目: 加算は value の 4 倍 (4 バイト単位)
#define DEFINE_OPERAND_ADDRESS(name, field)                           \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value * 4;                                \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// 8 ビット、16 ビットの項目: 比較は項目の型に切り詰めた値と行う
#define DEFINE_OPERAND_NARROW(name, field, type)                      \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
		}                                                             \
		else if (operation == SCRIPT_OPERAND_ADD)                     \
		{                                                             \
			state->field += value;                                    \
		}                                                             \
		else                                                          \
		{                                                             \
			state->comparison_result = state->field == (type)value;   \
		}                                                             \
	}

DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, script_address)

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->cursor = value;
	}
	else if (operation == SCRIPT_OPERAND_ADD)
	{
		state->cursor = ((s16)state->cursor + value);
	}
	else
	{
		state->comparison_result = (s16)state->cursor == (s16)value;
	}
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword, unsigned_halfword, u16)
DEFINE_OPERAND_WORD(Script_SetOrCompareX, word_08)
DEFINE_OPERAND_WORD(Script_SetOrCompareY, word_0c)
DEFINE_OPERAND_WORD(Script_SetOrCompareZ, word_10)

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->halfword_20 = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		state->halfword_20 = (state->halfword_20 + value);
		return;
	}
	state->comparison_result = state->halfword_20 == (s16)value;
}

DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, word_18)
DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, word_1c)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, word_24)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, word_28)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, word_2c)
DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, word_30)
DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, word_34)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, word_38)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, word_3c)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, word_40)
DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, word_44)
DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, word_48)
DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, word_14)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, word_4c)
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, address_50)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, byte_54, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, byte_55, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, byte_56, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult, comparison_result, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, byte_58, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, byte_59, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, byte_5a, u8)
DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, byte_5b, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, byte_5d, u8)

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		work->halfword_5e = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		work->halfword_5e += value;
		return;
	}
	work->comparison_result = (s16)work->halfword_5e == (s16)value;
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64, signed_halfword_64, s16)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66, signed_halfword_66, s16)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, word_68)
DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, word_6c)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, byte_62, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, byte_63, u8)

// 命令語の次の 2 語 (項目の番号と値) を読み、項目の関数に operation をわたす
static __inline__ s32 ApplyOperand(struct ScriptOperands *work, s32 operation)
{
	s16			index = work->cursor;
	const s32  *command = (const s32 *)work->script_address + index + 1;
	OperandFunc handler = Script_OperandHandlerTable[command[0]];

	if (handler != 0)
		handler(work, operation, command[1]);
	work->cursor += 3;
	return 1;
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_SET);
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_ADD);
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_COMPARE);
}
