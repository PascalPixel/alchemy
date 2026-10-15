/* Draft, differ 6 of 132: The Broken Seal's UiText_DecodeSymbol as it is
 * (the old draft here measured differ 142). Left: the native loads the bit
 * count (ldr r0, [r0, #8]) before the leaves' address is formed, and the
 * second leaf byte before it masks the first; this schedules each load one
 * slot later, as in the lane's other one-row near misses. */
#include "types.h"
#include "text_reader.h"

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
	u32		  code;
	u32		  hi;
	const u8 *base;
	const u8 *tree;
	const u8 *leaves;
	const u8 *text;
	s32		  textBits;
	s32		  treeBits;
	s32		  rank;
	s32		  depth;
	s32		  bit;
	u32		  index;
	u32		  half;

	code = reader->previousCode;
	text = (const u8 *)reader->words;
	textBits = reader->bits;
	treeBits = 1;
	hi = code >> 8;
	code &= 0xff;
	base = Text_MessageContexts[hi][0];
	base += ((const u16 *)Text_MessageContexts[hi][1])[code];
	tree = base;
	leaves = base - 1;
	rank = 0;
	for (;;)
	{
		bit = treeBits & 1;
		treeBits >>= 1;
		if (bit != 0 && treeBits == 0)
		{
			treeBits = *tree++;
			bit = treeBits & 1;
			treeBits = (treeBits >> 1) | 0x80;
		}
		if (bit != 0)
			break;
		bit = textBits & 1;
		textBits >>= 1;
		if (bit != 0 && textBits == 0)
		{
			textBits = *text++;
			bit = textBits & 1;
			textBits = (textBits >> 1) | 0x80;
		}
		if (bit == 0)
			continue;
		depth = 0;
		do
		{
			bit = treeBits & 1;
			treeBits >>= 1;
			if (bit != 0 && treeBits == 0)
			{
				treeBits = *tree++;
				bit = treeBits & 1;
				treeBits = (treeBits >> 1) | 0x80;
			}
			if (bit == 0)
				depth++;
			else
			{
				rank++;
				depth--;
			}
		} while (depth >= 0);
	}
	index = rank * 3;
	if (((index * 4) & 7) == 0)
	{
		bit = leaves[-(index >> 1)];
		code = bit << 4;
		bit = leaves[-(index >> 1) - 1];
		code |= (u32)bit >> 4;
	}
	else
	{
		half = leaves[-(index >> 1)];
		half &= 0xf;
		code = leaves[-(index >> 1) - 1] | (half << 8);
	}
	reader->bits = textBits;
	reader->previousCode = code;
	reader->words = (const u32 *)text;
	return code;
}

