#include "unit/field/facing.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
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
#include "unit/field/update.h"
#include "unit/field/vertices.h"
#include "unit/field/curvedat.h"
#include "unit/field/griddata.h"

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
	u32		  control;
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
	base = literal;
	start = destination;
	command++;
	control |= 0x100; // 8 ビット読み終えたしるし
	for (;;)
	{
		if ((control & 1) != 0)
		{
			if (control != 1) // 残っているのがしるしだけなら次の制御バイトへ
				*destination++ = *literal++;
		}
		else
		{
			token = *command++ << 8;
			token |= *command++;
			if (token == 0)
				break;
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
		}
		control >>= 1;
		if (control == 0)
		{
			control = *command;
			command++;
			control |= 0x100;
		}
	}
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
		if ((s16)e->timer <= 0)
		{
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
		}
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

	/* FAKEMATCH: the index/test loop keeps an 8-byte frame; a for loop spills an
	 * entry cursor and uses 12 bytes. 2026-10-13: for (index = 0; index <
	 * state->count; index++) strength-reduces entries[index] into that cursor
	 * (82 instructions for 76, differ 30). */
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
	Dma_Fill32Cell(zero, 0, objects, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
	Dma_Fill32Cell(zero, 0, states, ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
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
		Dma_Fill32(0, dst, sizeof(struct AnimationEntry));
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
		Dma_Fill32(0, work, sizeof(*work));
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
	Dma_Fill32Cell(fill, 0, objs, 64 * sizeof(*objs));
	Dma_Fill32Cell(fill, 0, sys, sizeof(*sys));
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
		Dma_Fill32(0, obj, sizeof(struct ObjectRuntime));
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
			obj->animation = ent;
			Dma_Fill32(0, ent, 0x10);
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
	/* FAKEMATCH: frame local: unused; the native frame keeps room for
	   three points above the position. Measured (2026-10-13): without it
	   the frame is 44 bytes for 80 (differ 4, the two sp adjustments). */
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
	/* FAKEMATCH: frame local: unused; the native frame keeps room for two
	   points above the scales. Measured (2026-10-13): without it the frame
	   is 28 bytes for 52 (differ 4, the two sp adjustments). */
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

//--------------------------------------------------------------------------
// スクリプトの読む位置を移し、1 を返して次の命令を続けて実行する
//--------------------------------------------------------------------------
s32 Script_AdvancePastCursor(struct ScriptInterpreter *sc)
{
	s32 zero;

	// 命令の次の語を新しい先頭にする
	sc->script = sc->script + sc->cursor + 1;
	zero = 0;
	sc->cursor = zero;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 0 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_ClearByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 0;
	sc->cursor++;
	return 1;
}

//--------------------------------------------------------------------------
// byte_54 を 1 にして、次の命令へ
//--------------------------------------------------------------------------
s32 Script_SetByte54(struct ScriptInterpreter *sc)
{
	sc->byte_54 = 1;
	sc->cursor++;
	return 1;
}

void Script_WaitForEventTimeout(s32 arg0)
{
	s32 cnt;

	cnt = 0;
	while (cnt <= 0x257 && Object_IsTargetUnset((struct ObjectRuntime *)arg0) == 0)
	{
		WaitFrames(1);
		cnt++;
	}
}

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
	s32 first;
	s32 second;

	if (object->flags == 0)
	{
		second = object->target_x;
		if (second != 0x80000000)
			return 0;
		first = object->target_y;
	}
	else
	{
		first = object->target_x;
		second = 0x80000000;
	}
	if (first != second || object->target_z != first)
		return 0;
	return 1;
}

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern ScriptCommandFn ScriptObject_CommandTable[];

static __inline__ s32 Square(s32 value)
{
	return value * value;
}

void Object_UpdateAllThumb(void)
{
	struct ScriptMotionObject	   *obj;
	const struct ScriptMotionWords *script;
	s32								cnt;
	s32								px;
	s32								py;
	s32								pz;
	s32								ground;
	s32								reached;
	s32								pos[3];
	s32								dx;
	s32								dy;
	s32								dz;
	s32								vx;
	s32								vy;
	s32								vz;
	s32								dist;
	s32								len;
	s32								ratio;
	s32								rem;
	s32								half;
	s32								work;
	u32								cmd;
	u32								phase;

	obj = gObjectSlots;
	for (cnt = 63; cnt >= 0; cnt--, obj++)
	{
		reached = 0;

		script = obj->script;
		if (script == NULL)
			continue;

		if (obj->hook != NULL)
		{
			obj->hook(obj);
			script = obj->script;
		}
		if (script == NULL)
			continue;
		if (obj->paused != 0)
			continue;

		if (obj->wait != 0)
		{
			obj->wait--;
		}
		else
		{
			for (;;)
			{
				cmd = script->word[(s16)obj->script_cursor];
				if (cmd <= 63)
				{
					if (ScriptObject_CommandTable[cmd](obj) == 0)
						break;
				}
				else
				{
					obj->script_cursor++;
				}
				script = obj->script;
			}
			if (obj->script == NULL)
				continue;
		}

		px = obj->x;
		py = obj->y;
		pz = obj->z;

		if (obj->frozen == 0)
		{
			if (obj->motion_flags == 0)
			{
				if (obj->target_x != TARGET_UNSET)
				{
					dx = (obj->target_x - px) / 65536;
					dy = (obj->target_y - py) / 65536;
					dz = (obj->target_z - pz) / 65536;
					dist = Iwram_Sqrt(Square(dx) + Square(dy) + Square(dz));
					if (dist == 0)
					{
						px = obj->target_x;
						py = obj->target_y;
						pz = obj->target_z;
					}
					else
					{
						ratio = Iwram_RatioMulQ14(dist << 16, obj->acceleration);
						obj->velocity_x += dx * ratio;
						obj->velocity_y += dy * ratio;
						obj->velocity_z += dz * ratio;
						vx = obj->velocity_x;
						vy = obj->velocity_y;
						vz = obj->velocity_z;
						len = FixedSqrt(
							Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vy, vy) + Iwram_MulQ16(vz, vz));
						if (len > obj->speed_limit)
						{
							ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
							obj->velocity_x = Iwram_MulQ16(vx, ratio);
							obj->velocity_y = Iwram_MulQ16(vy, ratio);
							obj->velocity_z = Iwram_MulQ16(vz, ratio);
						}
					}
				}
				else
				{
					dx = obj->velocity_x;
					dy = obj->velocity_y;
					dz = obj->velocity_z;
					len = FixedSqrt(
						Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
					if (len != 0)
					{
						rem = len - obj->acceleration;
						if (rem < 0)
							rem = 0;
						ratio = Iwram_RatioMulQ14(len, rem);
						obj->velocity_x = Iwram_MulQ16(dx, ratio);
						obj->velocity_y = Iwram_MulQ16(dy, ratio);
						obj->velocity_z = Iwram_MulQ16(dz, ratio);
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_y = 0;
						obj->velocity_z = 0;
					}
				}
			}
			else
			{
				if (obj->target_x != TARGET_UNSET)
				{
					dx = (obj->target_x - px) / 65536;
					dz = (obj->target_z - pz) / 65536;
					dist = Iwram_Sqrt(Square(dx) + Square(dz)) << 16;
					if (dist <= 0x00ffffff)
					{
						dx = obj->target_x - px;
						dz = obj->target_z - pz;
						dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
					}
					if (dist == 0)
					{
						px = obj->target_x;
						pz = obj->target_z;
					}
					else
					{
						ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
						obj->velocity_x += Iwram_MulQ16(dx, ratio);
						obj->velocity_z += Iwram_MulQ16(dz, ratio);
						vx = obj->velocity_x;
						vz = obj->velocity_z;
						len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vz, vz));
						if (len > obj->speed_limit)
						{
							ratio = Iwram_RatioMulQ14(len, obj->speed_limit);
							obj->velocity_x = Iwram_MulQ16(vx, ratio);
							obj->velocity_z = Iwram_MulQ16(vz, ratio);
						}
					}
				}
				else
				{
					dx = obj->velocity_x;
					dz = obj->velocity_z;
					len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
					if (len != 0)
					{
						rem = len - obj->acceleration;
						if (rem < 0)
							rem = 0;
						ratio = Iwram_RatioMulQ14(len, rem);
						obj->velocity_x = Iwram_MulQ16(dx, ratio);
						obj->velocity_z = Iwram_MulQ16(dz, ratio);
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_z = 0;
					}
				}

				if ((obj->motion_flags & 1) != 0)
				{
					vx = px + obj->velocity_x;
					vz = pz + obj->velocity_z;
					ground = Map_GetTerrainHeight(obj->terrain_id, vx, vz);
					vy = ground - obj->terrain_height;
					if (ground - py > -0x40000)
						py += vy;
					if (vy < 0)
						vy = -vy;
					half = obj->acceleration / 2;
					if (vy > half)
						vy = half;
					vy *= 3;
					if (vy != 0 && (obj->motion_flags & 0x10) == 0)
					{
						dx = obj->velocity_x;
						dy = obj->velocity_y;
						dz = obj->velocity_z;
						len = FixedSqrt(
							Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
						if (len != 0)
						{
							rem = len - vy;
							if (rem < 0)
								rem = 0;
							ratio = Iwram_RatioMulQ14(len, rem);
							obj->velocity_x = Iwram_MulQ16(dx, ratio);
							obj->velocity_y = Iwram_MulQ16(dy, ratio);
							obj->velocity_z = Iwram_MulQ16(dz, ratio);
						}
					}
					obj->terrain_height = ground;
				}

				if ((obj->motion_flags & 2) != 0)
				{
					ground = obj->terrain_height;
					if (py > ground)
					{
						obj->velocity_y -= obj->gravity;
					}
					else if (obj->velocity_y < 0)
					{
						py = ground;
						obj->velocity_y = -Iwram_MulQ16(obj->velocity_y, obj->vertical.bounce);
						if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y)
							<= obj->gravity)
							obj->velocity_y = 0;
					}
				}

				if ((obj->motion_flags & 4) != 0)
				{
					phase = (obj->vertical.phase & 0x3f);
					if ((obj->motion_flags & 8) != 0)
						obj->velocity_y = ScriptObject_CycleTable[phase >> 1] * obj->gravity / 16;
					else
						obj->velocity_y = ScriptObject_CycleTable[phase >> 1] * obj->gravity / 64;
					obj->vertical.phase++;
				}
			}
		}

		px += obj->velocity_x;
		py += obj->velocity_y;
		pz += obj->velocity_z;

		if ((obj->collision_flags & 0x80) != 0)
		{
			pos[0] = px;
			pos[1] = py;
			pos[2] = pz;
			if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)obj, pos) != 0)
			{
				obj->refused++;
				continue;
			}
			obj->refused = 0;
		}

		switch (obj->arrival_axis)
		{
		case 16:
			if (px == obj->target_x || ((obj->x - obj->target_x) ^ (px - obj->target_x)) < 0)
				reached = 1;
			break;
		case 17:
			if (py == obj->target_y || ((obj->y - obj->target_y) ^ (py - obj->target_y)) < 0)
				reached = 1;
			break;
		case 18:
			if (pz == obj->target_z || ((obj->z - obj->target_z) ^ (pz - obj->target_z)) < 0)
				reached = 1;
			break;
		default:
			break;
		}

		if (reached != 0)
		{
			if (obj->snap_to_target != 0)
			{
				px = obj->target_x;
				obj->velocity_x = 0;
				pz = obj->target_z;
				obj->velocity_z = 0;
				if (obj->motion_flags == 0)
				{
					py = obj->target_y;
					obj->velocity_y = 0;
				}
			}
			obj->target_x = TARGET_UNSET;
			obj->target_y = TARGET_UNSET;
			obj->target_z = TARGET_UNSET;
			obj->arrival_axis = 0;
		}

		obj->x = px;
		obj->y = py;
		obj->z = pz;

		if ((obj->steering_flags & 1) != 0)
		{
			px = obj->velocity_x;
			pz = obj->velocity_z;
			if (px != 0 || pz != 0)
			{
				s32 limit;

				work = (s16)(ArcTan2(pz, px) - obj->facing);
				limit = 0x1000;
				if (work > limit)
					work = limit;
				limit = -0x1000;
				if (work < limit)
					work = limit;
				obj->facing += work;
			}
		}
	}
}

//--------------------------------------------------------------------------
// motion_set_position_and_reset_motion.c
// オブジェクト：位置の設定と移動の取り消し
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 位置を置き、移動先を「なし」に、速度を 0 にする
// obj     : オブジェクト
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y, s32 z)
{
	obj->z = z;
	obj->target_x = 0x80000000; // 移動先なし
	obj->target_y = 0x80000000;
	obj->target_z = 0x80000000;
	obj->x = x;
	obj->y = y;
	obj->velocity_x = 0;
	obj->velocity_y = 0;
	obj->velocity_z = 0;
}

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
	s32 dx;
	s32 dist;
	s32 dz;
	s32 total;
	u8 *axis;

	dx = (x - object->x) / 0x10000;
	dist = (y - object->y) / 0x10000;
	dz = (z - object->z) / 0x10000;
	total = dz * dz;
	dist = Iwram_Sqrt(dx * dx + dist * dist + total) << 16;
	if (dist < 0x100000)
	{
		dx = x - object->x;
		dist = y - object->y;
		dz = z - object->z;
		dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) + Iwram_MulQ16(dz, dz));
	}
	if (dist < 0x10000)
	{
		object->x = x;
		object->y = y;
		object->z = z;
		object->target_x = 0x80000000;
		object->target_y = 0x80000000;
		object->target_z = 0x80000000;
		return;
	}
	if (object->snap_to_target == 0)
	{
		s32 brake;

		brake = Iwram_RatioMulQ14(
			object->acceleration, Iwram_MulQ16(object->speed_limit, object->speed_limit));
		if (dist > brake)
			brake = dist - brake / 2;
		else
			brake = dist / 2;
		dist = Iwram_RatioMulQ14(dist, brake);
		x = object->x + Iwram_MulQ16(x - object->x, dist);
		y = object->y + Iwram_MulQ16(y - object->y, dist);
		z = object->z + Iwram_MulQ16(z - object->z, dist);
	}
	object->target_x = x;
	object->target_y = y;
	object->target_z = z;
	dx = x - object->x;
	dist = y - object->y;
	dz = z - object->z;
	axis = &object->arrival_axis;
	*axis = 16;
	if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz))
	{
		*axis = 18;
		dx = dz;
	}
	if (object->flags == 0)
	{
		if (dx < 0)
			dx = -dx;
		if (dx < (dist < 0 ? -dist : dist))
			*axis = 17;
	}
}

void Object_RunUpdateAllFromHeap(void)
{
	void (*routine)(void);

	RamCode_Load(routine, Object_UpdateAll, Object_UpdateAllCodeSize);
	routine();
	MemFree(routine);
}

