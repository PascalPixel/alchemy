#include "unit/graphics/tile/griddata.h"
#include "unit/field/field.h"
#include "unit/field/fldobj.h"
#include "unit/field/script.h"
#include "unit/field/operands.h"
#include "unit/field/iconmon.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
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

struct IconColumn
{
	s16 no;
	s16 animation;
	u8	on;
	s8	priority;
	s8	param;
};

void Ui_RunIconMonitor(void)
{
	struct IconColumn		  ent[4];
	struct AnimationMetadata *metadata;
	struct AnimationObject	 *tbl;
	volatile s32			  zero;
	s32						  no;
	s32						  x;
	s32						  y;
	u32						  mode;
	s32						  flag;
	struct AnimationPosition *work;
	s32						  slot;
	u32						  cnt;
	s32						  alt;
	s32						  moved;
	/* FAKEMATCH: volatile, as gKeysHeld and gKeysRepeat are declared (keystate.h): each *held and *repeat is a load of its own. A plain held differs 245, 663 instructions for 672; a plain repeat 89, 669 for 672. */
	volatile u32			 *held;
	volatile u32			 *repeat;
	u32						  i;

	x = 144;
	y = 96;
	mode = 1;
	flag = 1;
	slot = 0;
	work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
	gDecodeFillByte = 3;

	zero = 0;
	Dma_Set(&zero, work, 0x85000001, REG_DMA3);
	zero = 0;
	Dma_Set(&zero, ent, 0x85000008, REG_DMA3);

	no = Ui_FindNextNumberWithMetadata(-1, 1);
	for (i = 0; i <= 3; i++)
	{
		ent[i].animation = 1;
		ent[i].priority = 1;
		ent[i].no = no;
	}
	ent[0].on = 1;
	gDebugMode = 2;
	REG_BLDCNT = 0x3F42;
	PLTT_BACKDROP = 0x01E0;
	REG_DISPCNT = 0x1140;
	Blend_SetDarkenTarget0(1);

restart:
	Runtime_InitializeHeap();
	Scheduler_ResetTaskTable();
	work = Runtime_AllocateBlock(9, 10 * sizeof(*work));
	Resource_InitializeTable();
	ObjectSystem_Initialize(2);
	ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, ent[0].no, 0);
	metadata = Resource_GetMetadataRecordFar(ent[0].no);
	if (metadata->draw_kind == 20)
	{
		ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, ent[0].no + 1, 0);
	}

	for (i = 0; i <= 9; i++)
	{
		metadata = Resource_GetMetadataRecordFar(ent[0].no);
		alt = 0;
		if (metadata->draw_kind == 20 && (i & 1) != 0)
		{
			alt = 1;
		}
		alt = (s32)ResourceObject_Create(ent[0].no + alt + (alt << 12));
		ResourceMetadata_Register((struct AnimationObject *)alt, ent[1].no);
		ResourceMetadata_Register((struct AnimationObject *)alt, ent[2].no);
		ResourceMetadata_Register((struct AnimationObject *)alt, ent[3].no);
		/* FAKEMATCH: needed to match: the native adds the flags offset to the object's own register (r5) in place; a separate address takes a copy. */
		alt = (s32) & ((struct AnimationObject *)alt)->flags;
		*(u8 *)alt = flag;
	}

	for (i = 0; i <= 3; i++)
	{
		if (ent[i].on != 0)
		{
			Ui_SetGridColumnPriority(i, ent[i].priority);
		}
		else
		{
			Ui_SetGridColumnPriority(i, 8);
		}
		Ui_SetGridColumnParameter(i, ent[i].param);
		Ui_FillGridColumnFromMetadata(i, ent[i].animation);
	}

	Map_BuildProbeRing(x, y, work);
	Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
	WaitFrames(1);
	held = &gKeysHeld;
	repeat = &gKeysRepeat;
	for (;;)
	{

		if ((*held & KEY_START) != 0)
		{
			if ((*held & KEY_LEFT) != 0)
			{
				x--;
			}
			if ((*held & KEY_RIGHT) != 0)
			{
				x++;
			}
			if ((*held & KEY_UP) != 0)
			{
				y--;
			}
			if ((*held & KEY_DOWN) != 0)
			{
				y++;
			}
			Map_BuildProbeRing(x, y, work);
		}
		else
		{
			(void)gKeysRepeat;
			if ((*repeat & KEY_UP) != 0)
			{
				slot = (slot - 1) & 3;
			}
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				slot = (slot + 1) & 3;
			}
			if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				mode = (mode - 1) & 3;
			}
			if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				mode++;
				mode &= 3;
			}
		}

		if ((gKeyState & KEY_START) != 0)
		{
			tbl = gSpriteObjects;
			flag ^= 1;
			for (i = 0; i <= 9; i++)
			{
				tbl->flags = flag;
				tbl++;
			}
		}

		switch (mode)
		{
		case 0:
			if (slot == 1)
			{
				break;
			}
			if ((gKeysRepeat & KEY_L) != 0)
			{
				ent[slot].animation--;
				if (ent[slot].animation < 0)
				{
					ent[slot].animation = 0;
				}
				if (slot == 0)
				{
					ent[1].animation = ent[0].animation;
				}
				goto next;
			}
			if ((gKeysRepeat & KEY_R) == 0)
			{
				break;
			}
			ent[slot].animation++;
			if (ent[slot].animation > 99)
			{
				ent[slot].animation = 99;
			}
			if (slot == 0)
			{
				ent[1].animation = ent[0].animation;
			}
			goto next;

		case 1:
			moved = 0;
			cnt = 1;
			if ((*held & KEY_START) != 0)
			{
				cnt = 10;
			}
			if ((*repeat & KEY_L) != 0)
			{
				for (i = 0; i < cnt; i++)
				{
					ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, -1);
				}
				moved = 1;
			}
			if ((*repeat & KEY_R) != 0)
			{
				for (i = 0; i < cnt; i++)
				{
					ent[slot].no = Ui_FindNextNumberWithMetadata(ent[slot].no, 1);
				}
				moved = 1;
			}
			if (moved == 0)
			{
				break;
			}
			if (slot == 0)
			{
				goto restart;
			}
			if (ent[slot].on != 0)
			{
				Ui_SetGridColumnNumber(slot, ent[slot].no);
				Ui_SetGridColumnPriority(slot, ent[slot].priority);
				Ui_SetGridColumnParameter(slot, ent[slot].param);
				Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
			}
			goto next;

		case 2:
			moved = 0;
			if ((*repeat & KEY_L) != 0)
			{
				ent[slot].priority--;
				if (ent[slot].priority < 0)
				{
					ent[slot].priority = 3;
				}
				moved = 1;
			}
			if ((*repeat & KEY_R) != 0)
			{
				ent[slot].priority++;
				if (ent[slot].priority > 3)
				{
					ent[slot].priority = 0;
				}
				moved = 1;
			}
			if (moved == 0)
			{
				break;
			}
			if (ent[slot].on != 0)
			{
				Ui_SetGridColumnPriority(slot, ent[slot].priority);
			}
			goto next;

		case 3:
			moved = 0;
			if ((*repeat & KEY_L) != 0)
			{
				ent[slot].param--;
				if (ent[slot].param < 0)
				{
					ent[slot].param = 15;
				}
				moved = 1;
			}
			if ((*repeat & KEY_R) != 0)
			{
				ent[slot].param++;
				if (ent[slot].param > 15)
				{
					ent[slot].param = 0;
				}
				moved = 1;
			}
			if (moved == 0)
			{
				break;
			}
			if (ent[slot].on != 0)
			{
				Ui_SetGridColumnParameter(slot, ent[slot].param);
			}
			goto next;

		default:
			break;
		}

		if ((*repeat & KEY_A) != 0)
		{
			if (mode == 0)
			{
				if (slot != 1)
				{
					if (ent[slot].on != 0)
					{
						Ui_FillGridColumnFromMetadata(slot, ent[slot].animation);
						if (slot == 0)
						{
							Ui_FillGridColumnFromMetadata(1, ent[1].animation);
						}
					}
					goto next;
				}
			}
			else if (slot != 0)
			{
				ent[slot].on ^= 1;
				Ui_SetGridColumnPriority(slot, ent[slot].on != 0 ? ent[slot].priority : 8);
				goto next;
			}
		}

		if ((*repeat & KEY_SELECT) != 0)
		{
			Scheduler_ResetTaskTable();
			if ((*held & KEY_B) != 0)
			{
				RuntimeDispatch_ReturnZero(
					(s32)&ResourceId_ResourceFarCalls, FarCall_ResourceTable);
			}
			else
			{
				RuntimeDispatch_ReturnZero(
					(s32)&ResourceId_ResourceFarCallsAlternate, FarCall_ResourceTable);
			}
			goto restart;
		}
		WaitFrames(1);
	}
}

