/*
 * compress.c
 *
 * Pixel, map, and halfword compression formats.
 * Each encoder implements its format's literal, copy, and terminator rules.
 * Move-to-front pixel alphabets are shared with the byte LZ encoder in lz.c;
 * sprite-arena copying has its own planner in arena.c.
 */
#include "compress.h"

/*
 * Return the old palette index, then move that value to the front.
 */
unsigned
moveToFront(unsigned char table[16], unsigned value)
{
	unsigned i = 0;
	while (i < 16 && table[i] != value)
		i++;
	if (i == 16)
		fail("pixel exceeds four bits");
	memmove(table + 1, table, i);
	table[0] = value;
	return i;
}

/*
 * Nonzero pixels are literals; bytes above 0xdf encode zero runs and zero
 * itself terminates the stream.
 */
Buffer
compressZeros(const Buffer *p)
{
	Buffer out = {0};
	for (size_t i = 0; i < p->len;)
	{
		if (p->data[i])
		{
			if (p->data[i] > 0xdf)
				fail("zero-skip literal exceeds 0xdf");
			byte(&out, p->data[i++]);
		}
		else
		{
			size_t n = 1;
			while (n < 32 && i + n < p->len && !p->data[i + n])
				n++;
			byte(&out, 0xdf + n);
			i += n;
		}
	}
	byte(&out, 0);
	return out;
}

/*
 * Encode differences modulo 128, using an absolute pixel for the middle tier.
 */
Buffer
compressDelta7(const Buffer *data)
{
	Bits b = {0};
	unsigned previous = 0;
	for (size_t i = 0; i < data->len; i++)
	{
		unsigned p = data->data[i], d = (p - previous) & 127;
		if (p > 127)
			fail("delta7 pixel exceeds seven bits");
		if (!d)
			putBits(&b, 0, 2);
		else if (d <= 2)
		{
			putBits(&b, 1, 3);
			putBits(&b, d - 1, 1);
		}
		else if (d <= 10)
		{
			putBits(&b, 3, 3);
			putBits(&b, d - 3, 3);
		}
		else if (d <= 26)
		{
			putBits(&b, 2, 4);
			putBits(&b, d - 11, 4);
		}
		else if (d <= 101)
		{
			putBits(&b, 6, 3);
			putBits(&b, p, 7);
		}
		else if (d <= 117)
		{
			putBits(&b, 10, 4);
			putBits(&b, 117 - d, 4);
		}
		else if (d <= 125)
		{
			putBits(&b, 7, 3);
			putBits(&b, 125 - d, 3);
		}
		else
		{
			putBits(&b, 5, 3);
			putBits(&b, 127 - d, 1);
		}
		previous = p;
	}
	putBits(&b, 0, 1);
	alignBits(&b, 16, 0);
	return b.bytes;
}

/*
 * Mode one separates high and low bytes after taking halfword differences.
 */
Buffer
deltaMap(const Buffer *data, unsigned mode)
{
	if (mode > 2 || !data->len || data->len % 2)
		fail("delta map needs mode 0..2 and whole halfwords");
	Buffer transformed = {0}, out = {0};
	unsigned previous = 0;
	byte(&out, mode);
	for (size_t i = 0; i < data->len; i += 2)
	{
		unsigned v = read16(data->data + i);
		half(&transformed, mode ? v ^ previous : v);
		previous = v;
	}
	if (mode == 1)
	{
		for (size_t i = 1; i < transformed.len; i += 2)
			byte(&out, transformed.data[i]);
		for (size_t i = 0; i < transformed.len; i += 2)
			byte(&out, transformed.data[i]);
	}
	else
		append(&out, transformed.data, transformed.len);
	free(transformed.data);
	return out;
}

/*
 * The seventeenth code terminates a sixteen-entry move-to-front alphabet.
 */
Buffer
compressIcon(const Buffer *data)
{
	static const char *codes[] = {
		"0",		  "100",	   "1010",	   "1011",		 "1100",
		"1101",		  "1110",	   "111100",   "111101",	 "111110",
		"11111100",	  "11111101",  "11111110", "1111111100", "1111111101",
		"1111111110", "1111111111"};
	Bits b = {0};
	uint8_t table[16];
	for (unsigned i = 0; i < 16; i++)
		table[i] = i;
	for (size_t i = 0; i <= data->len; i++)
	{
		unsigned index =
			i == data->len ? 16 : moveToFront(table, data->data[i]);
		for (const char *s = codes[index]; *s; s++)
			putBits(&b, *s - '0', 1);
	}
	alignBits(&b, 8, 1);
	return b.bytes;
}

/*
 * Copies count halfwords, not bytes. Equal-length matches keep the nearest
 * source, and a zero copy token terminates the stream.
 */
Buffer
compressHalfwords(const Buffer *data)
{
	if (data->len % 2)
		fail("halfword LZ needs whole halfwords");
	Buffer out = {0};
	size_t at = 0, flags = 0;
	unsigned group = 0;
	for (;;)
	{
		if (!group)
		{
			flags = out.len;
			half(&out, 0);
		}
		size_t length = 0, distance = 0;
		for (size_t back = 1; back <= at && back <= 2047; back++)
		{
			size_t n = 0;
			while (n < 33 && at + n < data->len / 2 &&
				   read16(data->data + 2 * (at + n)) ==
					   read16(data->data + 2 * (at + n - back)))
				n++;
			if (n > length)
			{
				length = n;
				distance = back;
			}
		}
		if (at == data->len / 2 || length >= 2)
		{
			write16(out.data + flags,
					read16(out.data + flags) | (0x8000 >> group));
			half(&out,
				 at == data->len / 2 ? 0 : (distance << 5) | (length - 2));
			if (at == data->len / 2)
				break;
			at += length;
		}
		else
		{
			half(&out, read16(data->data + 2 * at));
			at++;
		}
		group = (group + 1) % 16;
	}
	return out;
}
