#include "unit/field/common/fielddat.h"
#include "unit/field/common/object/objdata.h"
#include "unit/graphics/render/facing.h"
#include "unit/graphics/tile/griddata.h"
#include "unit/lib/curvedat.h"
#include "unit/field/common/script/field.h"
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
// source を destination に展開して destination を返す
// source[0・1] : コマンドの列までのオフセット（0 なら圧縮なしで source + 2 を返す）
// source + 2   : そのまま写すバイトの列
// コマンドの列 : 制御バイトを下位ビットから 1 ビットずつ読む
//   1 : そのまま写すバイトを 1 つ写す
//   0 : 2 バイト（上位・下位）を読む。0 なら終わり。
//       下位 12 ビットが距離、上位 4 ビット + 2 が長さ（4 ビットが 0 なら次のバイト + 18）
//       source + 2 から距離だけ前のところから写す
//--------------------------------------------------------------------------
u8 *Resource_DecompressLz(const u8 *source, u8 *destination)
{
	s32		  token;
	u32		  length;
	u32		  sentinel;
	u32		  control;
	u8		  byte;
	u8		 *start;
	const u8 *base;
	const u8 *command;
	const u8 *copy;
	const u8 *literal;

	token = source[0] | (source[1] << 8);
	literal = source + 2;
	if (token == 0)
		return (u8 *)literal;
	command = source + token;
	control = *command;
	sentinel = 128;
	sentinel <<= 1; // 8 ビット読み終えたしるし
	base = literal;
	start = destination;
	command++;
	control |= sentinel;
	goto dispatch;

back_reference:
	length = token >> 12;
	if (length == 0)
	{
		length = *command + 16;
		command++;
	}
	length += 2;
	copy = base - (token & 0x0fff);
	while (length != 0)
	{
		*destination++ = *copy++;
		length--;
	}

shift:
	control >>= 1;
	if (control == 0)
	{
		control = *command;
		sentinel = 128;
		sentinel <<= 1;
		command++;
		control |= sentinel;
	}

dispatch:
	if ((control & 1) != 0)
	{
		if (control != 1) // 残っているのがしるしだけなら次の制御バイトへ
			*destination++ = *literal++;
		goto shift;
	}

	byte = *command++;
	token = byte << 8;
	token |= *command++;
	if (token != 0)
		goto back_reference;
	return start;
}

typedef void (*DrawFn)(const void *src, void *dst, s32 param);
typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir)
{
	s32						 changed;
	u32						 size;
	struct ObjectSystemWork *ctx;
	DrawFn					 draw;
	s32						 held;
	struct AnimationEntry	*e;
	void					*block;
	u8						*buf;
	u8						*tmp;
	void					*decoded;
	u16						 order[4];
	s32						 i;
	s32						 k;
	s32						 n;
	u32						 key;
	s32						 arg;
	s32						 base;
	u32						 attr;
	s32						 tile;

	changed = 0;
	ctx = (*(void *(*)[]) & gMenuCtrlWork)[0];
	held = 1;
	draw =
		(DrawFn)(*(void *(*)[]) & gMenuCtrlWork)[HEAP_SLOT_FRAME_DECODER - HEAP_SLOT_MENU_CONTROL];
	if (draw == 0)
	{
		block = Runtime_AllocateHeapBlock(HEAP_SLOT_FRAME_DECODER, (s32)Render_DecodeFrameCodeSize);
		Dma_Set(Render_DecodeFrame, block,
			(((u32)Render_DecodeFrameBuffer - (u32)Render_DecodeFrame) >> 2)
				| (DMA_ENABLE32 | DMA_32BIT),
			REG_DMA3);
		draw = (DrawFn)(*(void *(*)[])
						& gMenuCtrlWork)[HEAP_SLOT_FRAME_DECODER - HEAP_SLOT_MENU_CONTROL];
		held = 0;
	}

	for (i = 0; i < obj->count; i++)
	{
		e = obj->entries[i];
		if (e == 0)
		{
			continue;
		}
		if (e->script == 0)
		{
			continue;
		}
		/* FAKEMATCH: needed to match: explicit timer-test lifetime keeps command back-edges and the decoded frame value separate from the hold reload. */
	read_command:
		if ((s16)e->timer > 0)
			goto advance_timer;
		base = e->script[e->pos++];
		arg = e->script[e->pos++];
		switch (base)
		{
		case 254:
			Animation_SetWorkEntry(e, arg);
			obj->last_no = arg;
			break;
		case 253:
			e->pos = arg;
			break;
		case 245:
			e->timer += arg << 4;
			break;
		case 241:
			e->pos -= 2;
			goto framed;
		case 240:
			e->kind = arg;
			break;
		case 255:
			e->frame_base = 255;
			base = 255;
			e->timer += arg << 4;
			goto select_direction;
		case 239:
			e->frame_base = 255;
			e->script = 0;
			base = 255;
			obj->count--;
			goto select_direction;
		case 242:
		case 243:
		case 244:
		case 246:
		case 247:
		case 248:
		case 249:
		case 250:
		case 252:
			break;
		default:
			e->frame_base = base;
			e->timer += arg << 4;
			goto select_direction;
		}
		goto read_command;
	advance_timer:
		e->timer = e->timer - e->step;
	framed:
		base = e->frame_base;
	select_direction:
		switch (e->kind)
		{
		case 1:
			attr = AnimationFacing_Kind1[dir >> 13];
			break;
		case 2:
		case 20:
			attr = AnimationFacing_Kind2[dir >> 13];
			break;
		case 22:
			attr = AnimationFacing_Kind22[dir >> 13];
			break;
		case 3:
			attr = AnimationFacing_Kind3[dir >> 12];
			break;
		case 4:
			attr = AnimationFacing_Kind4[dir >> 10];
			break;
		case 5:
			attr = AnimationFacing_Kind5[dir >> 12];
			break;
		case 6:
			attr = AnimationFacing_Kind6[dir >> 10];
			break;
		case 8:
			attr = AnimationFacing_Kind8[((u32)(dir << 16) + 0x10000000) >> 29];
			break;
		case 88:
			attr = AnimationFacing_Kind88[((u32)(dir << 16) + 0x10000000) >> 29];
			break;
		default:
			attr = 0;
			break;
		}
		base += attr & 7;
		if (i == 0 && (attr >> 7) != 0)
		{
			changed = 1;
		}
		if (e->frame != base)
		{
			e->frame = base;
			obj->dirty = 1;
		}
	}

	if (obj->dirty != 0)
	{
		size = obj->width * obj->height;
		buf = MemAlloc(size);
		Iwram_ClearWords(buf, size);

		n = -1;
		for (i = obj->count - 1; i >= 0; i--)
		{
			e = obj->entries[i];
			if (e == 0)
			{
				continue;
			}
			if (e->frames == 0)
			{
				continue;
			}
			if (e->frame == 255)
			{
				continue;
			}
			key = e->priority;
			if (key > 3)
			{
				continue;
			}
			key = (key << 8) | i;
			/* FAKEMATCH: needed to match: the first shift written apart and the scan left by goto keep the scan unrotated, with its step at the top. */
			k = n;
			if (k >= 0 && order[k] > key)
			{
				order[k + 1] = order[k];
				for (;;)
				{
					k--;
					if (k < 0)
						goto insert;
					if (order[k] <= key)
						goto insert;
					order[k + 1] = order[k];
				}
			}
		insert:
			order[k + 1] = key;
			n++;
		}
		n++;

		for (i = 0; i < n; i++)
		{
			e = obj->entries[*(u8 *)&order[i]];
			if (e->mode == 1)
			{
				Resource_DecodeType01(e->frames[e->frame], buf);
			}
			else if (e->mode == 3)
			{
				if (e->param != 0)
				{
					tmp = MemAlloc(0x400);
					draw(Resource_DecompressLz(e->frames[e->frame], tmp), buf, e->param);
					MemFree(tmp);
				}
				else
				{
					decoded = Iwram_Decompress(e->frames[e->frame], buf);
					if (decoded != 0)
					{
						draw(decoded, buf, 0);
					}
				}
			}
			else
			{
				draw(e->frames[e->frame], buf, e->param);
			}
		}

		if ((obj->flags & 2) != 0)
		{
			u8 *mask;
			u8	edge;
			u8	fill;
			u32 w;
			u32 h;
			u8 *src;
			u8 *dst;

			mask = MemAlloc(size);
			w = obj->width;
			h = obj->height;
			edge = ctx->edge;
			fill = ctx->fill;
			Iwram_ClearWords(mask, size);
			src = buf + w + 1;
			dst = mask + w + 1;
			for (i = 1; i < h - 1; i++)
			{
				for (k = 1; k < w - 1; k++)
				{
					if (src[-1] != 0 && src[1] != 0 && *(src - w) != 0 && src[w] != 0)
					{
						*dst = 1;
					}
					dst++;
					src++;
				}
				dst += 2;
				src += 2;
			}
			src = buf;
			dst = mask;
			for (i = 0; i < size; i++)
			{
				if (*dst != 0)
				{
					*src = fill;
				}
				else if (*src != 0)
				{
					*src = edge;
				}
				dst++;
				src++;
			}
			MemFree(mask);
		}

		tile = VramBlock_LoadCached(obj->slot, size, 0);
		{
			void *vram = (void *)(0x06010000 + (tile << 5));
			((UploadFn)gWorkSlot[HEAP_SLOT_FRAME_UPLOADER])(buf, obj->width, obj->height, vram);
		}
		obj->part[0].tile = tile;
		obj->dirty = 0;
		ctx->used += size;
		MemFree(buf);
	}

	if (held == 0)
	{
		Runtime_ReleaseHeapBlock(52);
	}
	return changed;
}

