/*
 * assembly.c
 *
 * Build assembly from byte payloads, ordered labels, and relocations.
 * Pointers reserve four zero bytes in the payload and are emitted as symbolic
 * expressions. Appending an assembly rebases both labels and pointers; rendering
 * walks the ordered records once and appends source to the caller's buffer.
 */
#include "assembly.h"
#include <ctype.h>
#include <inttypes.h>
bool
identifier(const char *s)
{
	if (*s != '_' && !isalpha((unsigned char) *s))
		return false;
	for (s++; *s; s++)
		if (*s != '_' && !isalnum((unsigned char) *s))
			return false;
	return true;
}

/*
 * Several labels may share an offset, but names are unique and offsets must
 * arrive in byte order.
 */
void
labelAt(Assembly *assembly, const char *name, size_t at, bool global)
{
	if (!identifier(name))
		fail("invalid assembly symbol: %s", name);
	if (assembly->labelCount &&
		at < assembly->labels[assembly->labelCount - 1].offset)
		fail("assembly labels must follow byte order");
	for (size_t i = 0; i < assembly->labelCount; i++)
		if (!strcmp(assembly->labels[i].name, name))
			fail("duplicate label: %s", name);
	assembly->labels =
		resize(assembly->labels, assembly->labelCount + 1, sizeof(Label));
	assembly->labels[assembly->labelCount++] =
		(Label){at, copyString(name), global};
}

/*
 * Relocations occupy four bytes and may not overlap an earlier relocation.
 */
void
pointerAt(Assembly *assembly, const char *name, size_t at, int64_t addend)
{
	if (!identifier(name))
		fail("invalid pointer symbol: %s", name);
	if (assembly->pointerCount &&
		at < assembly->pointers[assembly->pointerCount - 1].offset + 4)
		fail("assembly pointers overlap or are out of order");
	assembly->pointers =
		resize(assembly->pointers, assembly->pointerCount + 1, sizeof(Pointer));
	assembly->pointers[assembly->pointerCount++] =
		(Pointer){at, copyString(name), addend};
}

void
addPointer(Assembly *assembly, const char *name, int64_t addend)
{
	pointerAt(assembly, name, assembly->bytes.len, addend);
	word(&assembly->bytes, 0);
}

void
alignAssembly(Assembly *assembly, size_t n, uint8_t fill)
{
	if (!n || (n & (n - 1)))
		fail("alignment is not a power of two");
	if (assembly->alignment < n)
		assembly->alignment = n;
	while (assembly->bytes.len % n)
		byte(&assembly->bytes, fill);
}

/*
 * Copy a completed fragment at the current offset. Its required alignment
 * must already have been satisfied by the caller.
 */
void
appendAssembly(Assembly *assembly, Assembly *other)
{
	size_t at = assembly->bytes.len;
	if (other->alignment && at % other->alignment)
		fail("appended assembly is unaligned");
	if (assembly->alignment < other->alignment)
		assembly->alignment = other->alignment;
	append(&assembly->bytes, other->bytes.data, other->bytes.len);
	for (size_t i = 0; i < other->labelCount; i++)
		labelAt(assembly, other->labels[i].name, at + other->labels[i].offset,
				other->labels[i].global);
	for (size_t i = 0; i < other->pointerCount; i++)
		pointerAt(assembly, other->pointers[i].symbol,
				  at + other->pointers[i].offset, other->pointers[i].addend);
}

/*
 * Append source at a line boundary. Flush byte rows before labels and pointers
 * so a relocation cannot accidentally become part of a byte directive.
 */
void
appendAssemblySource(Buffer *text, const Assembly *assembly)
{
	if (assembly->alignment > 1)
		print(text, "\t.balign %zu\n", assembly->alignment);
	/* Assemblies are built in byte order. Walk each label and pointer once,
   * even when a sprite bank contains several megabytes of pixel data. */
	size_t row = 0, labelIndex = 0, pointerIndex = 0;
	for (size_t at = 0; at <= assembly->bytes.len;)
	{
		while (labelIndex < assembly->labelCount &&
			   assembly->labels[labelIndex].offset == at)
		{
			const Label *label = assembly->labels + labelIndex++;
			if (row)
			{
				byte(text, '\n');
				row = 0;
			}
			if (label->global)
				print(text, "\t.global %s\n", label->name);
			print(text, "%s:\n", label->name);
		}
		if (at == assembly->bytes.len)
			break;
		const Pointer *relocation =
			pointerIndex < assembly->pointerCount &&
					assembly->pointers[pointerIndex].offset == at
				? assembly->pointers + pointerIndex++
				: NULL;
		if (relocation)
		{
			if (at + 4 > assembly->bytes.len ||
				read32(assembly->bytes.data + at))
				fail("invalid assembly pointer at %zu", at);
			if (row)
			{
				byte(text, '\n');
				row = 0;
			}
			print(text, "\t.4byte %s", relocation->symbol);
			if (relocation->addend > 0)
				print(text, " + %" PRId64, relocation->addend);
			else if (relocation->addend < 0)
				print(text, " - %" PRId64, -relocation->addend);
			byte(text, '\n');
			at += 4;
			continue;
		}
		static const char hex[] = "0123456789abcdef";
		uint8_t value = assembly->bytes.data[at++];
		const char digits[] = {'0', 'x', hex[value >> 4], hex[value & 15]};
		append(text, row ? ", " : "\t.byte ", row ? 2 : 7);
		append(text, digits, sizeof(digits));
		if (++row == 16)
		{
			byte(text, '\n');
			row = 0;
		}
	}
	if (labelIndex != assembly->labelCount ||
		pointerIndex != assembly->pointerCount)
		fail("assembly label or pointer lies outside its data");
	if (row)
		byte(text, '\n');
}

void
freeAssembly(Assembly *assembly)
{
	free(assembly->bytes.data);
	for (size_t i = 0; i < assembly->labelCount; i++)
		free(assembly->labels[i].name);
	for (size_t i = 0; i < assembly->pointerCount; i++)
		free(assembly->pointers[i].symbol);
	free(assembly->labels);
	free(assembly->pointers);
	*assembly = (Assembly){0};
}