void Object_UpdateAllMotion(void)
{
	struct ScriptMotionObject *obj;
	s32						   cnt;
	s32						   y;
	s32						   arrived;
	s32						   x;
	s32						   z;
	s32						   dx;
	s32						   dz;
	s32						   vx;
	s32						   dist;
	s32						   ratio;
	s32						   turn;

	obj = gObjectSlots;
	for (cnt = 13; cnt >= 0; cnt--, obj++)
	{
		if (obj->script == 0)
			continue;
		x = obj->x;
		y = obj->y;
		z = obj->z;
		if (obj->frozen == 0)
		{
			arrived = 0;
			if (obj->target_x != TARGET_UNSET)
			{
				dx = (obj->target_x - x) / 65536;
				dz = (obj->target_z - z) / 65536;
				dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
				if (dist <= 0xffffff)
				{
					dx = obj->target_x - x;
					dz = obj->target_z - z;
					dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
				}
				if (dist == 0)
				{
					x = obj->target_x;
					z = obj->target_z;
				}
				else
				{
					ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
					vx = obj->velocity_x + Iwram_MulQ16(dx, ratio);
					obj->velocity_x = vx;
					dx = obj->velocity_z + Iwram_MulQ16(dz, ratio);
					obj->velocity_z = dx;
					dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
					if (dist > obj->speed_limit)
					{
						ratio = Iwram_RatioMulQ14(dist, obj->speed_limit);
						obj->velocity_x = Iwram_MulQ16(vx, ratio);
						obj->velocity_z = Iwram_MulQ16(dx, ratio);
					}
				}
			}
			else
			{
				dx = obj->velocity_x;
				dz = obj->velocity_z;
				if ((dx | dz) != 0)
				{
					dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
					if (dist != 0)
					{
						if (dist - obj->acceleration < 0)
						{
							obj->velocity_x = 0;
							obj->velocity_z = 0;
						}
						else
						{
							ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
							obj->velocity_x = Iwram_MulQ16(dx, ratio);
							obj->velocity_z = Iwram_MulQ16(dz, ratio);
						}
					}
					else
					{
						obj->velocity_x = 0;
						obj->velocity_z = 0;
					}
				}
			}
			if (obj->motion_flags & 2)
			{
				if (y > obj->terrain_height)
				{
					obj->velocity_y -= obj->gravity;
				}
				else if (obj->velocity_y < 0)
				{
					y = obj->terrain_height;
					obj->velocity_y = -Iwram_MulQ16(obj->velocity_y, obj->vertical.bounce);
					if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
						obj->velocity_y = 0;
				}
			}
		}
		x += obj->velocity_x;
		y += obj->velocity_y;
		z += obj->velocity_z;
		if (obj->arrival_axis != 0)
			switch (obj->arrival_axis)
			{
			case 16:
				if (x == obj->target_x || ((obj->x - obj->target_x) ^ (x - obj->target_x)) < 0)
					arrived = 1;
				break;
			case 17:
				if (y == obj->target_y || ((obj->y - obj->target_y) ^ (y - obj->target_y)) < 0)
					arrived = 1;
				break;
			case 18:
				if (z == obj->target_z || ((obj->z - obj->target_z) ^ (z - obj->target_z)) < 0)
					arrived = 1;
				break;
			}
		if (arrived)
		{
			if (obj->snap_to_target)
			{
				obj->velocity_x = 0;
				obj->velocity_z = 0;
				x = obj->target_x;
				z = obj->target_z;
				if (obj->motion_flags == 0)
				{
					y = obj->target_y;
					obj->velocity_y = 0;
				}
			}
			obj->target_x = TARGET_UNSET;
			obj->target_y = TARGET_UNSET;
			obj->target_z = TARGET_UNSET;
			obj->arrival_axis = 0;
		}
		obj->x = x;
		obj->y = y;
		obj->z = z;
		if (obj->steering_flags & 1)
		{
			x = obj->velocity_x;
			z = obj->velocity_z;
			if (x != 0 || z != 0)
			{
				turn = (s16)(ArcTan2(z, x) - obj->facing);
				if (turn > 0x1000)
					turn = 0x1000;
				if (turn < -0x1000)
					turn = -0x1000;
				obj->facing += turn;
			}
		}
	}
}

s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter)
{
	s16 cursor = interpreter->cursor;

	interpreter->lookup_result = interpreter->script[cursor + 1] - 1;
	interpreter->cursor += 2;
	return 0;
}

s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
	if (interpreter->delay > 0x3B)
	{
		interpreter->delay = 0;
		interpreter->cursor++;
		return 1;
	}
	if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0)
	{
		interpreter->cursor++;
		return 1;
	}
	return 0;
}

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
	s16			  initial = interpreter->cursor;
	ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

	if (callback(interpreter) != 0)
		return 0;
	if (interpreter->cursor == initial)
		interpreter->cursor = interpreter->cursor + 2;
	return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
	u32 *entries;
	s32	 index;
	s16 *field = &interpreter->lookup_result;
	s32	 zero = 0;

	*field = zero;
	if (key == 0)
	{
		return 0;
	}

	key &= 0xBFFFFFFF;
	entries = (u32 *)interpreter->script;
	index = 0;
	do
	{
		if (*entries++ == key)
		{
			return index + 1;
		}
		index++;
	} while (index <= 0x3FF);
	return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
	const s32 *arguments;
	s32		   repeat_limit;
	s32		   jump_key;
	u8		  *repeat_count;
	s32		   next_count;

	arguments = &interpreter->script[interpreter->cursor + 1];
	repeat_limit = *arguments++;
	jump_key = *arguments;
	if (repeat_limit == 0xFFFF)
	{
		interpreter->cursor = Script_FindLabel(interpreter, jump_key);
	}
	else
	{
		repeat_count = &interpreter->repeat_count;
		next_count = *repeat_count + 1;
		*repeat_count = next_count;
		if ((u8)next_count < (s16)repeat_limit)
		{
			interpreter->cursor = Script_FindLabel(interpreter, jump_key);
		}
		else
		{
			*repeat_count = 0;
			interpreter->cursor += 3;
		}
	}
	return 1;
}

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
	interpreter->cursor =
		Script_FindLabel(interpreter, interpreter->script[interpreter->cursor + 1]);
	return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter)
{
	s32		   index = interpreter->cursor;
	const s32 *table = interpreter->script;
	u32		   value = table[index + 1];

	if (interpreter->condition_result != 0)
	{
		interpreter->cursor = Script_FindLabel(interpreter, value);
	}
	else
	{
		interpreter->cursor += 2;
	}
	return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
	s32		   index = interpreter->cursor;
	const s32 *table = interpreter->script;
	u32		   value = table[index + 1];

	if (interpreter->condition_result == 0)
	{
		interpreter->cursor = Script_FindLabel(interpreter, value);
	}
	else
	{
		interpreter->cursor += 2;
	}
	return 1;
}

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
	s32 result;

	interpreter->script = Script_MainScript;
	interpreter->cursor = (result = 0);
	return result;
}

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
	interpreter->condition_result = GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

// フラグの今の値を condition_result に入れてから、change でフラグを書き換える
#define DEFINE_FLAG_AND_TEST(name, change)                       \
	s32 name(struct ScriptInterpreter *interpreter)              \
	{                                                            \
		s32 value;                                               \
                                                                 \
		value = interpreter->script[interpreter->cursor + 1];    \
		interpreter->condition_result = GameFlag_TestFar(value); \
		change(value);                                           \
		interpreter->cursor += 2;                                \
		return 1;                                                \
	}

DEFINE_FLAG_AND_TEST(Script_SetFlagAndTest, GameFlag_SetBitFar)
DEFINE_FLAG_AND_TEST(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter)
{
	s32 value;
	s32 result;

	value = interpreter->script[interpreter->cursor + 1];
	result = GameFlag_TestFar(value);
	interpreter->condition_result = result;
	if ((result << 0x18) == 0x01000000)
	{
		GameFlag_ClearBitFar(value);
	}
	else
	{
		GameFlag_SetBitFar(value);
	}
	interpreter->cursor += 2;
	return 1;
}

s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter)
{
	ObjectDispatch_ApplyArgumentToChildren(
		interpreter, interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter)
{
	ObjectDispatch_Release((struct DispatchObject *)interpreter);
	return 0;
}

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
	interpreter->cursor += 2;
	return 1;
}

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
	AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
	interpreter->cursor += 2;
	return 1;
}

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
	s32					  index;
	u8					 *flags;
	struct ObjectRuntime *entry = gObjectSlots;

	flags = &entry->unknown_59;
	for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry))
	{
		if (entry->script != NULL && (1 & *flags) && entry != (struct ObjectRuntime *)object)
		{
			if (Runtime_CheckRadiusOverlap(&entry->x, ((struct FieldActor *)entry)->radius - 2,
					values, ((struct FieldActor *)object)->radius - 2)
				>= 0)
				return -1;
		}
	}
	return 0;
}

struct ScriptObjectEntry *ScriptObject_FindOverlappingEntry(
	struct ScriptObjectEntry *object, s32 *values)
{
	s32					  index;
	u8					 *flags;
	struct ObjectRuntime *entry = gObjectSlots;

	flags = &entry->unknown_59;
	for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry))
	{
		if (entry->script != NULL && (1 & *flags) && entry != (struct ObjectRuntime *)object)
		{
			if (Runtime_CheckRadiusOverlap(&entry->x, ((struct FieldActor *)entry)->radius - 2,
					values, ((struct FieldActor *)object)->radius - 2)
				>= 0)
				return (struct ScriptObjectEntry *)entry;
		}
	}
	return NULL;
}

// 命令語の次の 3 語を座標として place にわたす
static __inline__ s32 ApplyPosition(struct ScriptInterpreter *interpreter,
	void (*place)(struct ObjectRuntime *object, s32 x, s32 y, s32 z))
{
	s32		   first;
	s32		   second;
	s32		   third;
	const s32 *argument;

	argument = interpreter->script + interpreter->cursor;
	argument++;
	first = *argument;
	argument++;
	second = *argument;
	argument++;
	third = *argument;
	place((struct ObjectRuntime *)interpreter, first, second, third);
	interpreter->cursor += 4;
	return 1;
}

s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter)
{
	return ApplyPosition(interpreter, Object_SetPositionAndResetMotion);
}

s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter)
{
	return ApplyPosition(interpreter, Object_SetMoveTarget);
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
	u8	*entry = (u8 *)(object->script + (s16)object->script_cursor);
	s32 *cursor = (s32 *)(entry + 4);
	s32	 first = *cursor++;
	s32	 second = *cursor++;
	s32	 third = *cursor;

	Object_SetMoveTarget(
		(struct ObjectRuntime *)object, object->x + first, object->y + second, object->z + third);
	object->script_cursor += 4;
	return 1;
}

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
	struct ScriptObjectRuntime *target;

	target = object->linked_object;
	object->script_value = ArcTan2((target->z - object->z), (target->x - object->x));
	object->script_cursor++;
	return 1;
}

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
	struct ScriptObjectRuntime *target;

	target = object->linked_object;
	Object_SetMoveTarget((struct ObjectRuntime *)object, target->x, target->y, target->z);
	object->script_cursor++;
	return 1;
}

s32 Script_ApplyLocalOffsetPosition(struct ScriptObjectRuntime *object)
{
	s32 offset[3];

	Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + offset[0],
		object->y + offset[1], object->z + offset[2]);
	object->script_cursor += 3;
	return 1;
}

struct MapScrollBounds
{
	u8	unknown_00[0xec];
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
};

s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj)
{
	struct ObjectRuntime   *leader;
	struct MapScrollBounds *bounds;
	s32						min_x;
	s32						min_z;
	s32						max_x;
	s32						max_z;
	s32						x;
	s32						y;
	s32						z;
	s32						dz;
	s32						dy;
	s32						dx;
	s32						dist;
	s32						step;

	bounds = gMapWork[0];
	leader = obj->linked_object;
	min_x = bounds->left + 0x780000;
	min_z = bounds->top + leader->y + 0x600000;
	max_x = bounds->right - 0x780000;
	max_z = bounds->bottom + leader->y - 0x400000;
	obj->flags = 0;
	if (leader != NULL && leader->script != NULL)
	{
		x = leader->x;
		y = leader->y;
		z = leader->z;
		obj->target_x = TARGET_UNSET;
		obj->target_y = TARGET_UNSET;
		obj->target_z = TARGET_UNSET;
		if (x < min_x)
			x = min_x;
		if (z < min_z)
			z = min_z;
		if (x > max_x)
			x = max_x;
		if (z > max_z)
			z = max_z;
		if (obj->action != 0)
		{
			obj->x = x;
			obj->y = y;
			obj->z = z;
		}
		else
		{
			// XZ 平面の距離: dy は Object_SetMoveTarget と同じ形で求めるが距離には使わない
			dx = (x - obj->x) / 0x10000;
			/* FAKEMATCH: dead code: this dy is overwritten below and the compiler drops it, but its references rank y above the bounds in global allocation, keeping y in r5 as native; without it 114 rows differ in the English build (y in r9). The form is the sibling Object_SetMoveTarget's three-axis delta. */
			dy = (y - obj->y) / 0x10000;
			dz = (z - obj->z) / 0x10000;
			dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
			dx = x - obj->x;
			dy = y - obj->y;
			dz = z - obj->z;
			if (dist < 0x400000)
				dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
			if ((step = dist / 8) > obj->speed_limit)
				step = obj->speed_limit;
			if (dist < 0x4000)
			{
				obj->x = x;
				obj->z = z;
			}
			else
			{
				if (dist > step)
				{
					dx = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dx), step);
					dz = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dz), step);
				}
				obj->x += dx;
				obj->z += dz;
			}
			if ((dy < 0 ? -dy : dy) > 0x8000)
				dy /= 4;
			obj->y += dy;
		}
	}
	obj->step++;
	return 1;
}

extern u8 IwramSqrt[];
s32		  _call_via_r3(s32, s32, s32, s32);

s32 Object_ApproachLinkedObject(struct ObjectRuntime *object)
{
	s32					  len;
	s32					  mz;
	s32					  dx;
	s32					  dz;
	s32					  dzh;
	s32					  dxh;
	s32					  n;
	s32					  mx;
	s32					  dx2;
	struct ObjectRuntime *link;

	link = object->linked_object;
	object->speed_limit = link->speed_limit;
	object->acceleration = link->acceleration;
	dx = link->x - object->x;
	dz = link->z - object->z;
	dxh = dx >> 0x10;
	dzh = dz >> 0x10;
	len = _call_via_r3((dxh * dxh) + (dzh * dzh), dx, dzh, (u32)IwramSqrt);
	if (len > 0x10)
	{
		dx2 = dx;
		n = len - 0x10;
		mx = dx2 * n / len;
		mz = dz * n / len;
		Object_SetMoveTarget(object, object->x + mx, object->y, object->z + mz);
		ObjectDispatch_ApplyArgumentToChildren(object, 2);
		object->step = (object->step + 1);
		return 1;
	}
	ObjectDispatch_ApplyArgumentToChildren(object, 1);
	return 0;
}