void Render_PlaceSpritePartPair(struct AnimationObject *sprite, s32 screen_x, s32 height,
	s32 screen_y, s32 ground_height, s32 *scale)
{
	u32							half_width = sprite->width >> 1;
	u32							half_height = sprite->height >> 1;
	s32							margin_x = 8;
	s32							margin_y = 4;
	s32							affine = 1;
	s32							scale_x = *scale++;
	s32							scale_y = *scale;
	s32							x;
	s32							y;
	struct AnimationSpritePart *shadow;

	if (scale_x > 0x10000 || scale_y > 0x10000)
	{
		affine = 3;
		margin_x = 16;
		margin_y = 8;
		half_width <<= 1;
		half_height <<= 1;
	}
	x = (screen_x >> 16) - half_width;
	y = ((screen_y - height) >> 16) - half_height
		- ((((sprite->height >> 1) - sprite->offset_y) * scale_y + 0xffff) >> 16);
	sprite->part[0].affine = affine;
	sprite->part[0].x = x;
	sprite->part[0].y = y;
	x = (screen_x >> 16) - margin_x;
	y = ((screen_y - ground_height) >> 16) - margin_y;
	shadow = &sprite->part[1];
	shadow->affine = affine;
	shadow->x = x;
	shadow->y = y;
}

void Render_ApplyProjectedPlacement(struct AnimationObject *sprite, s32 *pos, s32 *scale, s32 mode)
{
	s32 matrix;
	u32 half_width = sprite->width >> 1;
	u32 half_height = sprite->height >> 1;
	s32 size = 8;
	s32 size_half = 4;
	s32 affine;
	s32 scale_x = *scale++;
	s32 px = *pos++;
	s32 scale_y = *scale;
	s32 py = *pos++;
	s32 pz = *pos++;
	s32 pw = *pos;
	s32 slot;
	/* FAKEMATCH: needed to match: the two unused arrays supply 24 bytes of the 68-byte frame; removing them leaves 44 bytes. */
	s32							ground[3];
	s32							screen[3];
	struct AffineTransform		effect;
	s32							flip;
	s32							x;
	s32							y;
	s32							depth;
	struct AnimationSpritePart *part;

	flip = Sprite_ComposeAnimationFrame(sprite, mode);
	if (flip == 0 && scale_x == 0x10000 && scale_y == scale_x && sprite->rotation == 0)
	{
		affine = 0;
		matrix = 0;
	}
	else
	{
		affine = 1;
		effect.angle = sprite->rotation;
		effect.scale_x = scale_x >> 8;
		effect.scale_y = scale_y >> 8;
		if (flip != 0)
			effect.scale_x = -effect.scale_x;
		matrix = AffineMatrix_BuildForEffect(&effect);
	}
	if (scale_x > 0x10000 || scale_y > 0x10000)
	{
		affine = 3;
		half_width <<= 1;
		half_height <<= 1;
		size = 16;
		size_half = 8;
	}
	if (py <= -0x640000)
	{
		slot = 1;
		depth = 0;
	}
	else
	{
		slot = (pz >> 17) + 10;
		depth = 2;
	}
	y = ((pz - pw) >> 16) - size_half;
	if (sprite->flags & 1)
	{
		if (y < 160)
		{
			part = &sprite->part[1];
			part->affine = affine;
			part->affine_index = matrix;
			part->x = (px >> 16) - size;
			part->y = y;
			Runtime_PushSlotEntry(part, depth);
		}
	}
	x = (px >> 16) - half_width + ((sprite->offset_x * scale_x + 0xffff) >> 16);
	y = ((pz - py) >> 16) - half_height
		- ((scale_y * ((sprite->height >> 1) - sprite->offset_y) + 0xffff) >> 16);
	if (x < 240 && y < 160)
	{
		part = &sprite->part[0];
		part->x = x;
		part->y = y;
		part->affine = affine;
		part->affine_index = matrix;
		Runtime_PushSlotEntry(part, slot);
	}
}

void Render_PlaceProjectedSprite(
	struct AnimationObject *sprite, s32 *point, s32 *scale, s32 mode, s32 depth)
{
	s32					   flip;
	u32					   half_height;
	u32					   size;
	s32					   affine;
	s32					   half_width;
	s32					   ground[3];
	s32					   screen[3];
	struct AffineTransform effect;
	s32					   z;
	s32					   base;
	s32					   scale_x;
	s32					   scale_y;
	s32					   offset_x;
	s32					   offset_y;
	s32					   slot;

