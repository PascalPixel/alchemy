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

extern s16	Colosso_ModeTaskTimer;
extern s16	Colosso_ModeMoveTarget;
extern s16	Colosso_ModeMoveDuration;
extern s16	Colosso_ModeTaskMode;
extern s16	Colosso_ModeTaskParam;
extern s32	Colosso_ModeTaskPosition;
extern s16 *Colosso_ModeTaskScript;

static void Colosso_LoadPortrait(s32 id)
{
	u8 *buf;
	s32 off;

	buf = Runtime_BumpAllocateAlternatePool(0x1c00 + COLOSSO_PALETTE_BYTES);
	if (Colosso_PortraitSlot == -1)
		Colosso_PortraitSlot = Resource_FindFreeEntry();
	off = Colosso_PortraitPaletteOffsets[id];
	if (id == 8)
		id = 4;
	FieldResource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Lettering), buf);
	Dma_Set(buf + off, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_ENABLE32 | DMA_32BIT | 8, REG_DMA3);
	Vram_Load(Colosso_PortraitSlot, 0x400, (u8 *)((id << 10) + (s32)buf + COLOSSO_PALETTE_BYTES));
	Dma_Wait(REG_DMA3);
	Runtime_BumpFree(buf);
}

static void Colosso_UpdateModeTask(void)
{
	u32					  *write = Colosso_ModeTaskSprites;
	struct Sprite		  *sprite = (struct Sprite *)write;
	s32					   tile = gVramBlockCache[Colosso_PortraitSlot].offset >> 5;
	u32					   flags;
	s32					   scale, blend, pos;
	s32					   matrix, i, x, y, left;
	struct AffineTransform work;

commands:
	if (Colosso_ModeTaskTimer == 0)
	{
		{
			switch (*Colosso_ModeTaskScript++)
			{
			case 0x4000:
				Colosso_ModeTaskPosition = *Colosso_ModeTaskScript++ << 8;
				Colosso_ModeMoveTarget = *Colosso_ModeTaskScript++;
				Colosso_ModeMoveDuration = 0;
				break;
			case 0x3000:
				Colosso_ModeMoveStart = Colosso_ModeMoveTarget;
				Colosso_ModeMoveTarget = *Colosso_ModeTaskScript++;
				Colosso_ModeMoveDuration = *Colosso_ModeTaskScript++;
				Colosso_ModeMoveStep = 0;
				break;
			case 0x1000:
				Colosso_ModeScaleStart = Colosso_ModeScaleTarget;
				Colosso_ModeScaleTarget = *Colosso_ModeTaskScript++;
				Colosso_ModeScaleDuration = *Colosso_ModeTaskScript++;
				Colosso_ModeScaleStep = 0;
				break;
			case 0x2000:
				Colosso_ModeBlendStart = Colosso_ModeBlendTarget;
				Colosso_ModeBlendTarget = *Colosso_ModeTaskScript++;
				Colosso_ModeBlendDuration = *Colosso_ModeTaskScript++;
				Colosso_ModeBlendStep = 0;
				break;
			case 0x7fff:
				Colosso_ModeTaskTimer = *Colosso_ModeTaskScript++;
				break;
			case -1:
				Engine_TaskRemoveCallback(Colosso_UpdateModeTask);
				Resource_ResetEntry(Colosso_PortraitSlot);
				return;
			}
		}
		goto commands;
	}
	Colosso_ModeTaskTimer--;
	if (Colosso_ModeScaleDuration == 0)
	{
		scale = Colosso_ModeScaleTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = Colosso_ModeScaleDuration;
		start = Colosso_ModeScaleStart;
		target = Colosso_ModeScaleTarget;
		progress = ++Colosso_ModeScaleStep;
		scale = start + (target - start) * progress / duration;
		if (progress >= duration)
			Colosso_ModeScaleDuration = 0;
	}
	if (Colosso_ModeBlendDuration == 0)
	{
		blend = Colosso_ModeBlendTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = Colosso_ModeBlendDuration;
		start = Colosso_ModeBlendStart;
		target = Colosso_ModeBlendTarget;
		progress = ++Colosso_ModeBlendStep;
		blend = start + (target - start) * progress / duration;
		if (progress >= duration)
			Colosso_ModeBlendDuration = 0;
	}
	if (Colosso_ModeMoveDuration == 0)
	{
		pos = Colosso_ModeMoveTarget;
	}
	else
	{
		s32 duration, start, progress, target;
		duration = Colosso_ModeMoveDuration;
		start = Colosso_ModeMoveStart;
		target = Colosso_ModeMoveTarget;
		progress = ++Colosso_ModeMoveStep;
		pos = start + (target - start) * progress / duration;
		if (progress >= duration)
			Colosso_ModeMoveDuration = 0;
	}
	work.angle = 0;
	work.scaleX = scale;
	work.scaleY = scale;
	matrix = (s16)AffineMatrix_BuildForEffect(&work);
	Colosso_ModeTaskPosition += pos;
	pos = Colosso_ModeTaskPosition / 256;
	switch (Colosso_ModeTaskMode)
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
				*write++ = 0xf400 | (tile + Colosso_ModeTaskParam);
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
				*write++ = 0xf400 | (tile + Colosso_ModeTaskParam);
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
			*write++ = 0xf400 | (tile + Colosso_ModeTaskParam);
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
			*write++ = 0xf400 | (tile + Colosso_ModeTaskParam);
			Runtime_PushSlotEntry(sprite, 236);
		}
		break;
	}
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
