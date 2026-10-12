#include "unit/field/objdata.h"
#include "unit/field/facing.h"
#include "unit/field/field.h"
#include "unit/field/script.h"
#include "unit/field/operands.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/oamlist.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/basepal.h"
#include "unit/runtime/heap.h"
#include "unit/runtime/matrix.h"
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
	if (gMenuCtrlWork->suspended == 0)
	{
		z = Render_ProjectPoint(point, screen);
		if (screen[2] != 0 && screen[0] >= -32 && screen[0] <= 272 && screen[1] >= -32
			&& screen[1] <= 208)
		{
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
		}
	}
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

			if (entry != 0 && entry->scripts != 0)
			{
				metadata = Resource_GetMetadataRecordFar(entry->anim_id);
				if (flags < metadata->animation_count)
				{
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
				}
			}
		}
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