	affine = 1;
	if (gMenuCtrlWork->suspended != 0)
		goto hide;
	z = Render_ProjectPoint(point, screen);
	if (screen[2] == 0)
		goto hide;
	if (screen[0] < -32)
		goto hide;
	if (screen[0] > 272)
		goto hide;
	if (screen[1] < -32)
		goto hide;
	if (screen[1] > 208)
		goto hide;
	if (sprite->display_flags & 2)
		z = sprite->scale;
	else
		z = Iwram_MulQ16(z, sprite->scale);
	half_width = sprite->width >> 1;
	half_height = sprite->height >> 1;
	size = 8;
	flip = Sprite_ComposeAnimationFrame(sprite, mode);
	base = (z + 0x400) & -0x800;
	scale_x = Iwram_MulQ16(base, *scale++);
	scale_y = Iwram_MulQ16(base, *scale);
	if (scale_x > 0x1f7ff)
		scale_x = 0x1f800;
	if (scale_y > 0x1f7ff)
		scale_y = 0x1f800;
	offset_x = Iwram_MulQ16(sprite->offset_x, scale_x);
	offset_y = -Iwram_MulQ16((sprite->height >> 1) - sprite->offset_y, scale_y);
	if (scale_x > 0x10000 || scale_y > 0x10000)
	{
		affine = 3;
		half_width <<= 1;
		half_height <<= 1;
		size = 16;
	}
	else if (scale_x == 0x10000 && sprite->rotation == 0 && scale_y == scale_x)
	{
		affine = 0;
	}
	if (affine != 0)
	{
		effect.angle = sprite->rotation;
		effect.scale_x = scale_x >> 8;
		if (flip != 0)
		{
			effect.scale_x = -effect.scale_x;
			offset_x = -offset_x;
		}
		effect.scale_y = scale_y >> 8;
		z = AffineMatrix_BuildForEffect(&effect);
	}
	else if (flip != 0)
	{
		z = 8;
		offset_x = -offset_x;
	}
	else
	{
		z = 0;
	}
	sprite->part[0].x = screen[0] - half_width + offset_x;
	sprite->part->y = screen[1] - half_height + offset_y;
	sprite->part[0].affine = affine;
	sprite->part[0].affine_index = z;
	if (depth == 0)
	{
		slot = (0x200 - screen[2]) / 2 + 128;
		if (slot <= 0)
			slot = 1;
		Runtime_PushSlotEntry(sprite, slot);
	}
	else
	{
		Runtime_PushSlotEntry(sprite, depth);
	}
	if (sprite->flags & 1)
	{
		struct AnimationSpritePart *shadow;

		ground[0] = point[0];
		ground[1] = 0;
		ground[2] = point[2];
		Render_ProjectPoint(ground, screen);
		shadow = &sprite->part[1];
		shadow->x = screen[0] - size;
		shadow->y = screen[1] - (size >> 1) + 2;
		shadow->affine = affine;
		shadow->affine_index = z;
		if (depth == 0)
			Runtime_PushSlotEntry(shadow, 0);
		else
			Runtime_PushSlotEntry(shadow, depth);
	}
	return;
hide:
	if (!(sprite->display_flags & 1))
	{
		Resource_ActivateEntry(sprite->slot);
		sprite->dirty = 1;
	}
}

void ObjectGroup_SetChildValueUnlessFifteen(void *raw_parent, u32 value)
{
	struct AnimationObject *parent = raw_parent;
	if (parent != 0)
	{
		u32 n = parent->count;

		if (n != 0)
		{
			struct AnimationEntry **p = parent->entries;
			u32						cnt = n;
			do
			{
				struct AnimationEntry *child = *p++;
				if (child->param != 15)
					child->param = value;
				cnt--;
			} while (cnt != 0);
		}
		parent->dirty = 1;
	}
}

struct ResourceSlotNumber
{
	u16 resource;
	s16 number;
};

enum
{
	RESOURCE_SLOT_COUNT = 8,
	ANIMATION_OBJECT_COUNT = 64
};

extern struct ResourceSlotNumber ResourceSlot_NumberTable[];

s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant)
{
	struct AnimationMetadata  *metadata;
	struct ObjectSystemWork	  *work;
	struct ResourceSlot		  *record;
	struct ResourceSlotNumber *entry;
	u32						  *offset;
	u8						  *text;
	u8						  *end;
	u8						  *table;
	u32						   size;
	u32						   count;
	u32						   index;
	u16						   entry_number;
	s32						   resource;

	if (slot >= RESOURCE_SLOT_COUNT)
		return 0;
	work = gMenuCtrlWork;
	record = &work->slots[slot];
	metadata = Resource_GetMetadataRecordFar(number);
	record->tag = (slot << 12) | number;
	record->buffer = buffer;

	entry = ResourceSlot_NumberTable;
	for (count = 0; count < 256; count++)
	{
		s16 *numbers = &entry->number;

		resource = entry->resource;
		entry_number = *numbers;
		entry++;
		if (entry_number == 0)
			return 0;
		if (entry_number == number)
			break;
	}
	size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

	offset = buffer;
	for (count = 0; count < 256; count++)
	{
		if (*offset == 0)
			break;
		*offset += (u32)buffer;
		offset++;
	}
	if (variant != 0)
	{
		index = variant - 1;
		text = (u8 *)(offset + 1);
		end = (u8 *)buffer + size;
		if (index > 4)
			index = 0;
		table = ResourceSlot_ConversionTables[index];
		for (; text < end; text++)
		{
			u32 c = *text;

			if (c <= 0xdf)
			{
				c = table[c];
				*text = c;
			}
		}
	}
	return metadata->width * metadata->height;
}

s32 Animation_LookupValueByKey(s32 key)
{
	u32					 no;
	struct ResourceSlot *p = gMenuCtrlWork->slots;

	for (no = 0; no < RESOURCE_SLOT_COUNT; no++, p++)
	{
		if (p->tag == key)
			return (s32)p->buffer;
	}
	return 0;
}

s32 Animation_InitializeObjects(struct AnimationObject *state)
{
	s32 index;

	for (index = 0; index < state->count; index++)
	{
		struct AnimationEntry	 *object = state->entries[index];
		struct AnimationMetadata *metadata = Resource_GetMetadataRecordFar(object->anim_id);
		void					**frames;

		if (metadata->width == 0)
			continue;

		if (index == 0)
		{
			state->width = metadata->width;
			state->height = metadata->height;
			state->scale = metadata->scale << 8;
			state->offset_y = metadata->adjust_y;
			state->offset_x = metadata->adjust_x;
		}

		frames = metadata->frames;
		if (frames == 0)
			frames = (void **)Animation_LookupValueByKey(object->anim_id);

		object->kind = metadata->draw_kind;
		object->frames = frames;
		object->scripts = metadata->animation;
		object->mode = metadata->frame_codec;
		object->frame = 0xff;
		object->script = 0;
		object->pos = 0;
	}

	return 0;
}

void Animation_InitWorkFromMetadata(struct AnimationEntry *work)
{
	void					**value;
	s32						  z;
	struct AnimationMetadata *info;

	if (work != NULL)
	{
		info = Resource_GetMetadataRecordFar(work->anim_id);
		if (info->width != 0)
		{
			value = info->frames;
			if (value == 0)
				value = (void **)Animation_LookupValueByKey(work->anim_id);
			work->kind = info->draw_kind;
			work->scripts = info->animation;
			work->frames = value;
			work->mode = info->frame_codec;
			z = 0;
			work->frame = 0xff;
			work->script = (u8 *)z;
			work->pos = z;
		}
	}
}

s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id)
{
	s32						  value = (s32)state->entries[0];
	s32						  index = 0;
	struct AnimationEntry	**slot;
	struct AnimationMetadata *metadata;

	if (value != 0)
	{
		slot = &state->entries[0];
		do
		{
			index++;
			if (index > 3)
				break;
			slot++;
			value = (s32)*slot;
		} while (value != 0);
	}
	if (index == 4)
		return -1;
	value = (s32)AnimationObject_Allocate(id);
	if (value == 0)
		return 0;
	state->entries[index] = (struct AnimationEntry *)value;
	metadata = Resource_GetMetadataRecordFar(id);
	if (state->count == 0)
	{
		state->width = metadata->width;
		state->height = metadata->height;
		state->scale = metadata->scale << 8;
		state->offset_y = metadata->adjust_y;
		state->offset_x = metadata->adjust_x;
	}
	if (index == state->count)
		state->count = index + 1;
	return value;
}

