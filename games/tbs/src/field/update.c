#include "unit/field/update.h"
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
extern ScriptCommandFn			  gScriptObject_CommandTable[];

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
				s32 cursor = (s16)obj->scriptCursor;
				u32 cmd = obj->script->word[cursor];

				if (cmd <= 63)
				{
					if (gScriptObject_CommandTable[cmd](obj) == 0)
						break;
				}
				else
				{
					obj->scriptCursor++;
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
			if (obj->targetX != TARGET_UNSET)
			{
				// 目標へ加速
				dx = (obj->targetX - x) / 65536;
				dz = (obj->targetZ - z) / 65536;
				dist = IwramSqrt(dx * dx + dz * dz) << 16;
				if (dist <= 0xffffff)
				{
					dx = obj->targetX - x;
					dz = obj->targetZ - z;
					dist = IwramSqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
				}
				if (dist == 0)
				{
					x = obj->targetX;
					z = obj->targetZ;
				}
				else
				{
					ratio = IwramRatioMulQ14(dist, obj->acceleration);
					vx = obj->velocityX + MulQ16(dx, ratio);
					obj->velocityX = vx;
					dx = obj->velocityZ + MulQ16(dz, ratio);
					obj->velocityZ = dx;
					dist = IwramSqrt(MulQ16(vx, vx) + MulQ16(dx, dx)) << 8;
					if (dist > obj->speedLimit)
					{
						// 最高速度
						ratio = IwramRatioMulQ14(dist, obj->speedLimit);
						obj->velocityX = MulQ16(vx, ratio);
						obj->velocityZ = MulQ16(dx, ratio);
					}
				}
			}
			else
			{
				// 減速
				dx = obj->velocityX;
				dz = obj->velocityZ;
				dist = IwramSqrtFarArm(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
				if (dist != 0)
				{
					if (dist - obj->acceleration < 0)
					{
						obj->velocityX = 0;
						obj->velocityZ = 0;
					}
					else
					{
						ratio = IwramRatioMulQ14FarArm(dist, dist - obj->acceleration);
						obj->velocityX = MulQ16(dx, ratio);
						obj->velocityZ = MulQ16(dz, ratio);
					}
				}
				else
				{
					obj->velocityX = 0;
					obj->velocityZ = 0;
				}
			}

			// 重力と跳ね返り
			if (obj->motionFlags & 2)
			{
				ground = obj->terrainHeight;
				if (y > ground)
					obj->velocityY -= obj->gravity;
				if (y <= ground && obj->velocityY < 0)
				{
					y = ground;
					obj->velocityY = -MulQ16(obj->velocityY, obj->vertical.bounce);
					if ((obj->velocityY < 0 ? -obj->velocityY : obj->velocityY) <= obj->gravity)
						obj->velocityY = 0;
				}
			}
		}

		x += obj->velocityX;
		y += obj->velocityY;
		z += obj->velocityZ;

		// 到着: 目標に着いた、または通り過ぎた
		switch (obj->arrivalAxis)
		{
		case 16:
			if (x == obj->targetX || ((obj->x - obj->targetX) ^ (x - obj->targetX)) < 0)
				arrived = 1;
			break;
		case 17:
			if (y == obj->targetY || ((obj->y - obj->targetY) ^ (y - obj->targetY)) < 0)
				arrived = 1;
			break;
		case 18:
			if (z == obj->targetZ || ((obj->z - obj->targetZ) ^ (z - obj->targetZ)) < 0)
				arrived = 1;
			break;
		}
		if (arrived)
		{
			if (obj->snapToTarget)
			{
				x = obj->targetX;
				obj->velocityX = 0;
				z = obj->targetZ;
				obj->velocityZ = 0;
				if (obj->motionFlags == 0)
				{
					y = obj->targetY;
					obj->velocityY = 0;
				}
			}
			obj->targetX = TARGET_UNSET;
			obj->targetY = TARGET_UNSET;
			obj->targetZ = TARGET_UNSET;
			obj->arrivalAxis = 0;
		}
		obj->x = x;
		obj->y = y;
		obj->z = z;

		// 進む方へ向きを変える
		if (obj->steeringFlags & 1)
		{
			x = obj->velocityX;
			z = obj->velocityZ;
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
