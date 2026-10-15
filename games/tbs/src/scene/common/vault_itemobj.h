#include "types.h"
#include "field_obj.h"
#include "field_event.h"

void OverlayObject_ConfigureObject22WithResource17(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
	u8 *o;
	u8 *q;
	u8 *p;
	u8 *v;
	s32 z;
	s32 m;

	z = 0;
	o = (u8 *)Engine_ObjectCreate(22, fixed_x, fixed_y, fixed_z);
	if (o != 0)
	{
		q = (u8 *)((struct FieldActor *)o)->sprite;
		p = q + 38;
		*p = z;
		p += 1;
		*p = z;
		m = 33;
		m = -m;
		q[5] &= m;
		q[9] &= 15;
		o[0x55] = z;
		o[0x5c] = 1;
		v = Engine_HeapAllocate(17, 0x608);
		Engine_ItemLoadIcon(a);
		v += 0x400;
		Engine_VramLoad(q[28], 0x80, v);
		Engine_HeapRelease(17);
	}
}
