/* Draft, differ 34 (33 instructions): collects the ids of the objects in a five-column strip three rows
 * deep. Measured: the native keeps the row sums inside the loop (adds r3, r4; adds r3, #8;
 * cmp; adds r3, #3), which the stepped edge variable gives (as one condition they are hoisted,
 * differ 56), and it counts in r0 with the output in r5, which returning the count gives
 * (differ 48 as void). Left: the native keeps the row in r4 and the object list in r1 and
 * loads x into r3 and z into r2; this has them the other way round. Tried 2026-10-15: the ☀️ twin
 * MakyuriChojo_CollectBandSlots as written, K&R parameters included (52: its y + 8 and y + 11
 * are hoisted out of the loop), the twin's table indexing with the stepped edge (40), and the
 * list pinned to r1 (42). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

u32 Func_02003a28(s32 *out, s32 z)
{
	struct ObjectRuntime **objects = (struct ObjectRuntime **)((u8 *)Ram_HeapSlots->event_work + 20);
	s32					   row = 64 - (z >> 20);
	u32					   i;

	z = 64 - (z >> 20);
	for (i = 0; i <= 63; i++)
	{
		struct ObjectRuntime *object = *objects++;

		if (object != NULL)
		{
			s32 column = object->x >> 20;
			s32 depth = object->z >> 20;

			if ((u32)(column - 4) <= 4)
			{
				s32 edge = z;

				edge += 8;
				if (edge <= depth)
				{
					edge += 3;
					if (depth < edge)
						*out++ = i;
				}
			}
		}
	}
	return i;
}