s32 Object_Wander(struct ScriptObjectRuntime *object)
{
	struct FieldPosition pos;
	struct FieldPosition probe;
	s32					 base;
	const s32			*args;
	s32					 range;
	s32					 limit;
	s32					 radius;
	s32					 heading;
	s32					 tries;
	s32					 dx;
	s32					 dz;

	args = &object->script[(s16)object->script_cursor + 1];
	base = *args++;
	range = *args++;
	limit = *args / 0x10000;
	tries = 0;
	limit = limit * limit;
retry:
	tries++;
	if (tries <= 7)
	{
		pos.x = object->x;
		pos.y = object->y;
		pos.z = object->z;
		radius = base + Iwram_MulQ16(Random16(), range);
		heading = object->script_value + (Random16() >> 2) - (Random16() >> 2);
		Vector_AddPolarOffset(radius, heading, &pos);
		if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0
			|| Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
			goto retry;
		radius += 0x80000;
		probe.x = object->x;
		probe.y = object->y;
		probe.z = object->z;
		Vector_AddPolarOffset(radius, heading, &probe);
		probe.x = object->x;
		probe.y = object->y;
		probe.z = object->z;
		Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
		if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
			goto retry;
		probe.x = object->x;
		probe.y = object->y;
		probe.z = object->z;
		Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
		if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
			goto retry;
		/* FAKEMATCH: one-pass block: it ends cse2's block, so the division works on the loaded pos.x and the copy outlives it into found. As plain statements cse2 swaps them (differ 16), also with dx/dz split or casts. The retry gotos stay: as for(;;) with continue, loop.c hoists &pos and &probe, which the native recomputes each try (differ 128). */
		do
		{
			dx = pos.x / 0x10000 - object->home_x;
			dz = pos.z / 0x10000 - object->home_z;
		} while (0);
		if (dx * dx + dz * dz > limit)
			goto retry;
		goto found;
	}
	object->script_value += 0x8000;
	object->turned_back = 1;
	return 0;
found:
	Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
	object->script_cursor += 4;
	return 1;
}

s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object)
{
	struct FieldPosition pos;
	struct FieldPosition probe;
	s32					 base;
	const s32			*args;
	s32					 range;
	s32					 limit;
	s32					 radius;
	u16					 heading;
	u16					 angle;
	s32					 tries;
	s32					 dx;
	s32					 dz;

	args = &object->script[(s16)object->script_cursor + 1];
	base = *args++;
	range = *args++;
	limit = *args / 0x10000;
	angle = object->script_value;
	limit *= limit;
	tries = 0;
	dx = object->x / 0x10000 - object->home_x;
	dz = object->z / 0x10000 - object->home_z;
	if (dx * dx + dz * dz <= limit)
	{
	roam:
		tries++;
		if (tries <= 7)
		{
			radius = base + Iwram_MulQ16(Random16(), range);
			heading = angle + (Random16() >> 2) - (Random16() >> 2);
			pos.x = object->x;
			pos.y = object->y;
			pos.z = object->z;
			Vector_AddPolarOffset(0x80000, heading, &pos);
			if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
				goto roam;
			pos.x = object->x;
			pos.y = object->y;
			pos.z = object->z;
			Vector_AddPolarOffset(radius, heading, &pos);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
				goto roam;
			probe.x = object->x;
			probe.y = object->y;
			radius += 0x80000;
			probe.z = object->z;
			Vector_AddPolarOffset(radius, heading, &probe);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
				goto roam;
			probe.x = object->x;
			probe.y = object->y;
			probe.z = object->z;
			Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
				goto roam;
			probe.x = object->x;
			probe.y = object->y;
			probe.z = object->z;
			Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
				goto roam;
			probe.x = object->x;
			probe.y = object->y;
			probe.z = object->z;
			Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
				goto roam;
			probe.x = object->x;
			probe.y = object->y;
			probe.z = object->z;
			Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
			if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
				goto roam;
			dx = pos.x / 0x10000 - object->home_x;
			dz = pos.z / 0x10000 - object->home_z;
			if (dx * dx + dz * dz > limit)
				goto roam;
			object->flags_59 |= 2;
			Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
			goto done;
		}
	}
	tries = 0;
	angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
	tries++;
	if (tries <= 7)
	{
		radius = base + Iwram_MulQ16(Random16(), range);
		heading = angle + (Random16() >> 2) - (Random16() >> 2);
		pos.x = object->x;
		pos.y = object->y;
		pos.z = object->z;
		Vector_AddPolarOffset(0x80000, heading, &pos);
		if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
			goto back;
		pos.x = object->x;
		pos.y = object->y;
		pos.z = object->z;
		Vector_AddPolarOffset(radius, heading, &pos);
		if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
			goto back;
		object->flags_59 &= ~2;
		Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
	}
done:
	object->script_cursor += 4;
	return 1;
}

enum ScriptOperandOperation
{
	SCRIPT_OPERAND_SET,
	SCRIPT_OPERAND_ADD,
	SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);

extern OperandFunc Script_OperandHandlerTable[];

// 作業領域の 1 項目を命令の値で書き換える: 代入、加算、または比較 (結果は comparison_result)
// 32 ビットの項目
#define DEFINE_OPERAND_WORD(name, field)                              \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value;                                    \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// アドレスの項目: 加算は value の 4 倍 (4 バイト単位)
#define DEFINE_OPERAND_ADDRESS(name, field)                           \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value * 4;                                \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// 8 ビット、16 ビットの項目: 比較は項目の型に切り詰めた値と行う
#define DEFINE_OPERAND_NARROW(name, field, type)                      \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
		}                                                             \
		else if (operation == SCRIPT_OPERAND_ADD)                     \
		{                                                             \
			state->field += value;                                    \
		}                                                             \
		else                                                          \
		{                                                             \
			state->comparison_result = state->field == (type)value;   \
		}                                                             \
	}

DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, script_address)

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->cursor = value;
	}
	else if (operation == SCRIPT_OPERAND_ADD)
	{
		state->cursor = ((s16)state->cursor + value);
	}
	else
	{
		state->comparison_result = (s16)state->cursor == (s16)value;
	}
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword, unsigned_halfword, u16)
DEFINE_OPERAND_WORD(Script_SetOrCompareX, word_08)
DEFINE_OPERAND_WORD(Script_SetOrCompareY, word_0c)
DEFINE_OPERAND_WORD(Script_SetOrCompareZ, word_10)

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->halfword_20 = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		state->halfword_20 = (state->halfword_20 + value);
		return;
	}
	state->comparison_result = state->halfword_20 == (s16)value;
}

DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, word_18)
DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, word_1c)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, word_24)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, word_28)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, word_2c)
DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, word_30)
DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, word_34)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, word_38)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, word_3c)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, word_40)
DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, word_44)
DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, word_48)
DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, word_14)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, word_4c)
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, address_50)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, byte_54, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, byte_55, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, byte_56, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult, comparison_result, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, byte_58, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, byte_59, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, byte_5a, u8)
DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, byte_5b, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, byte_5d, u8)

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		work->halfword_5e = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		work->halfword_5e += value;
		return;
	}
	work->comparison_result = (s16)work->halfword_5e == (s16)value;
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64, signed_halfword_64, s16)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66, signed_halfword_66, s16)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, word_68)
DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, word_6c)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, byte_62, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, byte_63, u8)

// 命令語の次の 2 語 (項目の番号と値) を読み、項目の関数に operation をわたす
static __inline__ s32 ApplyOperand(struct ScriptOperands *work, s32 operation)
{
	s16			index = work->cursor;
	const s32  *command = (const s32 *)work->script_address + index + 1;
	OperandFunc handler = Script_OperandHandlerTable[command[0]];

	if (handler != 0)
		handler(work, operation, command[1]);
	work->cursor += 3;
	return 1;
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_SET);
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_ADD);
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_COMPARE);
}

static __inline__ void StoreHalfword(s16 *address, s32 value)
{
	*address = value;
}

void ObjectDispatch_SetUpdateValue(void *raw, s32 value)
{
	struct ObjectRuntime *object = raw;

	if (object != NULL)
		*(s32 *)object->unknown_6c = value;
}

#if EDITION_INTERNATIONAL

#define ASSIGNED_KEYS gKeyState
#else

#define ASSIGNED_KEYS gKeysHeld
#endif

u32 Field_StoreAssignedKeyValue(u32 value)
{
	u32					  selector = value >> 14;
	u32					  request = 0x3FFF & value;
	struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

	if (GameFlag_TestFar(0x107) != 0)
	{
		StoreHalfword((s16 *)&work->raised_trigger, 0xFA);
	}
	else if (work->mode == 3)
	{
		if (ASSIGNED_KEYS & 0x100)
		{
			StoreHalfword((s16 *)&work->raised_trigger, 0xFC88);
		}
		else if (ASSIGNED_KEYS & 0x200)
		{
			StoreHalfword((s16 *)&work->raised_trigger, 0xFC87);
		}
	}
	else
	{
		switch (selector)
		{
		case 0:
			StoreHalfword(&work->psynergy_request, request);
			break;
		case 1:
			StoreHalfword((s16 *)work->unknown_180, request);
			break;
		}
	}

	return request;
}

s32 Field_CheckConfiguredKeys(void)
{
	struct EventRuntime *work = gEventWork;
	s32					 result = 0;

	if (work == NULL)
		return 0;
	if (gKeyState & gGameState.select_keys)
	{
		work->unknown_172[0] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.a_keys)
	{
		work->unknown_172[1] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.start_keys)
	{
		work->unknown_172[2] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.first_shortcut_keys)
	{
		result = Field_StoreAssignedKeyValue(gGameState.first_shortcut);
	}
	else if (gKeyState & gGameState.second_shortcut_keys)
	{
		result = Field_StoreAssignedKeyValue(gGameState.second_shortcut);
	}
	return result;
}

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
	const struct FieldPosition *first = (const struct FieldPosition *)a;
	const struct FieldPosition *second = (const struct FieldPosition *)b;
	const s32				   *first_word = &first->x;
	const s32				   *second_word = &second->x;
	s32							dx = (*first_word++ - *second_word++) >> 16;
	s32							dy = (*first_word++ - *second_word++) >> 16;
	s32							dz = (*first_word - *second_word) >> 16;
	s32							radius = radius_a + radius_b;

	if (!(dx > 0x400000) && !(dz > 0x400000) && dx * dx + dy * dy + dz * dz < radius * radius)
		return 0;
	return -1;
}

struct WorldMapSprite
{
	struct FieldSprite sprite;
	u8				   unknown_28[4];
	u8				  *shadow;
};

s32 FieldObject_UpdatePlayerControl(struct ObjectRuntime *object)
{
	struct FieldPosition  position;
	struct FieldPosition  test_position;
	struct FieldPosition  contact;
	s16					  angle_offsets[22];
	s32					  collision;
	s32					  pushed;
	s16					  heading;
	s32					  angle;
	s32					  mode;
	u16					  direction;
	s32					  rotation;
	s32					  angle_index;
	s32					  count;
	struct ObjectRuntime *other;

	collision = 0;
	pushed = 0;
	if (gDebugMode != 0 && GameFlag_TestFar(350) != 0)
	{
		s32 free;
		s32 block;

		free = 0;
		for (block = 0; block < 512; block++)
		{
			if (ResourceBlockOwners[block] == 0xff)
				free++;
		}
		if (free - 136 < 0)
			AudioCommand_PlayFar(135);
	}
	/* FAKEMATCH: the volatile read puts gDebugMode's address in r5 here and at the test before it, as the bytes do; read plainly both take r3 (differ 12, 808 instructions either way). */
	if (*(volatile u8 *)&gDebugMode != 0)
	{
		s32 mask;

		mask = KEY_L;
		if (gKeysHeld & mask)
		{
			s32 wait;

			wait = mask;
		wait_a:
			--wait;
			if (wait != 0)
				goto wait_a;
			wait = 95;
		wait_b:
			wait--;
			if (wait >= 0)
				goto wait_b;
			wait = 63;
		wait_c:
			wait--;
			if (wait >= 0)
				goto wait_c;
			wait = 63;
		wait_d:
			wait--;
			if (wait >= 0)
				goto wait_d;
		}
	}
	if (gKeysHeld & gGameState.dash_keys)
	{
		object->speed_limit = 0x18000;
		object->acceleration = 0x4000;
		mode = 5;
	}
	else
	{
		object->speed_limit = 0x10000;
		object->acceleration = 0x4000;
		mode = 2;
	}
	if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & KEY_B))
	{
		object->speed_limit = 0x40000;
		object->acceleration = 0x10000;
		mode = 5;
	}

	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	if ((u16)angle == 0xffff)
	{
		collision |= 4;
	}
	else
	{
		collision = 0;
		position.x = object->x;
		position.y = object->y;
		position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)angle, &position);
		if (gDebugMode != 0 && (gKeysHeld & KEY_L))
		{
			heading = angle;
		}
		else
		{
			if (Map_CheckObjectStep(object, &position) == 0)
			{
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &test_position);
				if (Map_CheckObjectStep(object, &test_position) == 0)
				{
					test_position.x = object->x;
					test_position.y = object->y;
					test_position.z = object->z;
					Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &test_position);
					if (Map_CheckObjectStep(object, &test_position) == 0)
					{
						test_position.x = object->x;
						test_position.y = object->y;
						test_position.z = object->z;
						Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &test_position);
						if (Map_CheckObjectStep(object, &test_position) == 0)
						{
							test_position.x = object->x;
							test_position.y = object->y;
							test_position.z = object->z;
							Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &test_position);
							if (Map_CheckObjectStep(object, &test_position) == 0)
							{
								heading = angle;
								goto push_objects;
							}
						}
					}
				}
			}
			direction = angle;
			count = 0;
			angle_offsets[count++] = direction + 0x1000;
			angle_offsets[count++] = direction - 0x1000;
			angle_offsets[count++] = direction + 0x2000;
			angle_offsets[count++] = direction - 0x2000;
			angle_offsets[count++] = direction + 0x3000;
			angle_offsets[count++] = direction - 0x3000;
			for (angle_index = 0; angle_index < count; angle_index++)
			{
				heading = angle_offsets[angle_index];
				position.x = object->x;
				position.y = object->y;
				position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)heading, &position);
				if (Map_CheckObjectStep(object, &position) != 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &test_position);
				if (Map_CheckObjectStep(object, &test_position) != 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &test_position);
				if (Map_CheckObjectStep(object, &test_position) != 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &test_position);
				if (Map_CheckObjectStep(object, &test_position) != 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &test_position);
				if (Map_CheckObjectStep(object, &test_position) == 0)
					goto push_objects;
			}
			position.x = object->x;
			position.y = object->y;
			position.z = object->z;
			collision |= 1;

		push_objects:
			contact.x = object->x;
			contact.y = object->y;
			contact.z = object->z;
			Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
			other = gObjectSlots;
			for (angle_index = 63; angle_index >= 0; angle_index -= 1, other++)
			{
				s32 radius;

				radius = object->radius - 2;
				if (other->script == 0)
					continue;
				if ((other->unknown_59 & 1) == 0)
					continue;
				if (other == object)
					continue;
				if (Runtime_CheckRadiusOverlap(&other->x, radius, &contact.x, other->radius - 2)
					< 0)
					continue;
				if ((other->unknown_59 & 2) != 0 && other->movement_state == 0)
				{
					direction = ArcTan2(other->z - object->z, other->x - object->x);
					test_position.x = other->x;
					test_position.y = other->y;
					test_position.z = other->z;
					heading = direction;
					Vector_AddPolarOffset(0x4000, direction, &test_position);
					if (!ScriptObject_CheckOverlap(
							(struct ScriptObjectEntry *)other, (s32 *)&test_position))
					{
						test_position.x = other->x;
						test_position.y = other->y;
						test_position.z = other->z;
						Vector_AddPolarOffset(0xa0000, direction, &test_position);
						if (Map_CheckObjectStep(other, &test_position) == 0)
						{
							test_position.x = other->x;
							test_position.y = other->y;
							test_position.z = other->z;
							Vector_AddPolarOffset(0xa0000, direction + 0x1000, &test_position);
							if (Map_CheckObjectStep(other, &test_position) == 0
								&& Map_CheckObjectStep(other, &test_position) == 0)
							{
								test_position.x = other->x;
								test_position.y = other->y;
								test_position.z = other->z;
								Vector_AddPolarOffset(0xa0000, direction - 0x1000, &test_position);
								if (Map_CheckObjectStep(other, &test_position) == 0)
								{
									Vector_AddPolarOffset(
										0x4000, direction, (struct FieldPosition *)&other->x);
									other->target_x = -0x80000000;
									other->target_y = -0x80000000;
									other->target_z = -0x80000000;
									pushed |= 1;
									continue;
								}
							}
						}
					}
				}
				collision |= 2;
			}
			if (collision == 0 && pushed != 0)
			{
				object->speed_limit = 0x4000;
				object->acceleration = 0x2000;
			}
		}
	}
	if (gEventWork != NULL)
	{
		if (collision & 3)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	if (pushed != 0)
	{
		ObjectDispatch_ApplyArgumentToChildren(object, 8);
	}
	else if (collision != 0)
	{
		s32 action;

		action = 9;
		if (UnitGetFar(gGameState.selected_actor)->hp == 0)
			action = 22;
		ObjectDispatch_ApplyArgumentToChildren(object, action);
	}
	else
	{
		ObjectDispatch_ApplyArgumentToChildren(object, mode);
	}

	if (collision != 0)
	{
		object->target_x = -0x80000000;
		object->target_y = -0x80000000;
		object->target_z = -0x80000000;
		object->velocity_x = 0;
		object->velocity_z = 0;
		if (collision & 3)
		{
			s32 difference;

			difference = (u16)angle - object->angle;
			difference = (s16)difference;
			if (difference > 0x1000)
				difference = 0x1000;
			if (difference < -0x1000)
				difference = -0x1000;
			object->angle += difference;
		}
		object->action = 0;
		object->unknown_66 = 2;
	}
	else
	{
		s32 square;

		Object_SetMoveTarget(object, position.x, position.y, position.z);
		square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
						   + Iwram_MulQ16(object->velocity_z, object->velocity_z));
		object->velocity_x = 0;
		object->velocity_z = 0;
		Vector_AddPolarOffset(square, (u16)heading, (struct FieldPosition *)&object->velocity_x);
		if (object->action != 0)
			object->action--;
	}

	rotation = (u16)angle;
	if (((struct MapScrollWork *)gMapWork[0])->footprints != 0 && object->action == 0
		&& collision == 0)
	{
		struct ObjectRuntime *effect;

		effect = FieldObject_Create(25, object->x, object->y, object->z);
		if (effect != 0)
		{
			struct FieldSprite *animation;

			effect->terrain_height = object->terrain_height;
			animation = effect->animation;
			ObjectDispatch_Initialize(
				(struct DispatchObject *)effect, (u32)WorldMap_FootprintActions);
			effect->unknown_23 = 2;
			effect->flags = collision;
			if (animation != 0)
			{
				AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
				animation->flags = collision;
				animation->rotation = rotation + 0x4000;
				animation->priority = 3;
			}
			if (object->unknown_66 == 2)
			{
				AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 2);
				object->unknown_66 = collision;
			}
			if (object->unknown_66 != 0)
				effect->angle = 0x8000;
			if (mode == 5)
				object->action = 12;
			else
				object->action = 18;
			object->unknown_66 ^= 1;
		}
	}
	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

