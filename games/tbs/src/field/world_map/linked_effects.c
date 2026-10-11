#include "resource.h"
#include "types.h"
#include "field_effect.h"
#include "vram_block.h"
#include "b5_context.h"

struct PairDetail
{
	u8 unknown_00[22];
	u8 field_16;
};

struct PairSprite
{
	struct FieldSprite sprite;
	struct PairDetail *detail;
};

union PairObject
{
	union FieldObject object;
	s32				  words[28];
	struct
	{
		u8				  unknown_00[0x68];
		union PairObject *parent;
	} link;
};

struct PairWork
{
	u8	unknown_00[70];
	u16 vram_block;
};

s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);

struct WorldMapOam
{
	u8	unknown_00[4];
	u16 attr0;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
};

void WorldMap_CreateLinkedEffects(void *object)
{
	union PairObject   *parent = object;
	union PairObject   *pair[2];
	union PairObject   *child;
	struct PairSprite  *part;
	struct FieldSprite *sprite;
	struct PairWork	   *work = (struct PairWork *)gEffectWork;
	s32					i;

	for (i = 0; i < 2; ++i)
	{
		child = (union PairObject *)Engine_ObjectCreate(26, parent->object.actor.x.fixed,
			parent->object.actor.y.fixed, parent->object.actor.z.fixed);
		pair[i] = child;
		if (child != NULL)
		{
			child->words[5] = parent->words[5];
			part = (struct PairSprite *)child->object.actor.sprite;
			child->object.actor.motion_flags = 0;
			child->object.effect.spin = 0;
			child->link.parent = parent;
			if (part != NULL)
			{
				sprite = &part->sprite;
				AnimationObjects_SelectAnimation(sprite, 0);
				sprite->flags = 0;
				Resource_ResetEntry(sprite->vram_block);
				sprite->vram_block = work->vram_block;
				sprite->unknown_1d_0 = 1;
				sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
				sprite->full_color = 0;
				sprite->shape = 1;
				((struct WorldMapOam *)sprite)->size = 2;
				part->detail->field_16 = 0;
			}
		}
	}
	{
		union PairObject   *p = pair[0];
		struct FieldSprite *sp = p->object.actor.sprite;

		p->object.actor.update = (void *)Effect_AnimateVerticalNegative;
		sp->priority = 1;
	}
	{
		struct FieldActor  *p = &pair[1]->object.actor;
		struct FieldSprite *sp = p->sprite;

		sp->priority = 1;
		p->update = (void *)Effect_AnimateVerticalPositive;
		p->priority_flags = 2;
	}
}
