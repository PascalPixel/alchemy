#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "affine.h"
#include "vram_block.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "tbs_edition.h"
#include "mem.h"
#include "field_event.h"
#include "field_service.h"
#include "dma.h"
#include "resource_ids.h"
#include "call.h"
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"

static __inline__ void Dma_Wait(volatile u32 *dma)
{
	while (dma[2] & 0x80000000)
		;
}

struct Sprite
{
	u32 words[3];
};

extern s16	gColosso_ModeTaskTimer;
extern s16	gColosso_ModeMoveTarget;
extern s16	gColosso_ModeMoveDuration;
extern s16	gColosso_ModeTaskMode;
extern s16	gColosso_ModeTaskParam;
extern s32	gColosso_ModeTaskPosition;
extern s16 *gColosso_ModeTaskScript;

static void Colosso_LoadPortrait(s32 id)
{
	u8 *buf;
	s32 off;

	buf = Runtime_BumpAllocateAlternatePool(0x1c00 + COLOSSO_PALETTE_BYTES);
	if (gColosso_PortraitSlot == -1)
		gColosso_PortraitSlot = Resource_FindFreeEntry();
	off = gColosso_PortraitPaletteOffsets[id];
	if (id == 8)
		id = 4;
	FieldResource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Lettering), buf);
	Dma_Set(buf + off, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
	Vram_Load(gColosso_PortraitSlot, 0x400, (u8 *)((id << 10) + (s32)buf + COLOSSO_PALETTE_BYTES));
	Dma_Wait(REG_DMA3);
	Runtime_BumpFree(buf);
}

