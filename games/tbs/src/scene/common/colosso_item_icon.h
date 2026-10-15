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
#include "field_obj.h"
#include "field_event.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

void Colosso_ShowItemIcon(s32 id, s32 item)
{
	struct FieldActor	   *obj;
	s32						one;
	struct AnimationObject *spr;
	s32						base;
	s32						tile;
	s32						none;

	obj = Object_GetById(id);
	if (obj != 0)
	{
		one = obj->active;
		if (one == 1)
		{
			spr = (struct AnimationObject *)obj->sprite;
			base = (s32)Runtime_AllocateHeapBlock(17, 0x608);
			none = 0;
			base += 0x400;
			Dma_Fill32(none, (void *)base, 0x80);
			ItemIcon_LoadTilesFar(item);
			tile = Engine_VramLoad(spr->slot, 128, base);
			Runtime_ReleaseHeapBlock(17);
			obj->unknown_5c = one;
			ResourceMetadata_ClearRecord(spr->entries[0]);
			spr->entries[0] = (void *)none;
			spr->count = none;
			spr->part[0].full_color = none;
			spr->part[0].tile = tile;
			spr->dirty = none;
			spr->flags = none;
		}
	}
}