#if EDITION_INTERNATIONAL
#define WORLD_PROBE_RADIUS 0x70000
#else
#define WORLD_PROBE_RADIUS 0x80000
#endif

s32 Object_MoveOnWorldMap(struct ObjectRuntime *object)
{
	struct FieldPosition position;
#if EDITION_INTERNATIONAL
	struct FieldPosition test_position;
	s16					 angle_offsets[28];
#else
	struct FieldPosition left_position;
	struct FieldPosition right_position;
	s16					 angle_offsets[22];
#endif
	s32						collision;
	s32						angle;
	u16						direction;
	s32						angle_index;
	s32						mode;
	s32						count;
	s32						difference;
	s32						collision_kind;
	s32						square;
	s32						rate;
	struct PerspectiveWork *work;
	s32						step;
	struct FieldSprite	   *sprite;
	struct ObjectRuntime   *effect;
	struct FieldSprite	   *animation;

	mode = 2;
	collision = 0;
#if !EDITION_INTERNATIONAL
	count = 0;
#endif
	if (gKeysHeld & gGameState.dash_keys)
	{
		object->speed_limit = 0x10000;
		object->acceleration = 0x14000;
		mode = 5;
	}
	else
	{
		object->speed_limit = 0x8000;
		object->acceleration = 0x4000;
	}
	if ((gKeysRepeat & KEY_L) != 0)
		object->speed_limit = 0x40000;

	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	direction = angle;
	if (direction == 0xffff)
	{
		collision |= 4;
	}
	else
	{
		collision = 0;
		position.x = object->x;
		position.y = object->y;
		position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
		if (gDebugMode == 0 || (gKeysHeld & KEY_L) == 0)
		{
#if EDITION_INTERNATIONAL
			if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) == 0)
			{
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position)
					== 0)
				{
					test_position.x = object->x;
					test_position.y = object->y;
					test_position.z = object->z;
					Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
					if (CheckWorldMapCollisionRange(
							(s32)object, (struct WorldPosition *)&test_position)
						== 0)
					{
						test_position.x = object->x;
						test_position.y = object->y;
						test_position.z = object->z;
						Vector_AddPolarOffset(
							WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
						if (CheckWorldMapCollisionRange(
								(s32)object, (struct WorldPosition *)&test_position)
							== 0)
						{
							test_position.x = object->x;
							test_position.y = object->y;
							test_position.z = object->z;
							Vector_AddPolarOffset(
								WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
							if (CheckWorldMapCollisionRange(
									(s32)object, (struct WorldPosition *)&test_position)
								== 0)
								goto update_object;
						}
					}
				}
			}
			count = 0;
			angle_offsets[count++] = direction + 0x1000;
			angle_offsets[count++] = direction - 0x1000;
			angle_offsets[count++] = direction + 0x2000;
			angle_offsets[count++] = direction - 0x2000;
			angle_offsets[count++] = direction + 0x3000;
			angle_offsets[count++] = direction - 0x3000;
			angle_offsets[count++] = direction + 0x4000;
			angle_offsets[count++] = direction - 0x4000;
			for (angle_index = 0; angle_index < count; angle_index++)
			{
				direction = angle_offsets[angle_index];
				position.x = object->x;
				position.y = object->y;
				position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
					!= 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position)
					!= 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position)
					!= 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position)
					!= 0)
					continue;
				test_position.x = object->x;
				test_position.y = object->y;
				test_position.z = object->z;
				Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
				if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position)
					!= 0)
					continue;
				angle = (s16)direction;
				goto update_object;
			}
			collision |= 1;

#else
			left_position.x = object->x;
			left_position.y = object->y;
			left_position.z = object->z;
			Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
			right_position.x = object->x;
			right_position.y = object->y;
			right_position.z = object->z;
			Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
			if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
					| CheckWorldMapCollisionRange(
						(s32)object, (struct WorldPosition *)&left_position)
					| CheckWorldMapCollisionRange(
						(s32)object, (struct WorldPosition *)&right_position))
				!= 0)
			{
				angle_offsets[count++] = direction + 0x1000;
				angle_offsets[count++] = direction - 0x1000;
				angle_offsets[count++] = direction + 0x2000;
				angle_offsets[count++] = direction - 0x2000;
				angle_offsets[count++] = direction + 0x3000;
				angle_offsets[count++] = direction - 0x3000;
				for (angle_index = 0; angle_index < count; angle_index++)
				{
					direction = angle_offsets[angle_index];
					position.x = object->x;
					position.y = object->y;
					position.z = object->z;
					Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
					left_position.x = object->x;
					left_position.y = object->y;
					left_position.z = object->z;
					Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
					right_position.x = object->x;
					right_position.y = object->y;
					right_position.z = object->z;
					Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
					if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
							| CheckWorldMapCollisionRange(
								(s32)object, (struct WorldPosition *)&left_position)
							| CheckWorldMapCollisionRange(
								(s32)object, (struct WorldPosition *)&right_position))
						!= 0)
						continue;
					angle = (s16)direction;
					goto update_object;
				}
				collision |= 1;
			}
#endif
		}
	}

update_object:
	if (gEventWork != NULL)
	{
		if ((collision & 3) != 0)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	if (collision != 0)
		ObjectDispatch_ApplyArgumentToChildren(object, 9);
	else
		ObjectDispatch_ApplyArgumentToChildren(object, mode);

	if (collision != 0)
	{
		object->target_x = -0x80000000;
		object->target_y = -0x80000000;
		object->target_z = -0x80000000;
		object->velocity_x = 0;
		object->velocity_z = 0;
		if ((collision & 3) != 0)
		{
			direction = angle;
			difference = (s16)(direction - object->angle);
			if (difference > 0x1000)
				difference = 0x1000;
			if (difference < -0x1000)
				difference = -0x1000;
			object->angle += difference;
		}
		object->action = 0;
		object->unknown_66 = 2;
	}
	else
	{
		Object_SetMoveTarget(object, position.x, position.y, position.z);
		square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
						   + Iwram_MulQ16(object->velocity_z, object->velocity_z));
		object->velocity_x = collision;
		object->velocity_z = collision;
		Vector_AddPolarOffset(square, (u16)angle, (struct FieldPosition *)&object->velocity_x);
		if (object->action != 0)
			object->action--;
	}

	work = gMapWork[0];
	rate = WorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
	step = (s16)(rate - work->yaw) / 8;
	if (step > 0x200)
		step = 0x200;
	if (step < -0x200)
		step = -0x200;
	if (step > -16 && step < 16)
		step = rate - work->yaw;
	work->yaw += step;

	if (object->animation_kind == 1)
	{
		sprite = object->animation;
		collision_kind = GetWorldMapCollision((struct WorldPosition *)&object->x);
		if (collision_kind == 9)
		{
			((struct WorldMapSprite *)sprite)->shadow[6] = 1;
			sprite->flags = 0;
		}
		else
		{
			((struct WorldMapSprite *)sprite)->shadow[6] = 9;
			sprite->flags = 1;
		}
		if (collision_kind == 6 && object->action == 0 && collision == 0)
		{
			effect = FieldObject_Create(24, object->x, object->y, object->z);
			if (effect != 0)
			{
				animation = effect->animation;
				ObjectDispatch_Initialize(
					(struct DispatchObject *)effect, (u32)(WorldMap_FootprintActions + 12));
				effect->flags = collision;
				effect->terrain_id = 1;
				if (animation != 0)
				{
					AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
					animation->flags = collision;
					animation->blend_mode = 1;
					animation->priority = 2;
				}
				object->action = 10;
			}
		}
	}
	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

s32 Field_CheckConfiguredKeysAndCount(void *work)
{
	struct ObjectRuntime *object = work;

	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
	struct FieldPosition pos;
	/* FAKEMATCH: never used; its 68 bytes are the native frame space no instruction touches. Without it the frame is 12 bytes, not 80 (differ 8: the sp adjustments and pos's address). 68 bytes is exactly the probe locals of the sibling Object_MoveOnWorldMap (test_position and angle_offsets[28] internationally, left_position, right_position and angle_offsets[22] in Japan); declared here instead, they also match tbs-en and tbs-ja (2026-10-13). */
	u8					 unused[68];
	struct MapCell		*from;
	struct MapCell		*to;
	u16					 angle;
	s32					 motion;
	s32					 blocked;
	s32					 delta_z;
	s32					 height;

	object->speed_limit = 0x8000;
	object->acceleration = 0x4000;
	motion = 12;
	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	if (angle == 0xffff)
	{
		blocked = 4;
	}
	else
	{
		motion = 14;
		if ((angle & 0xf000) != 0)
		{
			motion = 15;
			if ((angle & 0xf000) != 0x8000)
				motion = 10;
		}
		blocked = 0;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		Vector_AddPolarOffset(0x80000, angle, &pos);
		pos.x += object->x;
		delta_z = pos.z;
		if (delta_z < 0)
			object->angle = 0xc000;
		if (delta_z > 0)
			object->angle = 0x4000;
		pos.y = object->y - pos.z;
		pos.z = object->z;
		from =
			&((struct MapCell *)Ram_MapCellBuffer)[object->x / 0x100000 + (pos.z / 0x100000) * 128];
		to = &((struct MapCell *)Ram_MapCellBuffer)[pos.x / 0x100000 + (pos.z / 0x100000) * 128];
		if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0
			|| from->collision_code != to->collision_code)
		{
			blocked = 4;
			motion = 12;
		}
		else if (gKeysHeld & KEY_UP)
		{
			height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z - 0x100000);
			if (height - object->y < 0x100000)
				goto refused;
		}
		else if (gKeysHeld & KEY_DOWN)
		{
			height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z);
			if (height - object->y > -0x80000)
			{
			refused:
				blocked = 1;
				motion = 12;
			}
		}
	}
	if (gEventWork != NULL)
	{
		if (blocked & 3)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	ObjectDispatch_ApplyArgumentToChildren(object, motion);
	if (blocked != 0)
	{
		object->target_x = 0x80000000;
		object->target_y = 0x80000000;
		object->target_z = 0x80000000;
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
	}
	else
	{
		Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
	}
	object->step++;
	return 1;
}

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
	s32 n;
	u8 *p;
	u8 *tbl;

	p = buf;
	tbl = Runtime_ByteRemapTable;
	n = cnt - 1;
	if (n != -1)
	{
		do
		{
			n -= 1;
			*p = tbl[*p];
			p += 1;
		} while (n != -1);
	}
}