static void Colosso_UpdateModeTask(void)
{
	u32					  *write = gColosso_ModeTaskSprites;
	struct Sprite		  *sprite = (struct Sprite *)write;
	s32					   tile = gVramBlockCache[gColosso_PortraitSlot].offset >> 5;
	u32					   flags;
	s32					   scale, blend, pos;
	s32					   matrix, i, x, y, left;
	struct AffineTransform work;

commands:
	if (gColosso_ModeTaskTimer == 0)
	{
		{
			switch (*gColosso_ModeTaskScript++)
			{
			case 0x4000:
				gColosso_ModeTaskPosition = *gColosso_ModeTaskScript++ << 8;
				gColosso_ModeMoveTarget = *gColosso_ModeTaskScript++;
				gColosso_ModeMoveDuration = 0;
				break;
			case 0x3000:
				gColosso_ModeMoveStart = gColosso_ModeMoveTarget;
				gColosso_ModeMoveTarget = *gColosso_ModeTaskScript++;
				gColosso_ModeMoveDuration = *gColosso_ModeTaskScript++;
				gColosso_ModeMoveStep = 0;
				break;
			case 0x1000:
				gColosso_ModeScaleStart = gColosso_ModeScaleTarget;
				gColosso_ModeScaleTarget = *gColosso_ModeTaskScript++;
				gColosso_ModeScaleDuration = *gColosso_ModeTaskScript++;
				gColosso_ModeScaleStep = 0;
				break;
			case 0x2000:
				gColosso_ModeBlendStart = gColosso_ModeBlendTarget;
				gColosso_ModeBlendTarget = *gColosso_ModeTaskScript++;
				gColosso_ModeBlendDuration = *gColosso_ModeTaskScript++;
				gColosso_ModeBlendStep = 0;
				break;
			case 0x7fff:
				gColosso_ModeTaskTimer = *gColosso_ModeTaskScript++;
				break;
			case -1:
				Engine_TaskRemoveCallback(Colosso_UpdateModeTask);
				Resource_ResetEntry(gColosso_PortraitSlot);
				return;
			}
		}
		goto commands;
	}
	gColosso_ModeTaskTimer--;
	if (gColosso_ModeScaleDuration == 0)
	{
		scale = gColosso_ModeScaleTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = gColosso_ModeScaleDuration;
		start = gColosso_ModeScaleStart;
		target = gColosso_ModeScaleTarget;
		progress = ++gColosso_ModeScaleStep;
		scale = start + (target - start) * progress / duration;
		if (progress >= duration)
			gColosso_ModeScaleDuration = 0;
	}
	if (gColosso_ModeBlendDuration == 0)
	{
		blend = gColosso_ModeBlendTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = gColosso_ModeBlendDuration;
		start = gColosso_ModeBlendStart;
		target = gColosso_ModeBlendTarget;
		progress = ++gColosso_ModeBlendStep;
		blend = start + (target - start) * progress / duration;
		if (progress >= duration)
			gColosso_ModeBlendDuration = 0;
	}
	if (gColosso_ModeMoveDuration == 0)
	{
		pos = gColosso_ModeMoveTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = gColosso_ModeMoveDuration;
		start = gColosso_ModeMoveStart;
		target = gColosso_ModeMoveTarget;
		progress = ++gColosso_ModeMoveStep;
		pos = start + (target - start) * progress / duration;
		if (progress >= duration)
			gColosso_ModeMoveDuration = 0;
	}
	work.angle = 0;
	work.scaleX = scale;
	work.scaleY = scale;
	matrix = (s16)AffineMatrix_BuildForEffect(&work);
	gColosso_ModeTaskPosition += pos;
	pos = gColosso_ModeTaskPosition / 256;
	switch (gColosso_ModeTaskMode)
	{
	case 1:
	{
		u32 attr;
		i = 0;
		y = 56;
		flags = 0x80004000;
		attr = matrix << 25;
		for (; i < 4; i++)
		{
			x = pos + scale * (i * 32 - 48) / 256;
			left = x + 88;
			if ((u32)(x + 152) < 304)
			{
				x = left & 511;
				*write++ = 0;
				*write++ = (x << 16) | y | flags | attr | 0x700;
				*write++ = 0xf400 | tile;
				Runtime_PushSlotEntry(sprite++, 236);
			}
			tile += 8;
		}
		break;
	}
	case 3:
	{
		u32 attr;
		i = 0;
		y = 48;
		flags = 0x80004000;
		attr = matrix << 25;
		for (; i < 2; i++)
		{
			x = pos + scale * (i * 32 - 16) / 256;
			left = x + 88;
			if ((u32)(x + 152) < 304)
			{
				x = left & 511;
				*write++ = 0;
				*write++ = (x << 16) | y | flags | attr | 0x700;
				*write++ = 0xf400 | (tile + gColosso_ModeTaskParam);
				Runtime_PushSlotEntry(sprite++, 236);
			}
			tile += 8;
		}
		break;
	}
#if defined(TBS_EDITION_FR)
	case 4:
	{
		u32 attr;
		i = 0;
		y = 64;
		flags = 0x80004000;
		attr = matrix << 25;
		for (; i < 2; i++)
		{
			x = pos + scale * (i * 32 - 16) / 256;
			left = x + 88;
			if ((u32)(x + 152) < 304)
			{
				x = left & 511;
				*write++ = 0;
				*write++ = (x << 16) | y | flags | attr | 0x700;
				*write++ = 0xf400 | (tile + gColosso_ModeTaskParam);
				Runtime_PushSlotEntry(sprite++, 236);
			}
			tile += 8;
		}
		break;
	}
#else
	case 4:
		y = 48;
		flags = 0xc0004000;
		left = pos + 56;
		if ((u32)(pos + 120) < 304)
		{
			x = left & 511;
			*write++ = 0;
			*write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
			*write++ = 0xf400 | (tile + gColosso_ModeTaskParam);
			Runtime_PushSlotEntry(sprite, 236);
		}
		break;
#endif
	case 2:
		y = 48;
		flags = 0x80000000;
		left = pos + 88;
		if ((u32)(pos + 152) < 304)
		{
			x = left & 511;
			*write++ = 0;
			*write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
			*write++ = 0xf400 | (tile + gColosso_ModeTaskParam);
			Runtime_PushSlotEntry(sprite, 236);
		}
		break;
	}
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
