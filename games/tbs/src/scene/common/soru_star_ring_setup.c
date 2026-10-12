#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

struct SoruRingObject
{
	u8	pad00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	pad14[4];
	s32 scale_x;
	s32 scale_y;
	u8	pad20[24];
	s32 base_x;
	s32 base_y;
	s32 base_z;
	u8	pad44[12];
	u8 *anim;
	u8	pad54;
	u8	state;
};

struct SoruRingEntry
{
	struct SoruRingObject *obj;
	s32					   x;
	s32					   y;
	s32					   z;
	s32					   speed_x;
	s32					   speed_y;
	s32					   speed_z;
	s32					   scale;
	s32					   scale_step;
	u8					   timer;
	u8					   hold;
	u8					   pad26[2];
};

struct SoruRingList
{
	struct SoruRingEntry entries[10];
	u16					 count;
};

static void Soru_UpdateRing(void)
{
	struct SoruRingList	 *list = Runtime_AllocateBlock(33, 404);
	struct SoruRingEntry *entry = list->entries;
	u32					  i;

	i = 0;
	while (i != list->count)
	{
		struct SoruRingObject *obj;
		s32					   speed_x;
		s32					   speed_y;
		s32					   speed_z;
		s32					   scale;
		s32					   step;
		u8					   timer;
		s32					   x;
		s32					   y;
		s32					   z;
		u8					   hold;

		obj = entry->obj;
		speed_x = entry->speed_x;
		speed_y = entry->speed_y;
		speed_z = entry->speed_z;
		scale = entry->scale;
		step = entry->scale_step;
		hold = entry->hold;
		x = entry->x;
		y = entry->y;
		z = entry->z;
		timer = entry->timer;
		if (--timer == 0)
		{
			u32 rx;
			u32 ry;
			u32 rz;
			s32 dx;
			s32 dy;

			timer = 3;
			if (hold == 0)
			{
				scale += step;
				if (scale >= Soru_RingOffsetX[i])
				{
					step = -Soru_RingOffsetZ[i];
				}
				else if (scale <= 0x1999)
				{
					step = Soru_RingOffsetZ[i];
					scale = 0x1999;
					hold = 24;
					x = obj->x;
					y = obj->y;
					z = obj->z;
					obj->x = 0;
					obj->y = 0;
					obj->z = 0;
				}
				obj->scale_x = scale;
				obj->scale_y = scale;
			}
			rx = (Soru_RingDrift[i][0] * (u32)Engine_RandomNext()) >> 16;
			ry = (Soru_RingDrift[i][1] * (u32)Engine_RandomNext()) >> 16;
			rz = (Soru_RingDrift[i][2] * (u32)Engine_RandomNext()) >> 16;
			rx = rx != 0 ? (rx << 16) / 1000 : 0;
			ry = ry != 0 ? (ry << 16) / 1000 : 0;
			rz = rz != 0 ? (rz << 16) / 1000 : 0;
			if (Soru_RingDirection[i][0] == 1)
				speed_x += rx;
			else if (Soru_RingDirection[i][0] == -1)
				speed_x -= rx;
			else
				speed_x = 0;
			if (Soru_RingDirection[i][1] == 1)
				speed_y += ry;
			else if (Soru_RingDirection[i][1] == -1)
				speed_y -= ry;
			else
				speed_y = 0;
			if (Soru_RingDirection[i][2] == 1)
				speed_z += rz;
			else if (Soru_RingDirection[i][2] == -1)
				speed_z -= rz;
			else
				speed_z = 0;
			dx = Engine_MathSin(speed_x * Soru_RingSwing[i][0]) * 2;
			dy = Engine_MathSin(speed_y * Soru_RingSwing[i][1]) * 2;
			{
				s32 dz = Engine_MathCos(speed_z * Soru_RingSwing[i][2]) * 2;

				if (hold != 0)
				{
					x += dx;
					y += dy;
					z += dz;
					if (--hold == 0)
					{
						obj->x = x;
						obj->base_x = x;
						if (ry != 0)
						{
							obj->y = y;
							obj->base_y = y;
						}
						obj->z = z;
						obj->base_z = z;
					}
				}
				else
				{
					obj->x += dx;
					obj->base_x = obj->x;
					if (ry != 0)
					{
						obj->y += dy;
						obj->base_y = obj->y;
					}
					obj->z += dz;
					obj->base_z = obj->z;
				}
			}
		}
		entry->speed_x = speed_x;
		entry->speed_y = speed_y;
		entry->speed_z = speed_z;
		entry->scale = scale;
		entry->scale_step = step;
		entry->hold = hold;
		entry->x = x;
		entry->y = y;
		entry->z = z;
		entry->timer = timer;
		i++;
		entry++;
	}
}

void SoruStar_SetupElementalRings(s32 first, u32 count)
{
	struct SoruRingList	  *list;
	struct SoruRingEntry  *entry;
	struct SoruRingObject *obj;
	u32					   i;
	s32					   none;
	s32					   cleared;
	volatile u32		   zero;

	list = Runtime_AllocateBlock(33, 0x194);
	zero = 0;
	entry = list->entries;
	Dma_Set((const void *)&zero, list, 0x85000065, REG_DMA3);
	if (count > 10)
		count = 10;
	none = 0;
	i = none;
	if (count != 0)
	{
		cleared = none;
		do
		{
			obj = (struct SoruRingObject *)Object_GetById(first);
			{
				u8 *frame = &obj->anim[38];

				entry->obj = obj;
				*frame = cleared;
			}
			obj->state = cleared;
			Engine_ObjectSetBlendMode(Object_GetById(first), 1);
			entry->scale = Soru_RingOffsetX[i];
			entry->scale_step = -Soru_RingOffsetZ[i];
			entry->timer = 3;
			i++;
			entry++;
			first++;
		} while (i != count);
	}
	list->count = count;
	Engine_TaskAddCallback(Soru_UpdateRing, 0xc80);
}