//--------------------------------------------------------------------------
// decode_staged_buffer.c
// マップ：展開したブロックのデータを gMapBlocks にほどく
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 展開バッファの [1] が方式、[2] からが 16 ビットのデータ
// size : 方式のバイトを含む長さ（偶数ならデータなし）
//   方式 0 : そのまま写す
//   方式 1 : 上位バイトの列のあとに下位バイトの列。前の値と XOR して戻す
//   方式 2 : 前の値と XOR して戻す
//--------------------------------------------------------------------------
void Tilemap_DecodeStagedBuffer(s32 size)
{
	u16 *dst;
	u16 *src;
	s32	 n;
	s32	 i;

	src = (u16 *)(Ram_MapCellBuffer + 2);
	dst = (u16 *)gMapBlocks;
	n = (size - 1) / 2;
	if (size & 1)
	{
		switch (((u8 *)gMapCellBuffer)[1])
		{
		case 0:
			for (i = 0; i < n; i++)
				*dst++ = *src++;
			break;
		case 1:
		{
			u32 prev = 0;
			u8 *hi = Ram_MapCellBuffer + 2;
			u8 *lo = hi + n;

			for (i = 0; i < n; i++)
			{
				u32 v = ((*hi++ << 8) | *lo++) ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		case 2:
		{
			s32 prev = 0;

			for (i = 0; i < n; i++)
			{
				s32 v = *src++ ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		}
	}
}

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

void Graphics_RenumberFillerEntries(void)
{
	u32 *p = (u32 *)gMapCellBuffer;
	u32	 cnt = 128 << 7;
	u32	 mask = 0xfff;
	s32	 no = -1;

	do
	{
		u32 value = *p++;
		u32 idx = value & mask;

		if (idx == mask)
		{
			if (no != idx)
			{
				no++;
			}
			value = value + no - idx;
			p[-1] = value;
		}
		cnt--;
	} while (cnt != 0);
}

void Tilemap_ConvertBuffer(void)
{
	u8		 *saved;
	ConvertFn routine;
	u32		  size;

	size = 0x8000;
	saved = Runtime_BumpAllocateAlternatePool(size);
	Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
	RamCode_Load(routine, Tile_ConvertMap, Tile_ConvertMapCodeSize);
	routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
	MemFree(routine);
	MemFree(saved);
}

static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
	u16 *colors;
	u8	*destination;
	u32	 index = ((*map << 20) >> 18) + parity;

	colors = (u16 *)Ram_MapBlocks;
	colors += index;
	destination = base + (rowmod + colmod + parity) * 2;
	*(u16 *)destination = *colors;
	colors = (u16 *)(Ram_MapBlocks + 4);
	colors += index;
	*(u16 *)(destination + 64) = *colors;
}

static __inline__ void CopyCameraCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
	u32 *tiles;
	u8	*dest;
	u32	 index = (*map << 20) >> 19;

	tiles = (u32 *)Ram_MapBlocks;
	tiles += index;
	dest = base + rowmod * 2 + colmod * 2;
	*(u32 *)dest = *tiles;
	tiles = (u32 *)(Ram_MapBlocks + 4);
	tiles += index;
	*(u32 *)(dest + 64) = *tiles;
}

struct SceneEntry
{
	u16 resources[6];
};

extern struct SceneEntry Map_LayeredScenes[];

struct SceneLayerSource
{
	u8 x;
	u8 y;
	s8 scroll_x;
	s8 scroll_y;
	s8 speed_x;
	s8 speed_y;
	u8 period_x;
	u8 period_y;
};

struct SceneHeader
{
	u8						origin[4];
	u8						priority[3];
	u8						char_base[3];
	u16						unknown_0a;
	struct SceneLayerSource layers[3];
	s32						tiles;
	s32						collision;
	s32						tilemap;
	s32						animation;
	s32						blend;
	s32						script;
};

s32 Map_LoadLayeredScene(s32 index)
{
	struct SceneEntry		*entry;
	struct MapScrollWork	*work;
	struct SceneHeader		*header;
	struct MapLayerScroll	*layer;
	struct SceneLayerSource *source;
	s32						*scale_x;
	s32						*scale_y;
	s32						 i;
	u8						*buf;
	s32						 backdrop;
	u8						*src;
	s32						 cnt;

	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	Blend_SetDarkenTarget0(0);
	entry = &Map_LayeredScenes[index];
	work = Runtime_AllocateBlock(8, sizeof(struct MapScrollWork));
	Iwram_ClearWords(work, sizeof(struct MapScrollWork));
	header =
		(struct SceneHeader *)Resource_GetTableEntry(entry->resources[0] + (u32)&ResourceId_Map001);
	src = (u8 *)header + header->tiles;
	Tilemap_DecodeStagedBuffer(Resource_DecodeType01(src, Ram_MapCellBuffer + 1));
	src = (u8 *)header + header->collision;
	Resource_DecodeType01(src, Ram_MapCollision);
	src = (u8 *)header + header->tilemap;
	Resource_DecodeType01(src, Ram_MapCellBuffer);
	Tilemap_ConvertBuffer();
	if (header->animation != 0)
	{
		src = (u8 *)header + header->animation;
		Resource_DecodeType01(src, Ram_MapCollision + 0x1000);
		MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
	}
	if (header->blend != 0)
	{
		src = (u8 *)header + header->blend;
		Resource_DecodeType01(src, Ram_MapCollision + 0x1e00);
		DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
	}
	work->script = (u8 *)header + header->script;
	work->min_x = header->origin[0] << 19;
	work->min_y = header->origin[1] << 19;
	work->max_x = header->origin[2] << 19;
	work->max_y = header->origin[3] << 19;
	scale_x = &work->view_x;
	*scale_x = 0;
	scale_y = &work->view_y;
	*scale_y = 0;
	work->enabled[0] = header->priority[0];
	work->enabled[1] = header->priority[1];
	work->enabled[2] = header->priority[2];
	layer = work->layers;
	source = header->layers;
	for (i = 0; i < 3; i++)
	{
		u32 x = source->x;
		u32 y = source->y;
		s32 scroll_x;
		s32 scroll_y;
		s32 base_x;
		s32 base_y;

		layer->offset_x = base_x = x << 19;
		layer->offset_y = base_y = y << 19;
		scroll_x = source->scroll_x << 12;
		scroll_y = source->scroll_y << 12;
		layer->scale_x = scroll_x;
		layer->scale_y = scroll_y;
		layer->speed_x = source->speed_x << 12;
		layer->speed_y = source->speed_y << 12;
		layer->mask_x = source->period_x;
		layer->mask_y = source->period_y;
		layer->phase_x = 0;
		layer->phase_y = 0;
		layer->cells = (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
		layer->x = Iwram_MulQ16(*scale_x, scroll_x) + base_x;
		layer->y = Iwram_MulQ16(*scale_y, scroll_y) + base_y;
		source++;
		layer++;
	}
	work->flags = DISPCNT_OBJ_ON;
	if (work->enabled[0] != 0)
		work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
	if (work->enabled[1] != 0)
		work->flags |= DISPCNT_BG2_ON;
	if (work->enabled[2] != 0)
		work->flags |= DISPCNT_BG1_ON;
	cnt = work->enabled[0] | BGCNT_CHAR_BASE(header->char_base[0]) | BGCNT_SCREEN_BASE(5);
	REG_BG3CNT = cnt;
	Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(header->char_base[1]) | BGCNT_SCREEN_BASE(6),
		&REG_BG2CNT);
	Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(header->char_base[2]) | BGCNT_SCREEN_BASE(7),
		&REG_BG1CNT);
	if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0)
	{
		GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
	}
	else
	{
		buf = MemAlloc(0x4000);
		if (buf != NULL)
		{
			backdrop = *(s16 *)BG_PLTT;
			Resource_DecodeType01(
				Resource_GetTableEntry(entry->resources[1] + (u32)&ResourceId_Map001), buf);
			*(s16 *)buf = backdrop;
			/* FAKEMATCH: needed to match: these four copies call the fixed entry itself; through Iwram_CopyWords the scene header and the work move from r5 and r8 to r8 and sl (237 of 357 instructions differ), with or without the function's other devices. */
			Iwram_CopyWordsEntry((void *)BG_PLTT, buf, 0x1c0);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[5] + (u32)&ResourceId_Map001),
				gMapLayerData);
			MemFree(buf);
		}
	}
	cnt = 0;
	REG_MOSAIC = cnt;
	REG_BLDCNT = cnt;
	cnt = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	REG_DISPCNT = cnt;
	/* FAKEMATCH: Scheduler_AddOrUpdateCallback is called as void (*)(void (*)(void), s32); as s32 (*)(SchedulerCallback, s32), its prototype, Map_LoadLayeredScene has 6 of 868 bytes different */
	((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)(Map_UpdateLayerScroll, 0xc85);
	return 2;
}

void Map_ApplyWorkOriginAndSpan(void)
{
	struct MapScrollWork *work = gMapWork[0];
	s32					  x;
	s32					  y;
	s32					  height;
	s32					 *p;

	p = work->origin;
	x = 0;
	height = 0;
	y = 0;
	if (p != NULL)
	{
		x = *p++;
		height = *p++;
		y = *p;
	}
	Map_SetCameraCenter(x, y - height);
	Map_UpdateLayerScroll();
}

void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2)
{
	u8 *dest = BG_SCREEN_BLOCK(5) + (a0 << 11);
	u32 row = ((a2 / 2) & 0x7F) << 7;
	u32 rowmod = (a2 & 30) << 5;
	u32 col = (a1 / 2) & 0x7F;
	u32 colmod = a1 & 30;
	u32 counter;

	for (counter = 0; counter <= 15; counter++)
	{
		u32 *map = (u32 *)Ram_MapCellBuffer;
		u32 *tiles;
		u8	*cell;
		u32	 index;

		map += row + col;
		index = (*map << 20) >> 19;
		tiles = (u32 *)Ram_MapBlocks;
		tiles += index;
		cell = dest + (rowmod + colmod) * 2;
		*(u32 *)cell = *tiles;
		tiles = (u32 *)(Ram_MapBlocks + 4);
		tiles += index;
		*(u32 *)(cell + 64) = *tiles;
		col = (col + 1) & 0x7F;
		colmod = (colmod + 2) & 30;
	}
}

void Map_RenderPaletteMappedBlock(u32 a0, s32 a1, s32 a2)
{
	u8 *destination = BG_SCREEN_BLOCK(5) + (a0 << 11);
	u32 row = ((a2 / 2) & 0x7F) << 7;
	u32 rowmod = (a2 & 30) << 5;
	u32 col = (a1 / 2) & 0x7F;
	u32 colmod = a1 & 30;
	u32 parity = a1 & 1;
	u32 counter;

	for (counter = 0; counter <= 10; counter++)
	{
		u32 *map = (u32 *)Ram_MapCellBuffer;

		map += row + col;
		CopyBlock(map, destination, rowmod, colmod, parity);

		row = (row + 128) & 0x3F80;
		rowmod = (rowmod + 64) & 0x3C0;
	}
}

