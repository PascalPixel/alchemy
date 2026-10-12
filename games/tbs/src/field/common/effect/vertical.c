#include "types.h"
#include "fieldobj.h"
#include "field_effect.h"

void Effect_AnimateVerticalPositive(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed + (0x10000 - amplitude) * 5 + 0x80000;
}

void Effect_AnimateVerticalNegative(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = -amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed - (0x10000 - amplitude) * 5 + 0x100000;
}