s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step)
{
	for (;;)
	{
		start += step;
		if (start < 0)
		{
			start = 0x200;
			continue;
		}
		{
			/* FAKEMATCH: needed to match: the upper bound in a local compares as cmp against 0x200 with blt; the literal compares against 0x1ff from the pool. */
			s32 limit = 0x200;
			if (start >= limit)
			{
				start = -1;
				continue;
			}
		}
		if (Resource_GetMetadataRecordFar(start)->width)
		{
			return start;
		}
	}
}

void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out)
{
	struct AnimationPosition *pair;
	s32						  angle;
	s32						  i;
	u16						  turn;
	u8						  kind;

	angle = 0;
	kind = gSpriteObjects->entries[0]->kind;
	if ((gKeysHeld & KEY_B) != 0)
		angle = (s16)(gFrameTick << 8);
	switch ((u32)kind)
	{
	case 3:
		for (i = 0; i < 6; i++)
		{
			angle <<= 16;
			out->x = x << 16;
			angle = (u32)angle >> 16;
			out->y = 0;
			out->z = z << 16;
			Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
			out++;
			angle = (s16)(angle + 0x2aaa);
		}
		for (; i < 10; i++)
			out++;
		break;
	case 5:
	case 8:
	case 44:
	case 88:
		for (i = 0; i < 8; i++)
		{
			angle <<= 16;
			out->x = x << 16;
			angle = (u32)angle >> 16;
			out->y = 0;
			out->z = z << 16;
			Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
			out++;
			angle = (s16)(angle + 0x2000);
		}
		for (; i < 10; i++)
			out++;
		break;
	case 4:
	case 6:
		for (i = 0; i < 10; i++)
		{
			angle <<= 16;
			out->x = x << 16;
			angle = (u32)angle >> 16;
			out->y = 0;
			out->z = z << 16;
			Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
			out++;
			angle = (s16)(angle + 0x1999);
		}
		break;
	case 20:
		turn = angle;
		angle = (s16)(turn + 0x4000);
		i = 0;
		pair = out;
		while (i < 2)
		{
			pair[i * 2].x = x << 16;
			pair[i * 2].y = 0;
			pair[i * 2].z = z << 16;
			angle <<= 16;
			angle = (u32)angle >> 16;
			Vector_AddPolarOffset(0x280000, angle, &out[0].x);
			pair[i * 2 + 1].x = x << 16;
			{
				s32 *point = &out[1].x;
				pair[i * 2 + 1].y = 0;
				pair[i * 2 + 1].z = z << 16;
				Vector_AddPolarOffset(0x280000, (u16)angle, point);
			}
			angle = (angle + 0x8000) << 16;
			i++;
			out += 2;
			angle >>= 16;
		}
		break;
	default:
		angle = ((angle << 16) + 0x20000000) >> 16;
		for (i = 0; i < 4; i++)
		{
			angle <<= 16;
			angle = (u32)angle >> 16;
			out->x = x << 16;
			out->y = 0;
			out->z = z << 16;
			Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
			out++;
			angle = (s16)(angle + 0x4000);
		}
		for (i = 4; i < 10; i++)
			out++;
		break;
	}
}