void ResourceMetadata_Unregister(struct AnimationObject *state, s32 handle)
{
	s32 count;
	u32 slot;
	u32 i;

	if (state != NULL && handle != 0)
	{
		ResourceMetadata_ClearRecord(handle);
		for (slot = 0; slot <= 3; slot++)
		{
			if (handle == (s32)state->entries[slot])
				break;
		}
		if (slot != 4)
		{
			state->entries[slot] = 0;
			count = 0;
			for (i = slot + 1; i <= 3; i++)
			{
				if (state->entries[i] != 0)
					count++;
			}
			if (count == 0)
				state->count = slot;
		}
	}
}

void ResourceMetadata_ReleaseSlot(struct AnimationObject *group, u32 no)
{
	struct AnimationEntry *v;
	s32					   cnt;
	u32					   i;

	if (group != NULL && no <= 3)
	{
		v = group->entries[no];
		if (v != NULL)
		{
			ResourceMetadata_ClearRecord(v);
			group->entries[no] = NULL;
			cnt = 0;
			for (i = no + 1; i <= 3; i++)
			{
				if (group->entries[i] != NULL)
					cnt++;
			}
			if (cnt == 0)
				group->count = no;
		}
	}
}

void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no)
{
	s32						  hi;
	struct AnimationMetadata *info;
	s32						  value;

	hi = 0x80 & no;
	if (work->scripts != 0)
	{
		info = Resource_GetMetadataRecordFar(work->anim_id);
		if (no < info->animation_count)
		{
			value = (s32)work->scripts[no];
			work->kind = info->draw_kind;
			work->script = (u8 *)value;
			work->step = 0x10;
			if (hi == 0)
			{
				work->pos = hi;
				work->timer = hi;
			}
		}
	}
}

s32 AnimationObjects_SelectAnimation(struct AnimationObject *state, s32 flags)
{
	s32 high_bit;
	s32 index;

	high_bit = flags & 0x80;
	flags &= 0x7f;

	/* FAKEMATCH: the index/test loop keeps an 8-byte frame; a for loop spills an entry cursor and uses 12 bytes. */
	if (state->last_no != flags)
	{
		index = 0;
		goto test_entry;
	entry_loop:
		{
			struct AnimationEntry	 *entry = state->entries[index];
			struct AnimationMetadata *metadata;
			s32						  selected_animation;

			if (entry == 0)
				goto next_entry;
			if (entry->scripts == 0)
				goto next_entry;

			metadata = Resource_GetMetadataRecordFar(entry->anim_id);
			if (flags >= metadata->animation_count)
				goto next_entry;

			selected_animation = (s32)entry->scripts[flags];
			entry->kind = metadata->draw_kind;
			entry->script = (u8 *)selected_animation;
			entry->step = 0x10;
			if (high_bit == 0)
			{
				entry->pos = 0;
				entry->timer = 0;
			}
			if (index == 0)
			{
				state->offset_y = metadata->adjust_y;
				state->offset_x = metadata->adjust_x;
			}
			goto next_entry;
		}
	next_entry:
		index++;
	test_entry:
		if (index < state->count)
			goto entry_loop;
		state->last_no = flags;
	}
	return 0;
}

s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *group, s32 val)
{
	u8						n = group->count;
	struct AnimationEntry **p;
	s32						cnt;

	if (n != 0)
	{
		val <<= 4;
		p = group->entries;
		cnt = n;
		do
		{
			struct AnimationEntry *entry = *p++;
			if (entry != 0 && entry->scripts != 0)
				entry->timer = val;
			cnt--;
		} while (cnt != 0);
	}
	return 0;
}

void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *group, s32 val)
{
	u8 n = group->count;
	if (n != 0)
	{
		struct AnimationEntry **p = group->entries;
		s32						cnt = n;
		do
		{
			struct AnimationEntry *entry = *p++;
			if (entry != 0 && entry->scripts != 0)
				entry->step = val;
			cnt--;
		} while (cnt != 0);
	}
}

