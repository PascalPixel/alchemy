#include "types.h"
#include "irq.h"
#include "iwram_call.h"
#include "mulq16.h"
#include "script_motion.h"

#define OBJECT_SLOTS 8
#define TARGET_UNSET ((s32)0x80000000)
#define TURN_MAX 0x1000 // 1 フレームに回れる角度

// 常駐ルーチンを絶対番地で呼ぶ。RAM へ写したこのコードからも届く。
#define IWRAM_ENTRY(type, offset) ((type)(IwramIrqMain + (offset)))
#define IwramSqrt IWRAM_ENTRY(s32 (*)(s32 value), Iwram_SqrtOffset)
#define IwramRatioMulQ14 IWRAM_ENTRY(s32 (*)(s32 divisor, s32 value), Iwram_RatioMulQ14Offset)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern struct ScriptMotionObject *gObjectSlots;
extern ScriptCommandFn			  ScriptObject_CommandTable[];

// このコードと一緒に写される中継 (updfar.s)
s32 ArcTan2FarArm(s32 z, s32 x);
s32 IwramSqrtFarArm(s32 value);
s32 IwramRatioMulQ14FarArm(s32 divisor, s32 value);

// 物体 8 個の台本と動きを 1 フレーム進める。
void Object_UpdateAll(void)
{
	struct ScriptMotionObject *obj;
	s32						   y;
	s32						   arrived;
	struct ScriptMotionObject *next;
	s32						   n;
	s32						   i;
	s32						   x;
	s32						   z;
	s32						   dx;
	s32						   dz;
	s32						   vx;
	s32						   dist;
	s32						   ratio;
	s32						   ground;
	s32						   turn;
	u16						   angle;

	for (i = 0, obj = gObjectSlots; i < OBJECT_SLOTS; i = n, obj = next)
	{
		n = i + 1;
		next = obj + 1;
		if (obj->script == NULL)
			continue;
		if (obj->hook != NULL)
			obj->hook(obj);
		if (obj->script == NULL)
			continue;
		if (obj->paused != 0)
			continue;

		// 待ち、または台本
		if (obj->wait != 0)
		{
			obj->wait--;
		}
		else
		{
			for (;;)
			{
				s32 cursor = (s16)obj->script_cursor;
				u32 cmd = obj->script->word[cursor];

				if (cmd <= 63)
				{
					if (ScriptObject_CommandTable[cmd](obj) == 0)
						break;
				}
				else
				{
					obj->script_cursor++;
				}
			}
			if (obj->script == NULL)
				continue;
		}

		x = obj->x;
		y = obj->y;
		z = obj->z;
		if (obj->frozen == 0)
		{
			arrived = 0;
			if (obj->target_x != TARGET_UNSET)
			{
				// 目標へ加速
				dx = (obj->target_x - x) / 65536;
				dz = (obj->target_z - z) / 65536;
				dist = IwramSqrt(dx * dx + dz * dz) << 16;
				if (dist <= 0xffffff)
				{
					dx = obj->target_x - x;
					dz = obj->target_z - z;
					dist = IwramSqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
				}
				if (dist == 0)
				{
					x = obj->target_x;
					z = obj->target_z;
				}
				else
				{
					ratio = IwramRatioMulQ14(dist, obj->acceleration);
					vx = obj->velocity_x + MulQ16(dx, ratio);
					obj->velocity_x = vx;
					dx = obj->velocity_z + MulQ16(dz, ratio);
					obj->velocity_z = dx;
					dist = IwramSqrt(MulQ16(vx, vx) + MulQ16(dx, dx)) << 8;
					if (dist > obj->speed_limit)
					{
						// 最高速度
						ratio = IwramRatioMulQ14(dist, obj->speed_limit);
						obj->velocity_x = MulQ16(vx, ratio);
						obj->velocity_z = MulQ16(dx, ratio);
					}
				}
			}
			else
			{
				// 減速
				dx = obj->velocity_x;
				dz = obj->velocity_z;
				dist = IwramSqrtFarArm(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
				if (dist != 0)
				{
					if (dist - obj->acceleration < 0)
					{
						obj->velocity_x = 0;
						obj->velocity_z = 0;
					}
					else
					{
						ratio = IwramRatioMulQ14FarArm(dist, dist - obj->acceleration);
						obj->velocity_x = MulQ16(dx, ratio);
						obj->velocity_z = MulQ16(dz, ratio);
					}
				}
				else
				{
					obj->velocity_x = 0;
					obj->velocity_z = 0;
				}
			}

			// 重力と跳ね返り
			if (obj->motion_flags & 2)
			{
				ground = obj->terrain_height;
				if (y > ground)
					obj->velocity_y -= obj->gravity;
				if (y <= ground && obj->velocity_y < 0)
				{
					y = ground;
					obj->velocity_y = -MulQ16(obj->velocity_y, obj->vertical.bounce);
					if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
						obj->velocity_y = 0;
				}
			}
		}

		x += obj->velocity_x;
		y += obj->velocity_y;
		z += obj->velocity_z;

		// 到着: 目標に着いた、または通り過ぎた
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
				x = obj->target_x;
				obj->velocity_x = 0;
				z = obj->target_z;
				obj->velocity_z = 0;
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

		// 進む方へ向きを変える
		if (obj->steering_flags & 1)
		{
			x = obj->velocity_x;
			z = obj->velocity_z;
			if (x != 0 || z != 0)
			{
				angle = ArcTan2FarArm(z, x);
				turn = (s16)(angle - obj->facing);
				if (turn > TURN_MAX)
					turn = TURN_MAX;
				if (turn < -TURN_MAX)
					turn = -TURN_MAX;
				obj->facing += turn;
			}
		}
	}
}