struct Vec2
{
	s32 x;
	s32 y;
};

// gSpriteObjects の 10 個それぞれについて、slot 番のエントリの field を value にする
#define DEFINE_SET_GRID_COLUMN(name, field)              \
	void name(s32 slot, s32 value)                       \
	{                                                    \
		struct AnimationObject *object = gSpriteObjects; \
		s32						i;                       \
                                                         \
		slot &= 3;                                       \
		for (i = 0; i < 10; i++)                         \
		{                                                \
			object->entries[slot]->field = value;        \
			object++;                                    \
		}                                                \
	}

DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnParameter, param)
DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnPriority, priority)

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
	s32						count;
	struct AnimationObject *object;

	object = gSpriteObjects;
	count = 0;
	do
	{
		struct AnimationEntry	 *entry = object->entries[slot & 3];
		struct AnimationMetadata *metadata;
		s32						  index = 0;

		if (entry->scripts != 0)
		{
			metadata = Resource_GetMetadataRecordFar(entry->anim_id);
			if (value < metadata->animation_count)
			{
				entry->kind = metadata->draw_kind;
				entry->script = entry->scripts[value];
				entry->timer = count * 0x10;
				entry->step = 0x10;
				entry->pos = index;
				entry->frame_base = index;
				entry->frame = 0xff;
			}
			object->offset_y = metadata->adjust_y;
			object->rotation = index;
		}
		count++;
		object++;
	} while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
	struct AnimationObject *object = gSpriteObjects;
	s32						i;

	Resource_GetMetadataRecordFar(no);
	slot &= 3;
	for (i = 0; i < 10; i++)
	{
		struct AnimationEntry *entry = object->entries[slot];

		entry->anim_id = no;
		Animation_InitWorkFromMetadata(entry);
		object++;
	}
}

void Battle_PlaceActorsByFormationKind(void)
{
	struct AnimationObject	 *actor = WORK_SLOT(4);
	u32						  kind = actor->entries[0]->kind;
	struct Vec2				  scale;
	struct AnimationPosition *table;
	u16						  angle;
	u16						  step;
	u16						  odd = 0;
	u32						  count;
	u32						  i;

	scale = (*(struct Vec2 *)&Battle_FormationPlacementScale);
	table = WORK_SLOT(HEAP_SLOT_BATTLE);

	switch (kind)
	{
	case 3:
		angle = 0;
		step = 0x2aaa;
		count = 6;
		break;
	case 5:
	case 8:
	case 44:
	case 88:
		angle = 0;
		step = 0x2000;
		count = 8;
		break;
	case 4:
	case 6:
		angle = 0;
		step = 0x1999;
		count = 10;
		break;
	case 20:
		angle = 0;
		step = 0;
		odd = 0x8000;
		count = 4;
		break;
	default:
		angle = 0x2000;
		step = 0x4000;
		count = 4;
		break;
	}

	for (i = 0; i < count; i++)
	{
		Render_ApplyProjectedPlacement(actor, &table[i].x, (s32 *)&scale, angle);
		actor++;
		angle += step;
		if (i & 1)
			angle += odd;
	}
}