void Map_UpdateLayerScroll(void)
{
	struct MapScrollWork  *work;
	struct MapLayerScroll *layer;
	s32					  *origin;
	s32					   x, y;
	s32					   min_x, min_y, max_x, max_y;
	s32					   base_x, base_y;
	u32					   i;

	work = gMapWork[0];
	origin = work->origin;
	layer = work->layers;
	if (origin == 0)
		return;

	base_x = *origin++ - 0x780000;
	{
		s32 height = *origin++;
		base_y = *origin - height - 0x600000;
	}
	min_x = work->min_x + work->shake_x;
	max_x = work->max_x - work->shake_x - 0xf00000;
	min_y = work->min_y + work->shake_y;
	max_y = work->max_y - work->shake_y - 0xa00000;
	if (min_x > max_x)
		max_x = min_x;
	if (min_y > max_y)
		max_y = min_y;
	if (base_x < min_x)
		base_x = min_x;
	if (base_x > max_x)
		base_x = max_x;
	if (base_y < min_y)
		base_y = min_y;
	if (base_y > max_y)
		base_y = max_y;

	if (work->shake_x != 0)
	{
		s32 first = Random16();
		s32 second = Random16();
		s32 amplitude = work->shake_x;
		base_x += Iwram_MulQ16(amplitude, first - second);
		work->shake_x = Iwram_MulQ16(amplitude, work->shake_decay);
	}
	if (work->shake_y != 0)
	{
		s32 first = Random16();
		s32 second = Random16();
		s32 amplitude = work->shake_y;
		base_y += Iwram_MulQ16(amplitude, first - second);
		work->shake_y = Iwram_MulQ16(amplitude, work->shake_decay);
	}
	work->view_x = base_x;
	work->view_y = base_y;
	for (i = 0; i < 3; i++, layer++)
	{
		base_x = Iwram_MulQ16(work->view_x, layer->scale_x);
		base_y = Iwram_MulQ16(work->view_y, layer->scale_y);
		if (layer->speed_x != 0)
		{
			layer->phase_x += layer->speed_x;
			base_x = (base_x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
		}
		if (layer->speed_y != 0)
		{
			layer->phase_y += layer->speed_y;
			base_y = (base_y + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
		}
		base_x += layer->offset_x;
		base_y += layer->offset_y;
		x = base_x / 0x80000;
		y = base_y / 0x80000;
		if ((layer->x ^ base_x) & 0x80000)
		{
			if (layer->x < base_x)
				Map_RenderPaletteMappedBlock(i, x + 30, y);
			else
				Map_RenderPaletteMappedBlock(i, x, y);
		}
		if ((layer->y ^ base_y) & 0x100000)
		{
			if (layer->y < base_y)
				Map_RenderMetatileRow(i, x, y + 20);
			else
				Map_RenderMetatileRow(i, x, y);
		}
		gBgScroll[3 - i].x = base_x >> 16;
		gBgScroll[3 - i].y = base_y >> 16;
		layer->x = base_x;
		layer->y = base_y;
	}
}

void Map_SetCameraCenter(s32 x, s32 y)
{
	u32					   no;
	u8					  *dest;
	struct MapScrollWork  *work = gMapWork[0];
	struct MapLayerScroll *layer = work->layers;

	x -= 0x780000;
	y -= 0x600000;
	if (x < work->min_x)
		x = work->min_x;
	if (x > work->max_x - 0xf00000)
		x = work->max_x - 0xf00000;
	if (y < work->min_y)
		y = work->min_y;
	if (y > work->max_y - 0xa00000)
		y = work->max_y - 0xa00000;
	work->view_x = x;
	work->view_y = y;

	for (no = 0; no < 3; no++)
	{
		if (work->enabled[no])
		{
			u32 rows = 22;
			u32 row, col, rowmod, colmod, i, j;

			x = Iwram_MulQ16(work->view_x, layer->scale_x);
			y = Iwram_MulQ16(work->view_y, layer->scale_y);
			if (layer->speed_x)
			{
				layer->phase_x += layer->speed_x;
				x = (x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
			}
			if (layer->speed_y)
			{
				layer->phase_y += layer->speed_y;
				y = (y + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
				rows = 32;
			}
			x += layer->offset_x;
			y += layer->offset_y;
			layer++;
			x /= 0x80000;
			y /= 0x80000;
			dest = BG_SCREEN_BLOCK(5) + (no << 11);
			row = ((y / 2) & 127) << 7;
			rowmod = (y & 30) << 5;
			for (i = 0; i < rows / 2; i++)
			{
				col = (x / 2) & 127;
				colmod = x & 30;
				for (j = 0; j <= 15; j++)
				{
					u32 *map = (u32 *)Ram_MapCellBuffer;
					map += row + col;
					CopyCameraCell(map, dest, rowmod, colmod);
					col = (col + 1) & 127;
					colmod = (colmod + 2) & 30;
				}
				row = (row + 128) & 0x3f80;
				rowmod = (rowmod + 64) & 0x3c0;
			}
		}
	}
}

// レイヤーの表示範囲の左上（セル）
struct TilePos
{
	s32 x, y;
};

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y, u32 dst_x, u32 dst_y, u32 width, u32 height)
{
	/* FAKEMATCH: needed to match: temporary sharing preserves register allocation and scheduling without changing the frame or instructions. Measured 2026-10-11, before the volatile casts on the two word copies were dropped as unneeded: i = 0 after the offset differs 59, at the top of the body 32; a fresh bytes = cell * 8 is hoisted by loop.c, 158; the scale before the first destination, 4; address expressions without scr and blk, 113-153. A plain body differs 214. */
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
	struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
	struct TilePos		   tile[3];
	s32					   i;
	struct TilePos		  *pos = tile;
	u32					   y;
	u32					   x;

	for (i = 2; i >= 0; i--)
	{
		pos->x = layer->x >> 20;
		pos->y = layer->y >> 20;
		pos++;
		layer++;
	}
	for (y = dst_y; y < dst_y + height; y++)
	{
		for (x = dst_x; x < dst_x + width; x++)
		{
			u32 val = *src++;
			u32 cell = val & 0xfff;
			u32 offset;
			u32 tmp3;
			u32 n;
			u32 tmp6;
			u32 tmp9;
			u32 tmp12;
			u32 tmp11;
			u32 tmp13;
			u32 tmp15;
			tmp9 = *dst;
			tmp3 = tmp9 & -0x1000;
			tmp12 = x & 15;
			dst[0] = cell | tmp3;
			tmp6 = tmp12;
			dst++;
			i = 0;
			tmp15 = y & 15;
			tmp3 = tmp15;
			pos = tile;
			tmp13 = tmp3 << 5;
			tmp12 = tmp13;
			n = tmp12;
			n += tmp6;
			tmp13 = 4 * n;
			tmp11 = tmp13;
			tmp6 = tmp11;
			offset = tmp6;
			for (; i < 3; i++)
			{
				if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y
					&& pos->y + 12 > (s32)y)
				{
					u32 scr, blk;
					u8 *dest, *tiles;

					scr = 0x06002800;
					dest = (u8 *)scr;
					dest += offset;
					tmp11 = cell * 8;
					blk = (u32)Ram_MapBlocks;
					tiles = (u8 *)blk;
					tiles += tmp11;
					*(u32 *)dest = *(u32 *)tiles;

					blk = (u32)Ram_MapBlocks + 4;
					tiles = (u8 *)blk;
					tiles += tmp11;
					scr = 0x06002840;
					dest = (u8 *)scr;
					dest += offset;
					*(u32 *)dest = *(u32 *)tiles;
					break;
				}
				offset += 0x800;
				pos++;
			}
		}
		src += 128 - width;
		dst += 128 - width;
	}
}

struct MapCopyCommand
{
	u16 source;
	s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
	struct MapCopyCommand *record = (struct MapCopyCommand *)command;
	u32					   source = record->source;
	u32					   end = 0xffff;
	u32					   test = source;

	/* FAKEMATCH: the conditional cancelling updates end CSE before the
     * comparison, preserving its ip copy while source stays in r0. Both
     * updates disappear; removing them loses the copy (53 -> 52 English
     * instructions, five rows differ). */
	if (record)
	{
		source ^= test;
		source ^= test;
	}
	if (test != end)
	{
		u32	 stop = end;
		s16 *args = (s16 *)(command + 1);

		while (1)
		{
			s32 source_y = args[0];
			s32 width = args[1];
			s32 height = args[2];
			s32 delay = args[3];
			args += 5;

			Map_CopyMetatileIndicesRect(
				source, (u16)source_y, destination_x, destination_y, (u16)width, (u16)height);
			record++;
			WaitFrames((u16)delay);
			source = record->source;
			{
				u32 next = source;

				/* FAKEMATCH: the same ip copy after the wait. 2026-10-11: the native compares a copy of source held in ip at both tests; the copy's pseudo must reach regclass with only the copy and the branch as its insns, which CSE's copy swap or combine's substitution removes in every plain form tried (best, a do-while on the field re-read with stop in a local: differ 12). */
				__asm__("" : "+r"(next));
				if (next == stop)
					break;
			}
		}
	}
}

//--------------------------------------------------------------------------
// mapcopy.c
// マップ：セルの矩形の写し
//--------------------------------------------------------------------------

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff	   // ブロック番号
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 // 属性

//--------------------------------------------------------------------------
// セル（16×16 ドット）の矩形をそのまま写す
// 写した先が BG レイヤーの表示範囲（16×12 セル）にあれば、
// VRAM の BG マップにもブロックの 2×2 キャラを書く
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	s32					   y = dst_y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	s32					   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32	 val = *src++;
				u32	 cell;
				u32	 offset;
				u32	 tmp3;
				u32 *dest;
				u32	 n;
				u32 *tiles;
				u32	 tmp6;
				u32	 tmp12;
				u32	 tmp13;

				dst[0] = val;
				cell = val & 0xfff;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				i = 0;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 *tmp;
						u32	 tmp2;
						u32	 tmp4;
						u32 *tmp7;
						u32	 tmp10;
						u32	 tmp14;
						u32	 tmp16;
						/* FAKEMATCH: restoring the unscaled cell keeps loop.c's
						 * shift inside the successful layer branch. The restoration
						 * folds away, preserving the native 36-byte frame and registers.
						 * The copy chain through tmp2 to tmp17 sets the registers and
						 * stack slots: written as two plain word stores, tbs-en differs. */
						u32 bytes = cell * 8;
						u32 tmp8;
						u32 tmp17;

						cell = bytes / 8;
						tmp = (u32 *)Ram_MapBlocks;
						tmp8 = 0x06002800 + offset;
						tiles = tmp;
						tmp2 = tmp8;
						tmp = (u32 *)tmp2;
						dest = tmp;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp16 = *tiles;
						tmp14 = tmp16;
						tmp17 = 0x06002840 + offset;
						tmp10 = tmp14;
						*dest = tmp10;
						tmp7 = (u32 *)(Ram_MapBlocks + 4);
						tiles = tmp7;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp8 = tmp17;
						tmp14 = tmp8;
						tmp17 = tiles[0];
						tmp16 = tmp17;
						tmp10 = tmp16;
						tmp4 = tmp10;
						tmp2 = tmp4;
						tmp4 = tmp14;
						tmp7 = (u32 *)tmp4;
						dest = tmp7;
						*dest = tmp2;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src += (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}

//--------------------------------------------------------------------------
// セルの矩形の属性だけを写す。写す先のブロック番号はそのまま
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	u32 *dst = MAP_CELLS;
	u32 *src = dst;
	u16	 row;
	u16	 col;

	dst += (dst_y << 7) + dst_x;
	src += (src_y << 7) + src_x;

	for (row = 0; row < height; row++)
	{
		u32 *dst_cell = dst + (row << 7);
		u32 *src_cell = src + (row << 7);

		for (col = 0; col < width; col++)
		{
			*dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) | (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
			dst_cell++;
		}
	}
}

//--------------------------------------------------------------------------
// セルの矩形のブロック番号だけを写す。写す先の属性はそのまま
// 表示範囲にあれば VRAM の BG マップも書きかえる
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellIndexRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	/* FAKEMATCH: needed to match: word/index temporaries preserve register allocation; the plain body allocates differently and uses a 40-byte frame. Measured 2026-10-11, before the volatile casts on the two word copies were dropped as unneeded: i = 0 after pos = view differs 38; the scale into n, 52; the scale before the first destination, 54. A plain body differs 204. */
	s32					   y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[dst_y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	s32					   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	y = dst_y;
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32 val = *src++;
				u32 cell;
				u32 offset;
				u32 tmp3;
				u32 n;
				u32 tmp6;
				u32 tmp12;
				u32 tmp13;

				cell = val & MAP_CELL_TILE_MASK;
				dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				i = 0;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 scr, blk;
						u8 *dest, *tiles;

						scr = 0x06002800;
						dest = (u8 *)scr;
						dest += offset;
						tmp13 = cell * 8;
						blk = (u32)Ram_MapBlocks;
						tiles = (u8 *)blk;
						tiles += tmp13;
						*(u32 *)dest = *(u32 *)tiles;

						blk = (u32)Ram_MapBlocks + 4;
						tiles = (u8 *)blk;
						tiles += tmp13;
						scr = 0x06002840;
						dest = (u8 *)scr;
						dest += offset;
						*(u32 *)dest = *(u32 *)tiles;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src = src + (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}

void Map_SetWorkFlagBits9To11(u32 v)
{
	struct MapState *state = gMapWork[0];
	u32				 mask = v & 0xe00;
	u32				 flags = state->flags;

	flags = (flags & 0xf1ff) | mask;
	state->flags = flags;
}

static __inline__ void Io_Set16(s32 value, volatile u16 *reg)
{
	*reg = value;
}

static __inline__ void Io_Put16(volatile u16 *reg, s32 value)
{
	*reg = value;
}

typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct WorldCell
{
	u32 tile : 24;
	u32 kind : 7;
	u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update)
{
	struct PerspectiveWork *work = gMapWork[0];
	u32					   *graphics = work->tiles;
	u16					   *shown;
	u16					   *buffer;
	u16					   *source;
	u8					   *blocks;
	u16					   *screen;
	u32						i;
	u32						j;
	u32						id;

	x &= 1;
	y &= 1;
	shown = &work->quarter_tiles[(layer * 2 + y) * 2 + x];
	if (update == 0 && tile == *shown)
		return 0;
	*shown = tile;
	buffer = Runtime_AllocateHeapBlock(14, 0x400);
	Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

	source = buffer;
	blocks = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
	for (i = 0; i < 16; i++)
	{
		Dma_Set(source, blocks, DMA_ENABLE32 | DMA_32BIT | 0x10, REG_DMA3);
		source += 32;
		blocks += 128;
	}

	if (update != 0)
	{
		screen = (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
		source = buffer;
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				u16 *bottom;

				id = *source;
				screen[0] = *(u16 *)(Ram_MapCellBuffer + id * 4);
				bottom = (u16 *)(Ram_MapCellBuffer + 2);
				bottom = (u16 *)((u8 *)bottom + id * 4);
				screen[32] = *bottom;
				screen++;
				source += 2;
			}
			screen += 48;
		}
	}
	Runtime_ReleaseHeapBlock(14);
	return 1;
}

s32 Map_InitializePerspectiveScene(void)
{
	struct PerspectiveWork *work;
	struct BattleCamera	   *camera;
	struct MapFrameWork	   *tiles;
	struct MapAffinePair(*lines)[160];
	s32						 *position;
	s32						 *distance;
	u16						 *yaw;
	u16						 *pitch;
	u16						 *turn;
	/* FAKEMATCH: the fill word is this function's own volatile local, filled by Dma_Fill32Cell, so it takes the native's frame slot at sp+28; plain Dma_Fill32's inner word lands at sp+16 and the slots after it shift (differ 4 in all six editions, measured 2026-10-13). */
	volatile u32			  fill;
	struct FixedPointPosition vector;
	s32						  far_plane;
	s32						  i;

	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	Blend_SetDarkenTarget0(0);
	work = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
	Dma_Fill32Cell(fill, 0, work, sizeof(struct PerspectiveWork));
	work->view_x = 0;
	work->view_y = 0;
	work->scale_x = 0x200000;
	work->scale_y = 0x400000;
	work->limit_x = 0x1fe00000;
	work->limit_y = 0x1fe00000;
	work->script = NULL;
	work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), Ram_MapCollision + 0x1000);
	MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
	Io_Set16(0x3f9e, &REG_BLDCNT);
	Io_Set16(0x1010, &REG_BLDALPHA);
	REG_BLDY = 0;
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), Ram_MapCollision);
	work->fade = 0x1f00;
	work->fade_step = 0x80;
	Io_Set16(0xa80a, &REG_BG3CNT);
	Io_Set16(0xaa0e, &REG_BG2CNT);
	Io_Set16(0x0501, &REG_BG1CNT);
	Io_Write16(0x100, &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	/* FAKEMATCH: one-pass block; its loop notes keep the 0x100 for BG2PD built just before its store and the 12 for Runtime_AllocateBlock loaded before the 76. Without it differ 6, whether the three writes are Io_Put16, Io_Set16 or plain stores; Io_Write16 for BG2PD (itself a one-pass macro) differ 2. */
	do
	{
		Io_Put16(&REG_BG2PD, 0x100);
		REG_BG2X = 0;
		REG_BG2Y = 0;
		Io_Put16(&REG_BG3PA, 0x100);
		REG_BG3PB = 0;
		REG_BG3PC = 0;
		Io_Put16(&REG_BG3PD, 0x100);
		REG_BG3X = 0;
		REG_BG3Y = 0;
	} while (0);

	camera = Runtime_AllocateBlock(12, sizeof(struct BattleCamera));
	tiles = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
	position = camera->pos;
	lines = tiles->pages;
	far_plane = 0x1fe0000;
	work->far_plane = far_plane;
	distance = &work->distance;
	*distance = far_plane;
	work->zoom = 0x10000;
	turn = &work->zoom_transition_frames;
	camera->unknown_18 = 0;
	camera->follow_pos = 0;
	*turn = 0;
	gProjection.center_x = 120;
	gProjection.center_y = 96;
	Camera_StoreSceneParameters(far_plane, far_plane >> 1, far_plane << 1);
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(position);
	yaw = &work->yaw;
	SceneTransform_ApplyYaw(*yaw);
	pitch = &work->pitch;
	SceneTransform_ApplyPitch(*pitch);
	vector.x = 0;
	vector.y = 0;
	vector.z = far_plane;
	Iwram_TransformVector((const s32 *)&vector, (s32 *)camera);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(camera->eye, position);
	RamCode_LoadSlot(46, Transform_UpdateVertices, &Transform_UpdateVerticesSize);
	WorldMap_BuildScanlineTable(
		Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), position, tiles->scanlines);
	gScanlineAngle = 0;
	gScanlinePitch = work->pitch;
	((PlaneFn)gWorkSlot[46])(camera, position, tiles->scanlines, lines[gFrameCount & 1]);
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	Render_ResetTransformState();
	Io_Put16(pitch, 0xe000);
	*yaw = 0;
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(position);
	SceneTransform_ApplyYaw(*yaw);
	SceneTransform_ApplyPitch(*pitch);
	vector.x = 0;
	vector.y = 0;
	vector.z = *distance + 0x10000;
	Iwram_TransformVector((s32 *)&vector, camera->eye);
	REG_MOSAIC = 0;
	Io_Write16(0x42, &REG_DISPCNT);
	gBgScroll[1].x = 0;
	gBgScroll[1].y = 0;
	gBgScroll[2].x = 0;
	gBgScroll[2].y = 0;
	gBgScroll[3].x = 0;
	gBgScroll[3].y = 0;
	work->window_top = 0;
	work->window_bottom = 159;
	Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
	Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
	for (i = 255; i >= 0; i--)
		work->tile_ids[i] = i;
}

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py)
{
	struct PerspectiveWork *window;
	s32					   *position;
	s32						origin_x;
	s32						origin_y;
	s32						tile;

	window = gMapWork[0];
	origin_x = 0;
	origin_y = 0;
	position = window->origin;
	if (position != 0)
	{
		origin_x = *position++;
		origin_y = position[1];
	}

	origin_x >>= 24;
	origin_y >>= 24;
	x >>= 4;
	y >>= 4;
	px >>= 3;
	py >>= 3;
	tile = (y << 4) + x;
	window->tile_ids[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tile;

	if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1)
	{
		Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
		Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
	}
}

