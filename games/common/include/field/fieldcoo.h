#ifndef ALCHEMY_FIELDCOO_H
#define ALCHEMY_FIELDCOO_H

#include "types.h"
#include "layout_guard.h"

union FieldCoordinate
{
	s32 fixed;
	struct
	{
		u16 fraction;
		s16 pixel;
	} part;
};

LAYOUT_SIZE_GUARD(FieldCoordinate_Size, union FieldCoordinate, 4);
LAYOUT_OFFSET_GUARD(FieldCoordinate_Pixel, union FieldCoordinate, part.pixel, 2);

#endif
