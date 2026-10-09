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
#include "battle_status_icon.h"
#include "iwram_call.h"
#include "motobj.h"
#include "animspr.h"
#include "metadata_lookup.h"

#define BattlePres_SetActorModeAndAction ((void (*)(s32))BattlePres_SetActorModeAndAction)

#define gMenuCtrlWork (*(u8 **)&gMenuCtrlWork)

struct PairedObjectList
{
	u8						padding0[8];
	struct AnimationObject *objects[1];
};

struct PairedObjectTable
{
	u8	padding0[0x18];
	s32 count;
};

extern volatile u8 gSchedulerStatus;

void WaitFrames(s32 frames);

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
	s32						 i;
	s32						 id;
	struct BtlObj			*slot;
	struct MotionObject		*object;
	struct AnimationObject	*res;
	struct SpriteEntry		*entry;
	struct AnimationObject **objects;
	u8						*table;
	s32						 resource;
	s32						 object_id;

	for (i = 0; i <= 13; i++)
	{
		if (!BattlePlacement_ContainsId(list, i))
		{
			object_id = i + 120;
			if (i <= 7)
				object_id = i;
			ReleaseBattleObjectRecords(object_id);
		}
	}
	if (gSchedulerStatus == 0)
		WaitFrames(1);
	for (i = 0; i <= 13 && (id = list[i]) != 255; i++)
	{
		if (id == 254)
			continue;
		slot = BtlObjGet(id);
		if (slot == 0)
			continue;
		BattleUnit_BuildStatusFlags(id, slot);
		object = slot->object;
		if (object == 0)
			continue;
		if (object->record_storage_kind != 0)
			continue;
		if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483)
		{
			table = gMenuCtrlWork + ((struct PairedObjectTable *)gMenuCtrlWork)->count * 4;
			objects = ((struct PairedObjectList *)table)->objects;
			resource = slot->resource;
			object->record_storage_kind = 2;
			object->records = objects;
			Iwram_ClearWords(objects, 16);
			res = ResourceObject_CreateFar(resource);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				object->height = Resource_GetMetadataRecordFar(resource)->box_y >> 1;
				*objects = res;
				objects = &((struct PairedObjectList *)table)->objects[1];
			}
			res->flags = 0;
			res = ResourceObject_CreateFar(resource + 0x2001);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				*objects = res;
			}
			res->flags = 0;
		}
		else
		{
			res = ResourceObject_CreateFar(slot->resource);
			if (res != 0)
			{
				object->record_storage_kind = 1;
				object->records = res;
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				entry = (struct SpriteEntry *)res->entries[0];
				entry->priority = 1;
				entry->palette = slot->palette;
				if ((resource = slot->overlay) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					entry->priority = 1;
				}
				if ((resource = slot->animation) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->animation_entry = entry;
					Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
					entry->priority = 3;
				}
				if ((resource = slot->effect) != 0)
				{
					if (res->width == 32)
						resource = 0x1ff;
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->effect_entry = entry;
					entry->priority = 0;
					res->flags = 0;
				}
			}
		}
		BattlePres_SetActorModeAndAction(id);
	}
	/* FAKEMATCH: needed to match: the goto rotation preserves indexed list reads */
	if (refresh)
	{
		i = 0;
		if ((resource = list[0]) != 255)
		{
		again:
			if (list[i] != 254 && (slot = BtlObjGet(resource)) != 0 && (object = slot->object) != 0)
				BattlePres_SetActorModeAndAction(resource);
			i++;
			if (i <= 13 && (resource = list[i]) != 255)
				goto again;
		}
	}
}