void WorldMap_LoadGraphics(s32 x, s32 z)
{
	struct PerspectiveWork *state;
	u32					   *resources;
	u8					   *buffer;
	u8					   *tiles;
	u32					   *cursor;
	u32						tile;
	s32						variant;
	s32						row;
	s32						col;
	s16						value;

	variant = 0;
	buffer = MemAlloc(0x200);
	state = gMapWork[0];
	if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind == 21)
		variant = 1;
	resources = WorldMap_GraphicsResources[variant];
	state->resources = resources;
	value = BG_PALETTE[0];
	Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
	*(s16 *)buffer = value;
	Dma_Set(buffer, BG_PALETTE, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
	tiles = gBgTileBuffer;
	Resource_DecodeType01(Resource_GetTableEntry(resources[1]), tiles);
	Dma_Set(tiles, BG_CHAR_BLOCK(2), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
	Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800,
		REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
	Dma_Set(
		Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
	Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800,
		REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
	Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 0x600);
	cursor = (u32 *)BG_SCREEN_BLOCK(6);
	tile = 0x01a901a8;
	for (row = 0; row < 20; row++)
	{
		for (col = 0; col < 15; col++)
		{
			*cursor++ = tile;
			tile += 0x00020002;
		}
		cursor++;
	}
	if (variant == 1)
	{
		state->tile_ids[10] = 0x10a;
		state->tile_ids[11] = 0x10b;
		state->tile_ids[12] = 0x10c;
		state->tile_ids[26] = 0x11a;
		state->tile_ids[27] = 0x11b;
		state->tile_ids[28] = 0x11c;
		Map_UpdateCurrentTileBlock();
	}
	MemFree(buffer);
}

void MapAnimation_ApplyAffineFrame(void)
{
	struct MapAffinePair(*lines)[160] = ((struct MapFrameWork *)WORK_SLOT(7))->pages;
	s32						dispcnt = (s16)(REG_DISPCNT & 0xfff8);
	u32					   *dst = (u32 *)&REG_BG2PA;
	struct PerspectiveWork *work = WORK_SLOT(8);
	volatile u16		   *channel = REG_DMA0;
	u32					   *src;
	u32						mode;
	u32						top;
	u32						control;

	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	if (lines != NULL)
	{
		src = (u32 *)lines[gFrameCount & 1];
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst = *src++;
		Dma_Set(src, (void *)&REG_BG2PA, 0xa6600008, (volatile u32 *)channel);
	}
	work->shown_top = work->window_top;
	work->shown_bottom = work->window_bottom;
	top = work->shown_top;
	mode = 0;
	if (top < 200)
	{
		u16 bottom = work->shown_bottom;

		mode = (bottom != 0) << 1;
		if (top <= bottom)
		{
			mode = 0;
			if (top == 0)
				mode = 2;
		}
	}
	dispcnt |= mode;
	control = (u16)dispcnt;
	REG_DISPCNT = control;
	work->window_pending = 0;
}

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
	u32	 palette = *(u16 *)map;
	u16 *colors = (u16 *)Ram_MapCellBuffer;

	colors += palette * 2;
	*(u16 *)destination = *colors++;
	*(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedRow(u32 value)
{
	u8 *map;
	u8 *destination;
	u32 counter;

	map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 7);
	destination = (u8 *)(0x06004000 + ((value & 62) << 6));
	counter = 0;
	do
	{
		CopyEntry(map, destination);
		counter++;
		destination += 2;
		map += 4;
	} while (counter <= 31);

	destination += 0xfc0;
	map += 0xf80;
	counter = 0;
	do
	{
		CopyEntry(map, destination);
		counter++;
		destination += 2;
		map += 4;
	} while (counter <= 31);
}

void Map_RenderPaletteMappedColumn(u32 value)
{
	u8 *map;
	u8 *destination;
	u32 counter;

	map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 2);
	destination = (u8 *)(0x06004000 + (value & 62));
	counter = 0;
	do
	{
		CopyEntry(map, destination);
		counter++;
		destination += 128;
		map += 128;
	} while (counter <= 63);
}

void WorldMap_UpdateView(void)
{
	struct BattleCamera	   *cam = WORK_SLOT(HEAP_SLOT_CAMERA);
	struct MapFrameWork	   *map = WORK_SLOT(7);
	struct PerspectiveWork *view = WORK_SLOT(8);
	s32					   *pos = cam->pos;
	s32					   *target = view->origin;
	struct MapAffinePair(*buffer)[160] = map->pages;
	s32						  distance = view->far_plane;
	s32						  height = view->distance;
	struct FixedPointPosition local;

	Map_UpdateCurrentTileBlockUntilBlocked();
	if (target != NULL)
	{
		s32 x;
		s32 y;
		s32 col;
		s32 row;

		y = target[2];
		x = target[0];
		if (view->shake_x != 0)
		{
			s32 r = Random16();
			s32 amp;

			r -= Random16();
			amp = view->shake_x;
			x += Iwram_MulQ16(amp, r);
			view->shake_x = Iwram_MulQ16(amp, view->shake_decay);
		}
		if (view->shake_y != 0)
		{
			s32 r = Random16();
			s32 amp;

			r -= Random16();
			amp = view->shake_y;
			y += Iwram_MulQ16(amp, r);
			view->shake_y = Iwram_MulQ16(amp, view->shake_decay);
		}
		col = x / 0x100000;
		row = y / 0x100000;
		if ((view->view_x ^ x) & 0x100000)
		{
			if (view->view_x < x)
				Map_RenderPaletteMappedColumn(col + 16);
			else
				Map_RenderPaletteMappedColumn(col - 16);
		}
		if ((view->view_y ^ y) & 0x100000)
		{
			if (view->view_y < y)
				Map_RenderPaletteMappedRow(row + 12);
			else
				Map_RenderPaletteMappedRow(row - 18);
		}
		view->view_x = x;
		view->view_y = y;
	}
	gProjection.center_x = 120;
	gProjection.center_y = 96;
	Camera_StoreSceneParameters(distance, height / 2, height * 2);
	pos[0] = *target++;
	pos[1] = 0;
	pos[2] = target[1];
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw(view->yaw);
	SceneTransform_ApplyPitch(view->pitch);
	local.x = 0;
	local.y = 0;
	local.z = height + 0x10000;
	Iwram_TransformVector((s32 *)&local, (s32 *)cam);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(cam, pos);
	if (gScanlinePitch != view->pitch)
	{
		s32 c = Trig_Cos(view->pitch);
		s32 s = Trig_Sin(view->pitch);

		WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map->scanlines);
		gScanlineAngle = 0;
		gScanlinePitch = view->pitch;
	}
	((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(
		cam, pos, map->scanlines, buffer[gFrameCount & 1]);
}

void Map_UpdateCurrentTileBlock(void)
{
	struct PerspectiveWork *window = gMapWork[0];
	s32						x0 = 0;
	s32						y0 = 0;
	u32						layer;
	u32						row;
	u32						col;
	s32						bias;
	s32						tile;

	if (window->origin != NULL)
	{
		s32 *p = window->origin;

		x0 = *p++;
		y0 = p[1];
	}
	x0 = (x0 - 0x1000000) >> 25;
	y0 = (y0 - 0x1400000) >> 25;
	layer = 0;
	bias = 0;
	for (; layer < 2; layer++)
	{
		for (row = 0; row < 2; row++)
		{
			for (col = 0; col < 2; col++)
			{
				tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
				tile = window->tile_ids[tile];
				tile += bias;
				Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
			}
		}
		bias += 320;
	}
}

void Map_UpdateCurrentTileBlockUntilBlocked(void)
{
	struct PerspectiveWork *window = gMapWork[0];
	s32						x0 = 0;
	s32						y0 = 0;
	u32						layer;
	u32						row;
	u32						col;
	s32						tile;

	if (window->origin != NULL)
	{
		s32 *p = window->origin;

		x0 = *p++;
		y0 = p[1];
	}
	x0 = (x0 - 0x1000000) >> 25;
	y0 = (y0 - 0x1400000) >> 25;
	for (layer = 0; layer < 2; layer++)
	{
		for (row = 0; row < 2; row++)
		{
			for (col = 0; col < 2; col++)
			{
				tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
				tile = window->tile_ids[tile];
				tile += layer * 320;
				if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
					return;
			}
		}
	}
}

void MapAnimation_PresentFrame(void)
{
	Io_Write16(0x682, &REG_BG1CNT);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
	struct MapFrameWork	   *pages = WORK_SLOT(7);
	struct PerspectiveWork *work = WORK_SLOT(8);
	s32						one = 1;

	work->animation_active = one;
	Scheduler_EnableCallbacks(MapAnimation_Update);
	Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
	WaitFrames(1);
	Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one], (s32)gMapCellBuffer);
	work->window_top = 200;
	work->window_bottom = 255;
	gVBlankHook = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
	Io_Write16(0x501, &REG_BG1CNT);
	Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000, REG_DMA3);
}

void Map_LoadAreaGraphics(void)
{
	struct PerspectiveWork *state;
	u32					   *resources;
	u8					   *buffer;
	s16						value;

	state = gMapWork[0];
	buffer = (u8 *)gMapCellBuffer;
	resources = state->resources;
	value = *(s16 *)BG_PLTT;
	Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
	*(s16 *)buffer = value;
	Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[1]), gBgTileBuffer);
	Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
	Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
	Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
	gVBlankHook = Map_ShowBg1FromBuffer;
	state->window_top = 0;
	state->window_bottom = 159;
	WaitFrames(1);
	Resource_DecodeType01(Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
	Map_UpdateCurrentTileBlock();
	state->animation_active = 0;
	Scheduler_DisableCallbacks(MapAnimation_Update);
	WaitFrames(1);
}

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
	struct PerspectiveWork *work = gMapWork[0];
	gVBlankHook = Map_ShowBg1FromBuffer;
	work->window_top = 0;
	work->window_bottom = 0x9f;
	WaitFrames(1);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
	Map_UpdateCurrentTileBlock();
	Scheduler_DisableCallbacks(MapAnimation_Update);
	WaitFrames(1);
}

void MapAnimation_Update(void)
{
	/* FAKEMATCH: each channel is written (state->anim + i), not through a channel pointer: a pointer anim = state->anim + i (or &state->anim[i], declared in either scope) differs 4, ip and lr swapping the 0xffff sentinel and the VRAM base; a walking anim++ differs 8; state->anim[i]. differs 167, no walking pointer at all. */
	struct MapState *state = gMapWork[0];
	u32				 i;

	for (i = 0; i <= 15; i++)
	{
		const u16 *script;
		u32		   op;
		u32		   count;
		u32		   dst;

		if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
			continue;
	next:
		if ((state->anim + i)->timer == 0)
		{
			script = (state->anim + i)->cursor;
			op = *script++;
			if (op == 0xffff)
			{
				(state->anim + i)->cursor = (state->anim + i)->start;
				goto next;
			}
			if ((op & 0xff00) == 0xfe00)
			{
				if ((op & 0xff) == 0xff)
					continue;
				(state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
				goto next;
			}
			count = *script++;
			dst = script[0];
			(state->anim + i)->timer = script[1];
			if (state->wide_tiles == 0)
			{
				if (op >= 0x600)
					Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
						DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
				else
					Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
						DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
			}
			else
			{
				if (op >= 0x200)
					Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
						DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
				else
					Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
						DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
			}
			(state->anim + i)->cursor += 4;
			goto next;
		}
		else
		{
			(state->anim + i)->timer--;
		}
	}
}

void Map_ClearLayerEntryFlag(u32 no)
{
	struct MapState		*state = gMapWork[0];
	struct MapAnimation *anim = &state->anim[no];

	anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
	struct MapState		*state = gMapWork[0];
	struct MapAnimation *anim = &state->anim[no];

	anim->paused = 1;
}

void MapAnimation_StartChannels(const u16 *script)
{
	struct MapState *state;
	s32				 count;
	u32				 command;

	state = gMapWork[0];
	count = 0;
	Dma_Fill32(0, state->anim, sizeof(state->anim));
	command = *script++;
	while (command != 0xffff)
	{
		if ((command & 0xff00) == 0xfd00)
		{
			struct MapAnimation *anim;
			u32					 idx = command & 15;
			u32					 flag = 0;
			if (command & 0x80)
				flag = 1;
			anim = &state->anim[idx];
			anim->start = script;
			anim->cursor = script;
			anim->timer = 0;
			anim->paused = flag;
			count++;
		}
		command = *script++;
	}
	if (count != 0)
		Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

void Map_EnableUpdateCallback(void)
{
	if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
		Scheduler_EnableCallbacks(MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
	if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
		Scheduler_DisableCallbacks(MapAnimation_Update);
}

void DisplayBlend_RunScript(void)
{
	struct MapState			*work = gMapWork[0];
	struct BlendScriptState *state;
	u16						*cursor;
	u32						 command;
	u32						 value;
	u16						 test;

	state = &work->blend;
	if (state->script == 0)
		return;
	if (state->paused != 0)
		return;

again:
	value = state->delay;
	test = value;
	if (test == 0)
	{
		cursor = state->cursor;
		command = *cursor;
		cursor++;
		if (command == 0xffff)
		{
			state->cursor = state->script;
			goto again;
		}

		if ((command & 0xff00) == 0xfe00)
		{
			value = command & 0xff;
			if (value == 0xff)
				return;
			state->cursor = state->script + value * 2;
			goto again;
		}

		if ((command & 0xf000) == 0x3000)
		{
			REG_BLDCNT = command;
			work->blend_control = command;
			state->cursor++;
			goto again;
		}

		if ((work->blend_control & 0xc0) == 0x40)
			REG_BLDALPHA = command;
		else
			REG_BLDY = command;
		state->delay = cursor[0];
		state->cursor += 2;
		goto again;
	}
	state->delay = value + 0xffff;
}

void DisplayBlend_StartScript(u16 *script)
{
	struct BlendScriptState *state;
	s32						 started;
	volatile u32			 zero;

	started = 0;
	zero = 0;
	state = &((struct MapState *)gMapWork[0])->blend;
	Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
	if (*script != 0xffff)
	{
		state->script = script;
		state->cursor = script;
		state->delay = 0;
		state->paused = 0;
		started = 1;
	}
	if (started)
		Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

void DisplayBlend_EnableRunScript(void)
{
	Scheduler_EnableCallbacks(DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
	Scheduler_DisableCallbacks(DisplayBlend_RunScript);
}

void Runtime_AllocateAndClearQueue(void)
{
	struct PaletteSnapshotWork *queue;
	struct PaletteSnapshot	   *entry;
	u16							i;
	u16							j;

	queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
	entry = queue->entries;
	for (i = 0; i != 4; i++)
	{
		entry->source = 0;
		entry->position = 0;
		entry->timer = 0;
		entry->delay = 0;
		entry->count = 0;
		for (j = 0; j != 16; j++)
		{
			entry->colors[j] = 0;
		}
		entry++;
	}
	queue->count = 0;
}

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
	struct PaletteSnapshotWork *work;
	struct PaletteSnapshot	   *entry;
	u16						   *source;
	u32							slot;
	u16							size;

	work = gPaletteWork;
	slot = work->count;
	if (slot > 3)
		return -1;
	entry = &work->entries[slot];
	source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
	size = count;
	entry->position = 0;
	entry->timer = 0;
	entry->count = size;
	entry->source = source;
	entry->delay = delay;
	Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
	work->count++;
	return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
	Scheduler_RemoveCallback(PaletteQueue_StepCycles);
	Runtime_ReleaseHeapBlock(0x1C);
}

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

void PaletteQueue_StepCycles(void)
{
	struct PaletteSnapshotWork *work = gPaletteWork;
	u8							i;
	u16							count = work->count & 3;
	u16							buf[16];

	for (i = 0; i < count; i++)
	{
		struct PaletteSnapshot *cycle = &work->entries[i];
		if (cycle->timer == 0)
		{
			u16	  start = cycle->position;
			u16	  len = cycle->count;
			void *dest = cycle->source;
			u16	 *src = cycle->colors;
			u8	  j;
			for (j = len - start; j < len; j++)
				buf[j] = *src++;
			for (j = 0; j < len - start; j++)
				buf[j] = *src++;
			Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
			start++;
			if (start >= len)
				start = 0;
			cycle->position = start;
			cycle->timer = cycle->delay;
		}
		else
		{
			cycle->timer--;
		}
	}
}

s32 Curve_GetFirstSampleA(const s8 *samples)
{
	return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Curve_LerpTwoSamples(const s8 *samples, s32 position)
{
	s32 start;
	s32 delta;

	start = samples[0] << CURVE_VALUE_SHIFT;
	delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * position;
	if (delta < 0)
	{
		delta += CURVE_FULL_STEPS - 1;
	}
	return start + (delta >> 4);
}

s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position)
{
	s32 base;
	s32 offset;

	base = samples[0] << CURVE_VALUE_SHIFT;
	offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * position;
	if (offset < 0)
	{
		offset += CURVE_FULL_STEPS - 1;
	}
	return base + (offset >> 4);
}

s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end)
{
	s32 first;
	s32 second;
	s32 maximum;
	u32 position;

	first = samples[0] << CURVE_VALUE_SHIFT;
	second = samples[1] << CURVE_VALUE_SHIFT;
	maximum = first;
	if (second > first)
	{
		maximum = second;
	}
	position = start + end;
	if (position == CURVE_FULL_STEPS - 1)
	{
		return maximum;
	}
	if (position < CURVE_FULL_STEPS - 1)
	{
		second = first;
	}
	return second;
}

s32 Curve_StepAtDifferencePosition(const s8 *samples, s32 start, s32 end)
{
	s32 first;
	s32 second;
	s32 maximum;
	s32 difference;
	u32 position = start;

	first = samples[0] << CURVE_VALUE_SHIFT;
	second = samples[1] << CURVE_VALUE_SHIFT;
	maximum = first;
	if (second > first)
	{
		maximum = second;
	}
	difference = end - position;
	position = difference + CURVE_FULL_STEPS - 1;
	if (position == CURVE_FULL_STEPS - 1)
	{
		return maximum;
	}
	if (position < CURVE_FULL_STEPS - 1)
	{
		second = first;
	}
	return second;
}

s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 start, s32 end)
{
	s32 first;
	s32 middle;
	s32 last;
	s32 result;

	first = *samples++ << CURVE_VALUE_SHIFT;
	middle = *samples << CURVE_VALUE_SHIFT;
	last = samples[1] << CURVE_VALUE_SHIFT;
	start += end;
	if (start == CURVE_FULL_STEPS - 1)
	{
		result = middle;
	}
	else if ((u32)start < CURVE_FULL_STEPS - 1)
	{
		result = first + (middle - first) * start / (CURVE_FULL_STEPS - 1);
	}
	else
	{
		start -= (CURVE_FULL_STEPS - 1);
		result = middle + (last - middle) * start / (CURVE_FULL_STEPS - 1);
	}
	return result;
}

s32 Curve_LerpThreeSamplesAtDifference(const s8 *samples, s32 start, s32 end)
{
	s32 first;
	s32 second;
	s32 third;
	s32 difference;
	s32 delta;

	first = *samples++ << CURVE_VALUE_SHIFT;
	second = samples[0] << CURVE_VALUE_SHIFT;
	third = samples[1];
	difference = end - start;
	start = difference + CURVE_FULL_STEPS - 1;
	third <<= CURVE_VALUE_SHIFT;

	if (start == CURVE_FULL_STEPS - 1)
		return second;

	if ((u32)start < CURVE_FULL_STEPS - 1)
	{
		delta = second - first;
		difference = delta * start;
		if (difference < 0)
			difference += CURVE_FULL_STEPS - 1;
		difference >>= 4;
		return first + difference;
	}
	difference *= third - second;
	if (difference < 0)
		difference += CURVE_FULL_STEPS - 1;
	difference >>= 4;
	return second + difference;
}

s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row)
{
	s32 start;

	start = samples[0] << CURVE_VALUE_SHIFT;
	return start
		   + (((samples[1] << CURVE_VALUE_SHIFT) - start)
			   * Curve_LerpWeightTable[position + (row * CURVE_FULL_STEPS)]);
}

s32 Curve_StepAtHalf(const s8 *samples, u32 position)
{
	s32 value;
	if (position < CURVE_HALF_STEPS)
		value = samples[0];
	else
		value = samples[1];
	return value << CURVE_VALUE_SHIFT;
}

s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position)
{
	s32 value;
	if (position < CURVE_HALF_STEPS)
		value = samples[0];
	else
		value = samples[1];
	return value << CURVE_VALUE_SHIFT;
}

