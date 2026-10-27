/*
 * shared.c
 *
 * Diagnostics, allocation, and byte/bit buffers for the host tools.
 * Growing buffers own their storage and keep a spare trailing NUL. Binary
 * writes use little-endian order; bit streams consume the low bits first.
 * File operations and string-list parsing live in their own modules.
 */
#include "shared.h"
#include <stdarg.h>

static const char *program = "ags";

void
startTool(int argc, char **argv, const char *usage)
{
	const char *slash = strrchr(argv[0], '/');
	program = slash ? slash + 1 : argv[0];
	if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
	{
		puts(usage);
		exit(0);
	}
}

_Noreturn void
fail(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "%s: ", program);
	vfprintf(stderr, format, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

void *
resize(void *data, size_t count, size_t size)
{
	if (size && count > SIZE_MAX / size)
		fail("allocation is too large");
	void *result = realloc(data, count * size ? count * size : 1);
	if (!result)
		fail("out of memory");
	return result;
}

void *
allocate(size_t count, size_t size)
{
	void *result = resize(NULL, count, size);
	memset(result, 0, count * size);
	return result;
}

static char *
vformat(const char *format, va_list args)
{
	va_list copy;
	va_copy(copy, args);
	int size = vsnprintf(NULL, 0, format, copy);
	va_end(copy);
	if (size < 0)
		fail("formatting failed");
	char *text = allocate((size_t) size + 1, 1);
	vsnprintf(text, (size_t) size + 1, format, args);
	return text;
}

char *
formatString(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	char *text = vformat(format, args);
	va_end(args);
	return text;
}

/*
 * Reserve payload plus a terminator even for an empty buffer. Growing a buffer
 * must never wrap the allocation size.
 */
void
reserve(Buffer *buffer, size_t extra)
{
	if (extra > SIZE_MAX - buffer->len - 1)
		fail("buffer is too large");
	size_t need = buffer->len + extra + 1;
	if (need > buffer->cap)
	{
		size_t cap = need < 256 ? 256 : need;
		if (buffer->cap <= SIZE_MAX / 2 && cap < buffer->cap * 2)
			cap = buffer->cap * 2;
		buffer->data = resize(buffer->data, cap, 1);
		buffer->cap = cap;
	}
	buffer->data[buffer->len] = 0;
}

/*
 * The source must not alias storage that growing this buffer can relocate.
 */
void
append(Buffer *buffer, const void *data, size_t size)
{
	reserve(buffer, size);
	if (size)
		memcpy(buffer->data + buffer->len, data, size);
	buffer->len += size;
	buffer->data[buffer->len] = 0;
}

void
byte(Buffer *buffer, unsigned value)
{
	uint8_t encoded = value;
	append(buffer, &encoded, 1);
}

void
half(Buffer *buffer, unsigned value)
{
	byte(buffer, value);
	byte(buffer, value >> 8);
}

void
word(Buffer *buffer, uint32_t value)
{
	half(buffer, value);
	half(buffer, value >> 16);
}

void
print(Buffer *buffer, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	char *text = vformat(format, args);
	va_end(args);
	append(buffer, text, strlen(text));
	free(text);
}

void
alignBuffer(Buffer *buffer, size_t alignment)
{
	if (!alignment)
		fail("zero alignment");
	while (buffer->len % alignment)
		byte(buffer, 0);
}

uint16_t
read16(const uint8_t *data)
{
	return data[0] | (unsigned) data[1] << 8;
}

uint32_t
read32(const uint8_t *data)
{
	return read16(data) | (uint32_t) read16(data + 2) << 16;
}

void
write16(uint8_t *data, unsigned value)
{
	data[0] = value;
	data[1] = value >> 8;
}

void
write32(uint8_t *data, uint32_t value)
{
	write16(data, value);
	write16(data + 2, value >> 16);
}

/*
 * Bits are packed least-significant first; a partial last byte stays zero-filled.
 */
void
putBits(Bits *bits, uint32_t value, unsigned count)
{
	for (unsigned i = 0; i < count; i++)
	{
		if (!(bits->count % 8))
			byte(&bits->bytes, 0);
		bits->bytes.data[bits->count / 8] |= ((value >> i) & 1)
											 << (bits->count % 8);
		bits->count++;
	}
}

void
alignBits(Bits *bits, unsigned alignment, unsigned fill)
{
	while (bits->count % alignment)
		putBits(bits, fill, 1);
}

unsigned
bitLength(size_t value)
{
	unsigned length = 0;
	while (value)
	{
		length++;
		value >>= 1;
	}
	return length;
}
