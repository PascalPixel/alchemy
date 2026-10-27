/*
 * lz.c
 *
 * Byte LZ matching and the general, palette, and move-to-front streams.
 * Pair chains preserve the nearest-match tie break. The packing input chooses
 * tokens; the shipped input supplies literals and validates copy reconstruction.
 * This distinction supports documented edits made after compression.
 */
#include "compress.h"

typedef enum
{
	LZ_GENERAL,
	LZ_PALETTE,
	LZ_MOVE_TO_FRONT,
	LZ_PALETTE_UNTAGGED
} LzFormat;

const LzMachine packer = {4123, 485, 4126, 272};
typedef struct
{
	size_t length, distance;
} Token;
typedef struct
{
	const Buffer *data;
	size_t *previous;
} Matcher;

/* A chain per byte pair, newest first, preserves the nearest-match tie break.
 */
static Matcher
matcher(const Buffer *data)
{
	Matcher m = {data, allocate(data->len, sizeof(size_t))};
	size_t *heads = allocate(65536, sizeof(size_t));
	for (size_t i = 0; i < 65536; i++)
		heads[i] = SIZE_MAX;
	for (size_t i = 0; i + 1 < data->len; i++)
	{
		unsigned pair = read16(data->data + i);
		m.previous[i] = heads[pair];
		heads[pair] = i;
	}
	free(heads);
	return m;
}

/*
 * Search newest candidates first and replace only a strictly shorter match.
 * Overlapping copies are legal and retain the nearest-match tie break.
 */
static Token
search(const Matcher *m, size_t at, size_t longest, size_t lowest)
{
	Token best = {1, 0};
	size_t max = m->data->len - at;
	if (max > longest)
		max = longest;
	if (max < 2)
		return best;
	const uint8_t *data = m->data->data;
	for (size_t from = m->previous[at]; from != SIZE_MAX && from >= lowest;
		 from = m->previous[from])
	{
		if (data[from + best.length] != data[at + best.length])
			continue;
		size_t n = 2;
		while (n < max && data[from + n] == data[at + n])
			n++;
		if (n > best.length)
			best = (Token){n, at - from};
		if (best.length == max)
			break;
	}
	return best;
}

/*
 * The packer's read-ahead buffer can move the oldest candidate near EOF.
 */
static size_t
oldest(size_t at, size_t size, size_t window, size_t ahead)
{
	size_t read = ahead > size - at ? size : at + ahead;
	return read > window + ahead ? read - window - ahead : 0;
}

static size_t
lowest(size_t old, size_t at, size_t reach)
{
	return at > reach && at - reach > old ? at - reach : old;
}

/*
 * A zero prefix selects a copy; the remaining bits encode its length tier.
 */
static void
lengthBits(Bits *b, size_t n)
{
	putBits(b, 0, 1);
	if (n <= 5)
	{
		putBits(b, (1u << (n - 2)) - 1, (unsigned) n - 1);
	}
	else if (n <= 7)
	{
		putBits(b, 15, 5);
		putBits(b, (unsigned) n - 6, 1);
	}
	else if (n <= 10)
	{
		putBits(b, 31, 5);
		putBits(b, (unsigned) n - 7, 2);
	}
	else
	{
		putBits(b, 31, 7);
		putBits(b, (unsigned) n - 10, 7);
	}
}

/*
 * Long-copy distance width grows with the amount already decoded.
 */
static void
distanceBits(Bits *b, size_t at, size_t distance)
{
	if (distance <= 32)
	{
		putBits(b, 1, 1);
		putBits(b, (unsigned) distance - 1, 5);
	}
	else
	{
		unsigned width = at >= 33 && at - 33 < 2048 ? bitLength(at - 33) : 12;
		if (distance - 33 >= (1u << width))
			fail("LZ distance exceeds its position-dependent width");
		putBits(b, 0, 1);
		putBits(b, (unsigned) distance - 33, width);
	}
}

/*
 * Lazy matching can defer a copy by one literal. Palette streams reset that
 * deferral each token, while general streams carry it into the next choice.
 */