s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 position)
{
	s32 a;
	s32 b;
	s32 delta;

	a = *samples++ << CURVE_VALUE_SHIFT;
	b = *samples++ << CURVE_VALUE_SHIFT;
	if (position < CURVE_HALF_STEPS)
	{
		delta = (b - a) * position;
		if (delta < 0)
		{
			delta += CURVE_HALF_STEPS - 1;
		}
		return a + (delta >> 3);
	}

	{
		s32 pos;

		a = *samples << CURVE_VALUE_SHIFT;
		pos = position;
		a -= b;
		pos -= CURVE_HALF_STEPS;
		delta = a * pos;
		if (delta < 0)
		{
			delta += CURVE_HALF_STEPS - 1;
		}
		return b + (delta >> 3);
	}
}

s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 position)
{
	s32 a;
	s32 b;
	s32 pos;
	s32 delta;
	s32 tail;

	a = *samples++ << CURVE_VALUE_SHIFT;
	b = *samples++ << CURVE_VALUE_SHIFT;
	if (position < CURVE_HALF_STEPS)
	{
		delta = (b - a) * position;
		if (delta < 0)
		{
			delta += CURVE_HALF_STEPS - 1;
		}
		return a + (delta >> 3);
	}

	a = *samples << CURVE_VALUE_SHIFT;
	pos = position;
	a -= b;
	pos -= CURVE_HALF_STEPS;
	tail = a * pos;
	if (tail < 0)
	{
		tail += CURVE_HALF_STEPS - 1;
	}
	return b + (tail >> 3);
}

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
	return samples[Curve_SampleIndexTable[position + (row << 4)]] << CURVE_VALUE_SHIFT;
}

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
	return samples[Curve_SampleIndexTable[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
		   << CURVE_VALUE_SHIFT;
}

s32 Curve_GetFirstSampleB(const s8 *samples)
{
	return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Curve_GetFirstSampleC(const s8 *samples)
{
	return samples[0] << CURVE_VALUE_SHIFT;
}

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

extern TerrainHeightFn Map_TerrainHeightFunctions[16];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y)
{
	struct MapState *work = gMapWork[0];
	struct MapCell	*cells;
	u8				*attributes;

	x >>= 16;
	y >>= 16;
	cells = (struct MapCell *)Ram_MapCellBuffer;
	if (work != NULL)
	{
		cells = work->layers[layer & 3].cells;
	}
	cells += x / 16 + (y / 16 << 7);
	attributes = Ram_MapCollision + cells->attribute_b * 4;
	return Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, y & 15);
}

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
	struct MapState *state = gMapWork[0];
	struct MapCell	*map;
	s32				 col;
	s32				 row;
	u32				 attr;

	x >>= 16;
	y >>= 16;
	map = (struct MapCell *)gMapCellBuffer;
	if (state != 0)
	{
		map = state->layers[index & 3].cells;
	}
	col = x / 16;
	row = y / 16;
	map += col + (row << 7);
	attr = map->attribute_b;
	return *(Ram_MapCollision + attr * 4) & 15;
}

s32 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
	struct MapState *state;
	struct MapCell	*cells;

	state = gMapWork[0];
	x >>= 20;
	y >>= 20;
	cells = (struct MapCell *)gMapCellBuffer;
	if (state != NULL)
	{
		cells = state->layers[layer & 3].cells;
	}
	cells += x + (y << 7);
	return cells->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
	struct MapState *state = gMapWork[0];

	x >>= 20;
	y >>= 20;
	if (state != NULL)
	{
		struct MapCell *cells = state->layers[layer & 3].cells;

		cells += x + (y << 7);
		cells->collision_code = collision_code;
	}
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
	s32				tile_x = x / 16;
	s32				tile_y = y / 16;
	struct MapCell *cell = (struct MapCell *)gMapCellBuffer + (tile_x + tile_y * 128);

	return ((u8 *)&cell->metatile_and_flags)[1] >> 6;
}

s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position)
{
	struct MapState *state;
	s32				 x;
	s32				 z;
	struct MapCell	*cell;
	u8				*attributes;
	s32				 height;
	s32				 delta;

	x = ((union FieldCoordinate *)&position->x)->part.pixel;
	z = ((union FieldCoordinate *)&position->z)->part.pixel;
	state = gMapWork[0];
	if (state == NULL)
		return 0;
	if (object->terrain_id <= 2)
		cell = state->layers[object->terrain_id].cells;
	else
		cell = (struct MapCell *)Ram_MapCellBuffer;
	cell += x / 16 + (z / 16 << 7);
	if (cell->collision_code == 0xff)
		return 2;
	attributes = Ram_MapCollision + cell->attribute_b * 4;
	height = Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, z & 15);
	delta = height - object->terrain_height;
	if (delta > 0x80000)
		return 1;
	if (delta < -0xc0000)
		return -1;
	return 0;
}

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
	s32				 x;
	s32				 y;
	s32				 tile_x;
	s32				 tile_y;
	struct MapState *work;
	struct MapCell	*cell;

	x = position->x / 65536;
	y = (position->y - position->height) / 65536;
	work = gMapWork[0];
	if (work == NULL)
		return 0;
	cell = work->layers[2].cells;
	tile_x = x / 16;
	tile_y = y / 16;
	cell += tile_x + tile_y * 128;
	return (cell->collision_code != 0xff) - 1;
}

s32 GetWorldMapCollision(struct WorldPosition *position)
{
	s32 x_step = position->x >> 17;
	s32 y = position->y;
	s32 y_step = y >> 17;
	u32 cell;
	u32 pixel_offset;
	u8	tile;
	u8	packed_pixels;
	u32 result;

	cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
	pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

	tile = BG_SCREEN_BLOCK(10)[cell];
	packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
	if (packed_pixels != 0)
	{
		if (x_step & 2)
			result = packed_pixels >> 4;
		else
			result = packed_pixels & 15;
		if (result != 0)
			return result;
	}

	tile = BG_SCREEN_BLOCK(8)[cell];
	packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
	if (packed_pixels != 0)
	{
		if (x_step & 2)
			result = packed_pixels >> 4;
		else
			result = packed_pixels & 15;
		if (result != 0)
			return result;
	}

	return 7;
}

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
	if ((GetWorldMapCollision(position) - 5) <= 7U)
	{
		return 0;
	}
	return -1;
}

s32 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
	s32	 selector = GetWorldMapCollision(position);
	s32	 x = position->x;
	s32	 y;
	s32	 flag = 0;
	u32	 tile_x;
	u32	 index;
	u32 *tile;

	if (x < 0)
		x += 0x1fffff;
	tile_x = (x >> 21) & 31;
	y = position->y;
	if (y < 0)
		y += 0x1fffff;

	index = tile_x + (((y >> 21) & 31) << 5);
	tile = gMapBlocks + index;
	if (((u8 *)tile)[3] & 0x80)
		flag = 0x10;

	*terrain_kind = (*tile << 1) >> 25;
	if (*terrain_kind == 21)
		flag = 0x20;

	return WorldMap_TerrainBehaviorTable[flag + selector];
}

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
{
	struct MapScrollWork *work;

	work = gMapWork[0];
	if (value0 >= 0)
	{
		work->shake_x = value0;
	}
	if (value1 >= 0)
	{
		work->shake_y = value1;
	}
	if (value2 >= 0)
	{
		work->shake_decay = value2;
	}
}

void Map_WaitWorkValuesBelow256(void)
{
	struct MapScrollWork *work;
	s32					  cnt;

	work = gMapWork[0];
	cnt = 0;
	if (work->shake_x > 255 || work->shake_y > 255)
	{
	wait:
		WaitFrames(1);
		if (++cnt < 300 && (work->shake_x > 255 || work->shake_y > 255))
			goto wait;
	}
	work->shake_decay = 0;
}

void Resource_RunCopiedDecoder(s32 a, s32 b)
{
	u8 *base;

	base = gDecodeBuffer;
	RamCode_LoadSlot(49, TileMap_DrawRows, TileMap_DrawRowsCodeSize);
	/* FAKEMATCH: one-pass block: its loop notes keep gBgTileBuffer's address load ahead of a's move into r0, as the bytes order the call's arguments. Plain forms measured 2026-10-13: the bare call differ 2 (that load one move later); the tile address in a local differ 2; &gBgTileBuffer[0x4000] and &base[0x1000] through WORK_SLOT differ 3; the routine in a local pointer differ 8; base += 0x1000 after the load call differ 10; gDecodeBuffer + 0x1000 at the declaration differ 26. */
	do
	{
		((void (*)(s32, s32, void *, void *))gWorkSlot[49])(
			a, b, gBgTileBuffer + 0x4000, base + 0x1000);
	} while (0);
	Runtime_ReleaseHeapBlock(49);
}

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row)
{
	s32	 ground[3];
	s32	 viewed[3];
	s32	 horizon;
	s32	 focus;
	s32 *view;
	s32	 line;
	s32	 distance;
	s32	 difference;
	s32	 scale;
	s32	 x;
	s32	 y;
	s32	 diagonal;
	s32	 vertical;
	s32	 zero;
	s32	 horizontal;

	zero = 0;
	ground[0] = position[0];
	ground[1] = zero;
	ground[2] = position[2];
	Iwram_TransformVector(ground, viewed);
	view = viewed;
	horizon = view[1] - Iwram_MulQ16(view[2], depth);
	focus = -gProjection.focal;
	for (line = 0; line < 160; line++)
	{
		distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
		difference = distance - depth;
		if (difference == 0)
			difference = 1;
		scale = Iwram_RatioMulQ14(difference, horizon);
		if (scale < 0)
		{
			diagonal = Iwram_MulQ16(-scale, 0x8000);
			row->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
			diagonal = Iwram_MulQ16(scale, distance);
			horizontal = (view[2] - scale) >> 4;
			vertical = (diagonal - view[1]) >> 4;
			x = Iwram_MulQ16(horizontal, horizontal);
			y = Iwram_MulQ16(vertical, vertical);
			y = Iwram_Sqrt(x + y) << 12;
			if (vertical < 0)
				y = -y;
			row->depth = Iwram_MulQ16(y, 0x8000);
		}
		else
		{
			row->scale = 0;
			row->depth = 0;
		}
		row->offset_x = 0;
		row->offset_y = 0;
		row++;
	}
}

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
	/* FAKEMATCH: volatile, as gKeysHeld and gKeysRepeat are declared (keystate.h): each *held and *repeat is a load of its own. A plain held differs 245, 663 instructions for 672; a plain repeat 89, 669 for 672. Using gKeysHeld and gKeysRepeat directly differs 283: the native keeps only these two addresses in fp and r9 and reloads every other key address from the pool. */
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

	Dma_Fill32Cell(zero, 0, work, 4);
	Dma_Fill32Cell(zero, 0, ent, sizeof(ent));

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
		/* FAKEMATCH: needed to match: the native adds the flags offset to the object's own register (r5) in place; a separate address takes a copy. Measured 2026-10-13: ((struct AnimationObject *)alt)->flags = flag, the same through a pointer local, or a byte offset from alt all differ 55; a separate object variable 128 (the index and the object share r5). */
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
			/* FAKEMATCH: the upper bound in a local compares as cmp against 0x200 with blt; the literal compares against 0x1ff from the pool. 2026-10-13: start >= 0x200 or 0x200 <= start differs 9 of 25 (ldr r3, =0x1ff and ble); the tests as one else-if chain 9, start < 0x200 guarding the lookup 13. */
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
