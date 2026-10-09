#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_point_position.h"
#include "animspr.h"
#include "types.h"
#include "motobj.h"
#include "global_cells.h"
#include "iwram_call.h"
#include "scene.h"
#include "btlwork.h"
#include "btldisp.h"
#include "btlsys.h"

s32 ObjectDispatch_ReleaseFar(s32);

s32 Graphics_PrepareTransferAndRun(void *, void *);
s32 Graphics_PrepareTransferInIwramWork(void *, void *);

s32 Render_ProjectPoint(const s32 *, s32 *);

struct BtlObj *BtlObjGet(s32 object_id)
{
	struct BtlWork *work = BtlWk;
	struct BtlObj  *slots = work->slots;
	s32				index;
	if (object_id > 7)
	{
		object_id -= 0x78;
	}
	index = object_id;
	if (work->placement[index] == 0xFF)
	{
		return NULL;
	}
	return &slots[work->placement[index]];
}

void ResetMotionRecordGroup(void *owner)
{
	struct AnimationObject *object = owner;
	s32						i;

	if (object != NULL)
	{
		for (i = 0; i < 4; i++)
		{
			if (object->entries[i] != NULL)
				object->entries[i]->script = NULL;
		}
	}
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
	s32	   storage_kind;
	s32	   remaining;
	void **record_groups;
	void  *record_group;

	if (object != NULL)
	{
		storage_kind = object->record_storage_kind & 0xF;
		switch (storage_kind)
		{
		case 1:
			ResetMotionRecordGroup(object->records);
			return;
		case 2:
			record_groups = object->records;
			remaining = 3;
			do
			{
				record_group = *record_groups++;
				if (record_group != NULL)
				{
					ResetMotionRecordGroup(record_group);
				}
				remaining -= 1;
			} while (remaining >= 0);
			break;
		}
	}
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
	struct BtlObj *slot;
	BattleMotion_ReleaseObjectSlotByValue(object_id);
	slot = BtlObjGet(object_id);
	slot->active = 1;
	return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
	/* FAKEMATCH: needed to match: the ignored-result signature keeps the return-address pop in r1 */
	s32			   no;
	s32			   i;
	struct BtlObj *slot;

	i = 0;
	do
	{
		no = i + 0x78;
		if (i <= 7)
		{
			no = i;
		}
		slot = BtlObjGet(no);
		if ((slot != NULL) && (slot->active != 0))
		{
			ObjectDispatch_ReleaseFar((s32)slot->object);
			slot->object = NULL;
			slot->active = 0;
		}
		i += 1;
	} while (i <= 0xD);
}

s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
	struct BtlWork *state = BtlWk;

	out->x = state->slots[no].anchor_x;
	out->y = 0;
	out->z = state->slots[no].anchor_z;
	return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
	struct BattleCamera *state = gCameraWork;
	Render_ResetTransformState();
	if (GameFlag_TestFar(0x16B) != 0)
	{
		Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
		return Graphics_PrepareTransferAndRun(state, state->pos);
	}
	else
	{
		return Graphics_PrepareTransferInIwramWork(state, state->pos);
	}
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						position[3];
	s32						scaled;

	Camera_ApplyTransformByFlag();
	position[0] = object->x;
	position[1] = object->y;
	position[2] = object->z;
	scaled = Render_ProjectPoint(position, projected);
	(void)Iwram_MulQ16(scaled, record->scale);
	return 0;
}

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
	s32 storage_kind = object->record_storage_kind & 0xF;
	if (storage_kind == 1)
	{
		if (record_index == 0)
		{
			return object->records;
		}
	}
	else if (storage_kind == 2)
	{
		return ((void **)object->records)[record_index];
	}
	return NULL;
}
