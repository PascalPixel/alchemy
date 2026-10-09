#include "edition.h"
#include "types.h"

void Tile_BuildMetatiles(u16 *dst, u16 *src)
{
	u32 row;
	u32 col;
	s32 a;
	s32 b;
	s32 c;
	s32 d;
	s32 e;
	s32 f;
	s32 found;
	s32 v;

	for (row = 0; row < 20;)
	{
		d = 0;
		a = d;
		c = d;
		f = d;
		b = f;
		e = f;
		found = f;
		col = f;
		dst++;
		for (; col < 30; col++)
		{
			u16 cmd = *src++;

			if (cmd == 0)
				continue;
			if (found == 0)
			{
				switch (cmd)
				{
				case 0xf010:
					a = col * 8;
					b = a + 8;
					c = a + 1;
					d = a;
					e = d + 7;
					f = b;
					found++;
					break;
				case 0xf013:
					a = col * 8;
					b = a + 8;
					c = a;
					d = a + 1;
					e = b;
					f = a + 7;
					found++;
					break;
				case 0xf007:
				case 0xf016:
#if EDITION_INTERNATIONAL || GAME_TLA
				case 0xf01a:
#endif
					a = col * 8;
					b = a + 8;
					d = a;
					c = d;
					f = b;
					e = f;
					found++;
				}
			}
			else
			{
				switch (cmd)
				{
				case 0xf012:
					b = col * 8 + 8;
					e = col * 8 + 7;
					f = b;
					break;
				case 0xf015:
					b = col * 8 + 8;
					e = b;
					f = col * 8 + 7;
					break;
				case 0xf007:
				case 0xf017:
#if EDITION_INTERNATIONAL || GAME_TLA
				case 0xf01b:
#endif
					f = col * 8 + 8;
					b = f;
					e = f;
					break;
				}
			}
		}
		v = e + (c << 8);
		*dst++ = v;
		v = b + (a << 8);
		dst++;
		*dst++ = v;
		dst++;
		*dst++ = v;
		dst++;
		*dst++ = v;
		dst++;
		src += 2;
		/* FAKEMATCH: this one-pass block and early row increment keep the
		 * source advance and row copy between their native stores
		 * (pure C differ 6 -> 2). */
		do
		{
			*dst++ = v;
			row++;
		} while (0);
		/* FAKEMATCH: the one-pass break lets CSE reuse row < 20 before
		 * scheduling, keeping its comparison before the seventh store
		 * (differ 2 -> 0). */
		do
		{
			dst++;
			*dst++ = v;
			if (row < 20)
				break;
		} while (0);
		dst++;
		*dst++ = v;
		v = f + (d << 8);
		dst++;
		*dst++ = v;
	}
}