static Buffer
encode(const Buffer *ship, const Buffer *pack, LzMachine machine, LzFormat kind)
{
	Matcher m = matcher(pack);
	size_t at = 0;
	bool deferred = false;
	bool palette = kind == LZ_PALETTE;
	Bits bits = {0};
	Buffer out = {0};
	unsigned char table[16];
	for (unsigned i = 0; i < 16; i++)
		table[i] = i;
	size_t flagAt = 0;
	unsigned group = 0;
	if (kind != LZ_PALETTE_UNTAGGED)
		byte(&out, kind);
	if (kind == LZ_PALETTE_UNTAGGED)
		palette = true;
	size_t window = palette ? 4092 : machine.window;
	size_t ahead = palette ? machine.paletteAhead : machine.ahead;
	size_t reach = palette ? 4095 : machine.distance;
	while (at < pack->len)
	{
		size_t old = oldest(at, pack->len, window, ahead);
		Token t = search(&m, at, palette ? 272 : 137, lowest(old, at, reach));
		bool lazy = !deferred;
		if (palette)
			deferred = false;
		if (kind != LZ_MOVE_TO_FRONT && lazy && t.length > 1 &&
			at + t.length < pack->len)
		{
			size_t alt = search(&m, at + 1, palette ? 272 : 137,
								lowest(old, at + 1, reach))
							 .length;
			size_t next = search(&m, at + t.length, palette ? 272 : 137,
								 lowest(old, at + t.length, reach))
							  .length;
			if (alt > 2 && alt + 1 >= t.length + next)
			{
				t = (Token){1, 0};
				deferred = true;
			}
		}
		if (t.distance)
		{
			for (size_t i = 0; i < t.length; i++)
				if (ship->data[at + i] != ship->data[at + i - t.distance])
					fail("LZ plan does not reconstruct the shipped bytes");
		}
		if (palette)
		{
			if (!group)
			{
				flagAt = out.len;
				byte(&out, 0);
			}
			if (t.distance)
			{
				out.data[flagAt] |= 1 << (7 - group);
				byte(&out, (t.distance >> 8) << 4 |
							   (t.length <= 16 ? t.length - 1 : 0));
				byte(&out, t.distance);
				if (t.length >= 17)
					byte(&out, t.length - 17);
			}
			else
				byte(&out, ship->data[at]);
			group = (group + 1) % 8;
		}
		else if (t.distance)
		{
			lengthBits(&bits, t.length);
			distanceBits(&bits, at, t.distance);
		}
		else
		{
			putBits(&bits, 1, 1);
			unsigned value = ship->data[at];
			if (kind == LZ_MOVE_TO_FRONT)
			{
				unsigned lo = moveToFront(table, value & 15),
						 hi = moveToFront(table, value >> 4);
				unsigned width = bitLength(lo > hi ? lo : hi);
				if (width < 2)
					width = 2;
				if (width == 2)
					putBits(&bits, 1, 1);
				else
				{
					putBits(&bits, 0, 1);
					putBits(&bits, width == 3, 1);
				}
				putBits(&bits, lo, width);
				putBits(&bits, hi, width);
			}
			else
				putBits(&bits, value, 8);
		}
		at += t.length;
	}
	if (palette)
	{
		if (!group)
		{
			flagAt = out.len;
			byte(&out, 0);
		}
		out.data[flagAt] |= 1 << (7 - group);
		half(&out, 0);
	}
	else
	{
		putBits(&bits, 0, 1);
		putBits(&bits, 31, 7);
		putBits(&bits, 0, 7);
		append(&out, bits.bytes.data, bits.bytes.len);
		free(bits.bytes.data);
	}
	free(m.previous);
	return out;
}

/*
 * Tagged output chooses the shorter encoding; ties select the palette form.
 */
Buffer
compressLz(const Buffer *ship, const Buffer *pack, const char *kind,
		   LzMachine machine)
{
	if (ship->len != pack->len)
		fail("packing inputs differ in length");
	if (!machine.window || !machine.distance || machine.distance > 4128 ||
		machine.ahead > SIZE_MAX - machine.window)
		fail("invalid LZ machine");
	if (!strcmp(kind, "general"))
		return encode(ship, pack, machine, LZ_GENERAL);
	if (!strcmp(kind, "palette"))
		return encode(ship, pack, machine, LZ_PALETTE);
	if (!strcmp(kind, "plz"))
		return encode(ship, pack, machine, LZ_PALETTE_UNTAGGED);
	if (!strcmp(kind, "mtf") || !strcmp(kind, "mtf4"))
		return encode(ship, pack, packer, LZ_MOVE_TO_FRONT);
	if (strcmp(kind, "lz") && strcmp(kind, "tagged"))
		fail("unknown LZ kind: %s", kind);
	Buffer a = encode(ship, pack, machine, LZ_GENERAL),
		   b = encode(ship, pack, machine, LZ_PALETTE);
	if (b.len <= a.len)
	{
		free(a.data);
		return b;
	}
	free(b.data);
	return a;
}