void ObjectSystem_Configure(s32 mode)
{
	void		*objects;
	void		*states;
	volatile u32 zero;

	if (mode == 3)
	{
		objects = Runtime_AllocateBlock(4, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
		states = Runtime_AllocateBlock(3, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
	}
	else
	{
		objects =
			Runtime_AllocateHeapBlock(4, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
		states =
			Runtime_AllocateHeapBlock(3, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
	}
	PaletteDma_LoadBlock();
	zero = 0;
	Dma_Set((const void *)&zero, objects,
		0x85000000 | (ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject) / 4), REG_DMA3);
	zero = 0;
	Dma_Set((const void *)&zero, states,
		0x85000000 | (ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry) / 4), REG_DMA3);
	VramBlock_LoadCached(93, 128, Object_ShadowTiles);
	RamCode_LoadSlot(53, Tile_CopyStrided, Tile_CopyStridedCodeSize);
}

struct AnimationEntry *AnimationObject_Allocate(s32 id)
{
	struct AnimationMetadata *metadata;
	struct AnimationEntry	 *entry;
	struct AnimationEntry	 *found;
	struct AnimationEntry	 *object;
	s32						  i;
	void					**frames;
	u8						**animation;

	found = NULL;
	metadata = Resource_GetMetadataRecordFar(id);
	entry = gAnimationObjects[0];
	object = NULL;

	if (metadata->width != 0)
	{
		for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, entry++)
		{
			if (entry->kind == 0)
			{
				found = entry;
				break;
			}
		}
		if (found != NULL)
		{
			/* FAKEMATCH: instruction order only: read as a word, the frames load does not wait on the anim_id store, so the zero for pos and param loads first as native; the typed read swaps the two (differ 2). */
			frames = (void **)*(s32 *)&metadata->frames;
			object = found;
			object->anim_id = id;
			if (frames == 0)
				frames = (void **)Animation_LookupValueByKey(id);

			animation = metadata->animation;
			object->scripts = animation;
			object->frames = frames;
			object->mode = metadata->frame_codec;
			object->frame = 0xff;
			object->script = animation[0];
			object->pos = 0;
			object->kind = metadata->draw_kind;
			object->param = 0;
		}
	}
	return object;
}

//--------------------------------------------------------------------------
// object.c
// リソース：アニメの記録を消す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// AnimationEntry 1 つ分を 0 で埋める（NULL なら何もしない）
//--------------------------------------------------------------------------
void ResourceMetadata_ClearRecord(void *dst)
{
	if (dst != 0)
	{
		volatile u32 zero;

		zero = 0;
		Dma_Set(&zero, dst,
			(DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32) | (sizeof(struct AnimationEntry) / 4),
			REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// resobj.c
// リソース：アニメのスプライトオブジェクトの作成・解放と、コマの長さ
//--------------------------------------------------------------------------

struct ResourceObjectWork
{
	u8	  unknown_00[28];
	u8	  resource; // VRAM キャッシュの番号
	u8	  flags;	// ビット 0：VRAM キャッシュを返さない
	u8	  unknown_1e[10];
	void *children[4];
};

typedef char ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

//--------------------------------------------------------------------------
// アニメ id のスプライトオブジェクトを作る
// あいている 64 個のオブジェクトの 1 つと VRAM キャッシュを使う
// 戻り値 : オブジェクト（作れなければ NULL）
//--------------------------------------------------------------------------
struct ResourceObject *ResourceObject_Create(s32 id)
{
	struct AnimationMetadata *meta;
	struct AnimationObject	 *obj;
	struct AnimationObject	 *found;
	u32						 *word;
	s32						  slot;
	s32						  tile;
	s32						  i;
	u32						  shape;

	found = NULL;
	meta = Resource_GetMetadataRecordFar(id);
	slot = Resource_FindFreeEntry();
	obj = gSpriteObjects;
	if (meta->width == 0)
		return NULL;
	for (i = 0; i <= 63; i++, obj++)
	{
		if (obj->width == 0) // 幅 0 はあき
		{
			found = obj;
			break;
		}
	}
	if (found == NULL)
		return NULL;
	if (slot == VRAM_CACHE_ENTRY_COUNT)
		return NULL;
	tile = VramBlock_LoadCached(slot, 0, 0);
	if (tile == 0)
		return NULL;
	found->slot = slot;
	found->rotation = 0;
	found->flags = 1;

	// 幅・高さ（ドット）から OBJ の形とサイズのビット（属性 0・1）
	switch ((u32)((meta->width << 8) + meta->height))
	{
	case 0x0808: // 8x8
		shape = 0;
		break;
	case 0x0810: // 8x16
		shape = 0x8000;
		break;
	case 0x1008: // 16x8
		shape = 0x4000;
		break;
	case 0x1010: // 16x16
		shape = 0x40000000;
		break;
	case 0x1020: // 16x32
		shape = 0x80008000;
		break;
	case 0x2010: // 32x16
		shape = 0x80004000;
		break;
	case 0x2020: // 32x32
		shape = 0x80000000;
		break;
	case 0x2040: // 32x64
		shape = 0xc0008000;
		break;
	case 0x4020: // 64x32
		shape = 0xc0004000;
		break;
	case 0x4040: // 64x64
		shape = 0xc0000000;
		break;
	default:
		shape = 0;
		break;
	}

	// パーツ 0：256 色、優先度 2
	// パーツ 1：16x8・256 色、VRAM キャッシュ 93 のタイル、優先度 2
	word = (u32 *)found->part;
	*word++ = 0;
	*word++ = shape | 0x2000;
	*word++ = tile | 0x800;
	*word++ = 0;
	*word++ = 0x6000;
	*word = (gVramBlockCache[93].offset >> 5) | 0x800;
	if (ResourceMetadata_Register(obj, id) == -1)
	{
		/* FAKEMATCH: native dead -1 after Register (movs/negs); empty arm
		   after a same-byte reload keeps the carrier (differ 32 -> 0). */
		found->dirty = found->dirty;
	}
	return (struct ResourceObject *)found;
}

//--------------------------------------------------------------------------
// オブジェクトを解放する
// VRAM キャッシュ（flags のビット 0 が立っていなければ）と 4 つの子を
// 返し、ワーク 56 バイトを 0 で埋める
//--------------------------------------------------------------------------
void ResourceObject_Release(struct ResourceObjectWork *work)
{
	volatile u32 zero;
	s32			 n;
	void	   **child;

	if (work)
	{
		if (!(work->flags & 1))
			Resource_ResetEntry(work->resource);
		child = work->children;
		n = 3;
		do
		{
			ResourceMetadata_ClearRecord(*child++);
		} while (--n >= 0);
		zero = 0;
		Dma_Set(&zero, work, (DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32) | (sizeof(*work) / 4),
			REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// アニメ id の no 番の、はじめの cnt コマの長さを足す
// コマンドは 2 バイト（コマンド・値）で、254・241・253・239 で終わる。
// 245・255・238 以下のコマンドの値を足す
//--------------------------------------------------------------------------
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt)
{
	struct AnimationMetadata *info;
	u8						 *p;
	u8						  op;
	u8						  val;
	s32						  sum = 0;

	info = Resource_GetMetadataRecordFar(id);
	if (no >= info->animation_count)
	{
		return 0;
	}
	p = ((u8 **)info->animation)[no];
	for (;;)
	{
		op = p[0];
		val = p[1];
		p += 2;
		if (op == 254 || op == 241 || op == 253 || op == 239)
		{
			break;
		}
		if (op == 245 || op == 255 || op <= 238)
		{
			sum += val;
			cnt--;
			if (cnt == 0)
			{
				break;
			}
		}
	}
	return sum;
}

//--------------------------------------------------------------------------
// object.c
// オブジェクト：フィールドのオブジェクトの作成・解放と、毎フレームの表示
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 絵を 1 ドットずつ消す処理の 1 回分
// 8×8 ドット（256 色）のキャラごとに、消す順の表にしたがって 1 ドット消す
// キャラごとに pos を 1 ずつずらし、pos が 64 から 127 のキャラだけ消す
// obj : 絵（AnimationObject）
// pos : 何回目か（0 から 127）
//--------------------------------------------------------------------------
void Map_RenderAnimatedTileFrame(u8 *obj, u32 pos)
{
	u16 *dst;
	s32	 cnt;
	u32	 n;
	u8	*tbl;
	u32	 high_mask;
	u32	 index_mask;
	u8	 offset_mask;

	struct AnimationObject *spr = (struct AnimationObject *)obj;

	dst = (u16 *)(0x06010000 + gVramBlockCache[spr->slot].offset); // OBJ の VRAM
	cnt = (spr->width * spr->height) / 64;
	n = 0;

	if (n < cnt)
	{
		tbl = Map_TileDissolveOrder;
		high_mask = 0xff00;
		index_mask = 0x3f;
		offset_mask = 0x3e;

		for (; n < cnt; n++, dst += 32, pos++)
		{
			if ((pos - 64) <= 63)
			{
				u8	 dot;
				u16 *cell;
				u32	 odd;

				// VRAM はバイトで書けないので、2 ドット分を読んで片方を消す
				dot = tbl[(pos + n * 16) & index_mask];
				cell = (u16 *)((u8 *)dst + (dot & offset_mask));
				odd = 1;
				odd &= dot;
				if (odd != 0)
					*cell = *(u8 *)cell;
				else
					*cell &= high_mask;
			}
		}
	}
}

//--------------------------------------------------------------------------
// 絵を 1 ドットずつ消す。1 フレームに 4 回分、32 フレームで消え終わる
// obj : 絵（AnimationObject）
//--------------------------------------------------------------------------
void Map_RenderAnimatedTileFramesForObject(u8 *obj)
{
	u32 pos;

	pos = 0;
	do
	{
		Map_RenderAnimatedTileFrame(obj, pos);
		Map_RenderAnimatedTileFrame(obj, pos + 1);
		Map_RenderAnimatedTileFrame(obj, pos + 2);
		Map_RenderAnimatedTileFrame(obj, pos + 3);
		pos += 4;
		WaitFrames(1);
	} while (pos <= 127);
}

//--------------------------------------------------------------------------
// いくつもの絵を、いっしょに 1 ドットずつ消す（32 フレーム）
// tbl : 絵の並び
// cnt : 絵の数
//--------------------------------------------------------------------------
void Map_RenderAllAnimatedTileFrames(u8 **tbl, s32 cnt)
{
	u8 **top;
	u8 **p;
	s32	 n;
	u32	 pos;
	u32	 pos1;
	u32	 pos2;
	u32	 pos3;

	top = tbl;
	pos = 0;
	do
	{
		if (cnt > 0)
		{
			pos1 = pos + 1;
			pos2 = pos + 2;
			p = top;
			pos3 = pos + 3;
			n = cnt;
			do
			{
				Map_RenderAnimatedTileFrame(*p, pos);
				Map_RenderAnimatedTileFrame(*p, pos1);
				Map_RenderAnimatedTileFrame(*p, pos2);
				n -= 1;
				Map_RenderAnimatedTileFrame(*p++, pos3);
			} while (n != 0);
		}
		WaitFrames(1U);
		pos += 4;
	} while (pos <= 0x7fU);
}

//--------------------------------------------------------------------------
// オブジェクトの画面上の位置（ドット）を out[0], out[1] に返す
// 画面のまわりの範囲にいれば 0、外なら 0, 0 として -1
// obj : オブジェクト
// out : x と y
//--------------------------------------------------------------------------
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out)
{
	struct MapScrollWork *map = gMapWork[0];
	s32					 *org = &map->view_x;
	s32					  ox;
	s32					  oz;
	s32					  x;
	s32					  y;

	ox = org[0] & 0xffff0000;
	oz = org[1] & 0xffff0000;
	x = obj->x - ox;
	y = obj->z - oz;
	if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000)
	{
		*out++ = x >> 16;
		*out = y >> 16;
		return 0;
	}
	*out++ = 0;
	*out = 0;
	return -1;
}

//--------------------------------------------------------------------------
// オブジェクトのしくみを用意する
// 64 個のオブジェクトの領域をとって 0 でうめ、毎フレームの処理を登録する
// mode : 4 なら動きの処理を Object_UpdateAllMotion で、3 と 4 は固定カメラ
//--------------------------------------------------------------------------
void ObjectSystem_Initialize(s32 mode)
{
	struct ObjectSystemWork *sys;
	struct ObjectRuntime	*objs;
	volatile u32			 fill;

	sys = Runtime_AllocateBlock(6, sizeof(*sys));
	objs = Runtime_AllocateBlock(5, 64 * sizeof(*objs));
	ObjectSystem_Configure(mode);
	fill = 0;
	Dma_Set((const void *)&fill, objs, 0x85000700, REG_DMA3); // 32 ビットでうめる
	fill = 0;
	Dma_Set((const void *)&fill, sys, 0x85000017, REG_DMA3);
	if (mode == 4)
		Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
	else
		Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
	if (mode >= 3 && mode <= 4)
	{
		Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
	}
	else
	{
		Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
		Iwram_Data2 = 0;
		Iwram_Data1 = 0;
	}
	sys->edge = 15;
	sys->fill = 0;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOpC0C4(void)
{
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOp2(void)
{
}

//--------------------------------------------------------------------------
// 64 個のうち、使われていない（スクリプトのない）オブジェクトを返す
// 空きがなければ 0
//--------------------------------------------------------------------------
void *ObjectDispatch_FindFreeObject(void)
{
	struct ObjectRuntime *obj = gObjectSlots;
	void				 *ret = 0;
	s32					  n = 0;

	while (n <= 63)
	{
		if (obj->script == 0)
		{
			ret = obj;
			break;
		}
		n++;
		obj++;
	}
	return ret;
}

//--------------------------------------------------------------------------
// オブジェクトの絵を解放し、オブジェクトを 0 でうめて空きにする
// obj : オブジェクト
//--------------------------------------------------------------------------
void ObjectDispatch_Release(struct DispatchObject *obj)
{
	volatile u32				zero;
	volatile u32			   *src;
	s32							n;
	struct ResourceObjectWork **child;

	if (obj)
	{
		switch (obj->kind & 15)
		{
		case 1: // 絵がひとつ
			ResourceObject_Release(obj->target.child);
			break;
		case 2: // 絵が 4 つまで
			child = (struct ResourceObjectWork **)obj->target.children;
			n = 3;
			do
			{
				struct ResourceObjectWork *res = *child++;
				if (res)
					ResourceObject_Release(res);
			} while (--n >= 0);
			break;
		}
		src = &zero;
		*src = 0;
		Dma_Set(src, obj, 0x85000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// フィールドのオブジェクトを作る。空きがなければ 0
// id      : 4096 で割った商が種類（0 絵ひとつ、2 絵ふたつ）、下の 12 ビットが絵
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z)
{
	struct ObjectRuntime	   *obj;
	struct FieldActor		   *act;
	struct ScriptMotionObject  *mot;
	struct ScriptObjectRuntime *sc;
	void					   *spr;
	s32							kind;
	void					  **ent;
	volatile u32				zero;

	ObjectDispatch_FindFreeObject();
	kind = id / 4096;
	id &= 0xfff;
	obj = ObjectDispatch_FindFreeObject();
	if (obj != NULL)
	{
		act = (struct FieldActor *)obj;
		act->radius = 16;
		switch (kind)
		{
		case 0:
			spr = ResourceObject_Create(id);
			if (spr != NULL)
			{
				obj->animation_kind = 1;
				obj->animation = spr;
				act->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
			}
			else
			{
				obj->animation_kind = 0;
			}
			break;
		case 2:
			// 絵の並び（4 つ分）をとって 0 でうめ、id と id + 1 の絵を入れる
			ent = (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
			obj->animation_kind = kind;
			zero = 0;
			obj->animation = ent;
			Dma_Set(&zero, ent, 0x85000004, REG_DMA3);
			spr = ResourceObject_Create(id);
			if (spr != NULL)
			{
				act->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
				*ent++ = spr;
			}
			spr = ResourceObject_Create(id + 1);
			if (spr != NULL)
				*ent = spr;
			break;
		}
	}
	if (obj != NULL)
	{
		mot = (struct ScriptMotionObject *)obj;
		sc = (struct ScriptObjectRuntime *)obj;
		Object_SetPositionAndResetMotion(obj, x, y, z);
		obj->script = (s32 *)ObjectDispatch_DefaultScript;
		obj->speed_limit = 0x20000;
		obj->step = 0;
		act->scale_x = 0x10000;
		act->scale_y = 0x10000;
		obj->acceleration = 0x10000;
		obj->flags = 3;
		mot->gravity = 0x10000;
		mot->vertical.bounce = 0x4000;
		obj->unknown_59 = 0;
		obj->action_flags = 1;
		act->rise_base_y = 0;
		obj->angle = 0x4000;
		sc->home_x = x / 65536; // もとの位置（ドット）
		sc->home_z = z / 65536;
	}
	return obj;
}

//--------------------------------------------------------------------------
// オブジェクトにスクリプトを与え、はじめから動かす
// obj    : オブジェクト
// script : スクリプト
//--------------------------------------------------------------------------
void ObjectDispatch_Initialize(struct DispatchObject *obj, const s32 *script)
{
	if (obj != 0)
	{
		obj->value_04 = 0;
		obj->value_00 = (u32)script;
		obj->value_5b = 0;
		obj->value_5d = 0;
		obj->value_57 = 0;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつならその絵に、絵の組ならある絵すべてに apply(絵, value) を呼ぶ
//--------------------------------------------------------------------------
#define DEFINE_APPLY_TO_CHILDREN(name, object_type, apply) \
	void name(object_type raw, s32 value)                  \
	{                                                      \
		struct DispatchObject *obj;                        \
		void				 **list;                       \
		s32					   n;                          \
		void				  *spr;                        \
                                                           \
		obj = raw;                                         \
		if (obj != 0)                                      \
		{                                                  \
			switch (obj->kind & 0xf)                       \
			{                                              \
			case 1:                                        \
				apply(obj->target.child, value);           \
				break;                                     \
			case 2:                                        \
				list = obj->target.children;               \
				n = 3;                                     \
				do                                         \
				{                                          \
					spr = *list++;                         \
					if (spr != 0)                          \
						apply(spr, value);                 \
					n--;                                   \
				} while (n >= 0);                          \
				break;                                     \
			}                                              \
		}                                                  \
	}

// オブジェクトの絵（すべて）のアニメを no にする
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyArgumentToChildren, void *,
	AnimationObjects_SelectAnimation)

// オブジェクトの絵（すべて）の AnimationObjects_SetActiveTimerSteps を呼ぶ
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyValueToChildren, struct DispatchObject *,
	AnimationObjects_SetActiveTimerSteps)

//--------------------------------------------------------------------------
// 上のふたつをいっしょに：アニメを no にし、steps をわたす
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *obj, s32 no, s32 steps)
{
	void **list;
	void  *spr;
	s32	   n;

	if (obj != 0)
	{
		switch (obj->kind & 15)
		{
		case 1:
			AnimationObjects_SelectAnimation(obj->target.child, no);
			AnimationObjects_SetActiveTimerSteps(obj->target.child, steps);
			break;
		case 2:
			list = obj->target.children;
			for (n = 3; n >= 0; n--)
			{
				spr = *list++;
				if (spr != 0)
				{
					AnimationObjects_SelectAnimation(spr, no);
					AnimationObjects_SetActiveTimerSteps(spr, steps);
				}
			}
			break;
		}
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の回転を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildRotation(struct DispatchObject *obj, u32 rot)
{
	if (obj != 0 && (obj->kind & 0xf) == 1)
		((struct FieldSprite *)obj->target.child)->rotation = rot;
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、アニメ番号を no にして初めから動かす
// no が負ならなにもしない
//--------------------------------------------------------------------------
void Animation_SetIndexAndInitObjects(void *raw, s32 no)
{
	struct DispatchObject  *obj = raw;
	struct AnimationObject *spr;

	if (obj != NULL && (obj->kind & 0xf) == 1)
	{
		spr = obj->target.child;
		if (no >= 0)
		{
			spr->entries[0]->anim_id = no;
			Animation_InitializeObjects(spr);
		}
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、その絵に no の情報を登録する
// no が負ならなにもしない
//--------------------------------------------------------------------------
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *obj, s32 no)
{
	if (obj != 0 && (obj->kind & 0xf) == 1)
	{
		struct AnimationObject *spr = obj->target.child;
		if (no >= 0)
			ResourceMetadata_Register(spr, no);
	}
}

//--------------------------------------------------------------------------
// 決まったスクリプトでオブジェクトを動かしはじめる
//--------------------------------------------------------------------------
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *obj, s32 arg)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table5Script);
	obj->argument = arg;
}

void ObjectDispatch_InitFromTable0(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table0Script);
}

void ObjectDispatch_InitFromTable1(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table1Script);
}

void ObjectDispatch_InitFromTable2(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table2Script);
}

void ObjectDispatch_InitFromTable3(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table3Script);
}

void ObjectDispatch_InitFromTable6(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table6Script);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *obj, s32 arg)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table4Script);
	if (arg != 0)
	{
		obj->value_34 = 0x8000;
		obj->value_30 = 0x40000;
		obj->argument = arg;
		obj->value_64 = 0;
	}
}

//--------------------------------------------------------------------------
// スクリプトが命令 0x10 に来るまで待つ（300 フレームまで）
//--------------------------------------------------------------------------
void ObjectDispatch_WaitForValue16(void *raw)
{
	struct ScriptInterpreter *sc = raw;
	s32						  cnt;

	cnt = 0;
	if (sc->script[sc->cursor] != 0x10)
	{
		do
		{
			WaitFrames(1);
			cnt++;
			if (cnt > 299)
				break;
		} while (sc->script[sc->cursor] != 0x10);
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の flags を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *obj, u32 flags)
{
	if (obj != NULL)
	{
		if ((obj->kind & 0xf) == 1)
			((struct AnimationObject *)obj->target.child)->flags = flags;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の半透明のしかたを設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *obj, u32 mode)
{
	if (obj != 0 && obj->kind == 1)
	{
		struct FieldSprite *spr = obj->target.child;
		spr->blend_mode = mode;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の display_flags の bit1 を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *obj, u32 on)
{
	if (obj != 0 && obj->kind == 1)
	{
		struct FieldSprite *spr = obj->target.child;
		spr->unknown_1d_1 = on;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、ObjectGroup_SetChildValueUnlessFifteen を呼ぶ
//--------------------------------------------------------------------------
void Animation_ApplyChildValues(struct DispatchObject *obj, u32 val)
{
	if (obj != NULL && obj->kind == 1)
		ObjectGroup_SetChildValueUnlessFifteen(obj->target.child, val);
}

//--------------------------------------------------------------------------
// カメラの処理を動かし、BG1-3 を消して OBJ を出す
//--------------------------------------------------------------------------
void Graphics_EnableObjLayerAndCallbacks(void)
{
	Scheduler_EnableCallbacks(ObjectSystem_UpdateCamera);
	Scheduler_EnableCallbacks(ObjectSystem_UpdateCameraFixed);
	BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
	BattleFx_StartBufferInterpolationFar(1);
	WaitFrames(1);
	REG_DISPCNT = (REG_DISPCNT & 0xf1ff) | DISPCNT_OBJ_ON;
}

//--------------------------------------------------------------------------
// カメラの処理を止め、BG1-3 と OBJ を消す
//--------------------------------------------------------------------------
void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
	Scheduler_DisableCallbacks(ObjectSystem_UpdateCamera);
	Scheduler_DisableCallbacks(ObjectSystem_UpdateCameraFixed);
	REG_DISPCNT &= 0xe1ff;
}

s32 ObjectDispatch_ReturnTrue(void)
{
	return 1;
}

// マップのセル
struct CameraTile
{
	u32 unk_00 : 12;
	u32 layer : 2;	  // 0 でなければ、上にいるオブジェクトのレイヤー + 1
	u32 priority : 2; // 0 でなければ、上にいるオブジェクトの表示の優先度
	u32 unk_10 : 16;
};

// オブジェクトの絵（OAM の属性をふくむ）
struct CameraSprite
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 blend_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 flip_x : 1;
	u16 flip_y : 1;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[0x0a];
	u16 second_tile : 10;
	u16 second_priority : 2;
	u16 second_palette : 4;
	u8	unknown_16[2];
	s32 scale;
	u8	resource; // VRAM ブロックの番号
	u8	flags_1d;
	u8	unk_1e[7];
	u8	activated;
};

// オブジェクト
struct CameraObject
{
	u32	  active;
	u16	  unk_04;
	u16	  angle;
	s32	  x;
	s32	  y;
	s32	  z;
	s32	  height;
	s32	  scale_x;
	s32	  scale_y;
	u8	  unk_20[2];
	u8	  layer;
	u8	  flags;
	u8	  unk_24[0x2c];
	void *sprite;
	u8	  kind; // 1 絵ひとつ、2 絵が 4 つまで
	u8	  unk_55[7];
	u8	  held;
	u8	  unk_5d[0x13];
};

struct CameraLayer
{
	struct CameraTile *tiles;
	u8				   unk_04[44];
};

// マップ
struct CameraState
{
	u8				   unk_000[0xe4];
	s32				   x; // 見ている位置
	s32				   z;
	u8				   unk_0ec[0x44];
	struct CameraLayer layers[1];
};

struct CameraPoint
{
	s32 x;
	s32 y;
	s32 z;
};

// 固定カメラ
struct FixedCamera
{
	struct CameraPoint	eye;
	struct CameraPoint	target;
	struct CameraPoint *eye_override;	 // あればこちらの視点を使う
	struct CameraPoint *target_override; // あればこちらの注視点を使う
};

//--------------------------------------------------------------------------
// 毎フレーム：オブジェクトの絵を画面に置く（ふつうのカメラ）
// 画面のまわりにいるオブジェクトは、足もとのセルからレイヤーと優先度を決めて
// 置く。外にいれば絵の VRAM ブロックを手放す
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCamera(void)
{
	/* FAKEMATCH: needed to match: unused; the native frame keeps room for three points above the position. */
	struct CameraPoint		 spare[3];
	s32						 cnt;
	struct CameraState		*map;
	s32						 cam_x;
	s32						 cam_z;
	struct ObjectSystemWork *sync;
	struct CameraObject		*obj;
	struct CameraSprite		*spr;
	s32						*cam;
	u32						 kind;
	s32						 dx;
	s32						 dz;
	s32						 top;
	struct CameraTile		*tile;
	u32						 bits;
	s32						 scale[2];
	s32						 pos[4];
	u32						 bits_val;

	map = *(struct CameraState **)(((u8 *)&gObjectSlots) + 12);
	cam = &map->x;
	cam_x = cam[0] & 0xffff0000;
	cam_z = cam[1] & 0xffff0000;
	sync = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
	// 絵を置く処理（Render_DecodeFrame）を作業領域へ写す
	RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
	sync->used = 0;
	obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
	for (cnt = 0; cnt < 64; cnt++, obj++)
	{
		if (obj->active == 0)
			continue;
		if (obj->x != 0 || obj->z != 0)
		{
			kind = obj->kind & 15;
			if (kind == 0)
				continue;
			if (kind != 1)
				continue;
			if (sync->suspended != 0 && obj->held == 0)
			{
				struct CameraSprite *frozen = obj->sprite;

				Resource_ActivateEntry(frozen->resource);
				frozen->activated = kind;
				continue;
			}
			dx = obj->x - cam_x;
			dz = obj->z - cam_z;
			top = dz - obj->y;
			spr = obj->sprite;
			if (dx > -0x200000 && dx < 0x1100000 && top > -0x200000 && top < 0xe00000)
			{
				// 足もとのセルの情報
				tile = &map->layers[obj->layer].tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
				if (obj->flags & 1)
				{
					bits = tile->priority;
					if (bits != 0)
					{
						spr->priority = bits;
						spr->second_priority = bits;
					}
				}
				bits_val = tile->layer;
				if (bits_val != 0)
					obj->layer = bits_val - 1;
				scale[0] = Iwram_MulQ16(obj->scale_x, spr->scale);
				scale[1] = Iwram_MulQ16(obj->scale_y, spr->scale);
				pos[0] = dx;
				pos[1] = obj->y;
				pos[2] = dz;
				pos[3] = obj->height;
				if (obj->flags & 2)
				{
					pos[1] += 0xfec00000; // -20 ドット
					pos[2] += 0xfec00000;
					pos[3] += 0xfec00000;
				}
				if (obj->flags & 4)
				{
					pos[1] += 0x1400000; // +20 ドット
					pos[2] += 0x1400000;
					pos[3] += 0x1400000;
				}
				Render_ApplyProjectedPlacement(spr, pos, scale, obj->angle);
				continue;
			}
			if (obj->held == 0)
			{
				if (!(spr->flags_1d & 1))
				{
					Resource_ActivateEntry(spr->resource);
					spr->activated = 1;
				}
			}
		}
		else
		{
			kind = obj->kind & 15;
			if (kind == 1)
			{
				spr = obj->sprite;
				if (obj->held == 0 && !(spr->flags_1d & 1))
				{
					Resource_ActivateEntry(spr->resource);
					spr->activated = kind;
				}
			}
		}
	}
	Runtime_ReleaseHeapBlock(52);
}

s32 ObjectCamera_ReturnTrue(void)
{
	return 1;
}

//--------------------------------------------------------------------------
// 毎フレーム：オブジェクトの絵を画面に置く（固定カメラ）
// 視点と注視点から向きを決め、64 番目のオブジェクトから順に置く
// コマンドを選んでいるあいだ（フラグ 363）は、決まった向きの行列を使い
// 大きさを 1 倍にする
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCameraFixed(void)
{
	s32						 cnt;
	struct FixedCamera		*cam;
	struct ObjectSystemWork *sync;
	struct CameraPoint		*eye;
	struct CameraPoint		*tgt;
	struct CameraObject		*obj;
	s32						 angle;
	s32						 part;
	s32						 kind;
	s32						*pos;
	s32						 one;
	void				   **parts;
	void					*spr;
	s32						 scale2[2];
	s32						 scale[2];
	/* FAKEMATCH: needed to match: unused; the native frame keeps room for two points above the scales. */
	struct CameraPoint spare[2];

	cam = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
	sync = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
	RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
	eye = &cam->eye;
	tgt = &cam->target;
	if (cam->eye_override != NULL)
		eye = cam->eye_override;
	if (cam->target_override != NULL)
		tgt = cam->target_override;
	angle = (s16)ArcTan2((eye->x - tgt->x) >> 16, (eye->z - tgt->z) >> 16);
	sync->used = 0;
	Render_ResetTransformState();
	if (GameFlag_TestFar(363))
	{
		angle += 0xffffe000;
		Iwram_TransformMatrix(Camera_FixedViewMatrix);
		Graphics_PrepareTransferAndRun(eye, tgt);
	}
	else
	{
		Graphics_PrepareTransferInIwramWork(eye, tgt);
	}
	obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
	obj += 63;
	one = 0x10000;
	for (cnt = 63; cnt >= 0; cnt--, obj--)
	{
		if (obj->active == 0)
			continue;
		kind = obj->kind & 15;
		switch (kind)
		{
		case 1:
			pos = &obj->x;
			scale[0] = obj->scale_x;
			scale[1] = obj->scale_y;
			spr = obj->sprite;
			if (GameFlag_TestFar(363))
			{
				scale[0] = one;
				scale[1] = one;
			}
			Render_PlaceProjectedSprite(spr, pos, scale, obj->angle + angle, obj->layer);
			break;
		case 2:
			scale2[0] = obj->scale_x;
			scale2[1] = obj->scale_y;
			if (GameFlag_TestFar(363))
			{
				scale2[0] = one;
				scale2[1] = one;
			}
			parts = obj->sprite;
			for (part = 3; part >= 0; part--)
			{
				spr = *parts++;
				if (spr != NULL)
					Render_PlaceProjectedSprite(
						spr, &obj->x, scale2, obj->angle + angle, obj->layer);
			}
			break;
		case 0:
			break;
		}
	}
	Runtime_ReleaseHeapBlock(52);
}

//--------------------------------------------------------------------------

// script.c

// オブジェクトスクリプト：位置の移動と byte_54 の命令

//--------------------------------------------------------------------------
