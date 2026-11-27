// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3): a number written as
// hexadecimal text, which nothing calls.

#include "types.h"

// Unused: writes value as eight hexadecimal digits (HexDigits) and a terminating null.
UNUSED static void scStringWriteU32AsHex(Uint8 *text, Uint32 value)
{
	Sint32 i;

	text  += 8;
	*text  = 0;
	text--;
	for (i = 7; i >= 0; i--)
	{
		*text   = HexDigits[value & 0xf];
		value >>= 4;
		text--;
	}
}
