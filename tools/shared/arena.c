/*
 * arena.c
 *
 * Compress sprite pictures against their previously encoded arena.
 * Candidate sources are bytes of earlier encoded pictures, not decoded pixels.
 * Plan copies before emitting split literal/code streams, and retain the raw
 * form when compression fails the format's size and savings limits.
 */
#include "compress.h"

/* Find candidates first, then choose copy lengths from the end of the picture. */
static void
planArenaCopies(const Buffer *data, const Buffer *arena, size_t low,
				size_t *lengths, size_t *sources, size_t *steps)
{
	size_t n = data->len;
	for (size_t at = 0; at < n; at++)
	{
		steps[at] = 1;
		size_t limit = n - at;
		if (limit > 273)
			limit = 273;
		size_t available = arena->len > at ? arena->len - at : 0;
		if (limit > available)
			limit = available;
		if (limit < 3)
			continue;
		for (size_t start = low; start + 1 < arena->len; start++)
		{
			if (read16(arena->data + start) != read16(data->data + at))
				continue;
			size_t count = 2;
			while (count < limit && start + count < arena->len &&
				   arena->data[start + count] == data->data[at + count])
				count++;
			if (count > lengths[at])
			{
				lengths[at] = count;
				sources[at] = start;
			}
		}
		if (lengths[at] < 3)
			lengths[at] = 0;
	}
	for (size_t i = n; i; i--)
	{
		size_t at = i - 1, own = lengths[at];
		if (own < 3)
			continue;
		steps[at] = own;
		size_t next = at + 1;
		while (next < at + own && lengths[next] <= own)
			next++;
		while (next < n && next - at < own)
		{
			if (steps[next] > own)
			{
				steps[at] = next - at < 3 ? 1 : next - at;
				break;
			}
			next += steps[next];
		}
	}
}

static void
emitArenaCopies(const Buffer *data, size_t base, const size_t *sources,
				const size_t *steps, Buffer *literals, Buffer *coded)
{
	size_t n = data->len, flagAt = 0;
	unsigned group = 0;
	for (size_t at = 0;;)
	{
		if (!group)
		{
			flagAt = coded->len;
			byte(coded, 0);
		}
		if (at == n)
		{
			half(coded, 0);
			break;
		}
		size_t step = steps[at];
		if (step == 1)
		{
			coded->data[flagAt] |= 1 << group;
			byte(literals, data->data[at]);
		}
		else
		{
			size_t distance = base - sources[at];
			unsigned value = (unsigned) distance |
							 (step <= 17 ? (unsigned) (step - 2) << 12 : 0);
			byte(coded, value >> 8);
			byte(coded, value);
			if (step >= 18)
				byte(coded, step - 18);
		}
		at += step;
		group = (group + 1) % 8;
	}
}

/*
 * The split field points past the literal block. Keep raw data only when its
 * final zero is the sole terminator and compression is not worthwhile.
 */
Buffer
compressArena(const Buffer *data, const Buffer *arena)
{
	size_t n = data->len, base = arena->len + 2;
	size_t low = base > 4095 ? base - 4095 : 0;
	if (low > arena->len)
		low = arena->len;
	size_t *lengths = allocate(n, sizeof(size_t)),
		   *sources = allocate(n, sizeof(size_t)),
		   *steps = allocate(n, sizeof(size_t));
	planArenaCopies(data, arena, low, lengths, sources, steps);
	Buffer literals = {0}, coded = {0};
	emitArenaCopies(data, base, sources, steps, &literals, &coded);
	Buffer out = {0};
	if (literals.len + 2 > 65535)
		fail("arena literal block exceeds its split field");
	size_t compressed = literals.len + 2 + coded.len;
	if (compressed > 1024 || compressed * 10 >= (n ? n - 1 : 0) * 9)
	{
		if (n && (data->data[n - 1] || memchr(data->data, 0, n - 1)))
			fail("raw arena stream must end in its only zero");
		half(&out, 0);
		append(&out, data->data, n);
	}
	else
	{
		half(&out, (unsigned) literals.len + 2);
		append(&out, literals.data, literals.len);
		append(&out, coded.data, coded.len);
	}
	free(lengths);
	free(sources);
	free(steps);
	free(literals.data);
	free(coded.data);
	return out;
}
