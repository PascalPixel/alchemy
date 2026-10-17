/*
 * assembly.h
 *
 * Byte payloads with ordered labels and relocations. Each Assembly owns its
 * bytes and symbol names; appendAssembly copies rather than transfers them.
 */
#ifndef AGS_ASSEMBLY_H
#define AGS_ASSEMBLY_H
#include "common.h"
typedef struct
{
	size_t offset;
	char *name;
	bool global;
} Label;
typedef struct
{
	size_t offset;
	char *symbol;
	int64_t addend;
} Pointer;
/* Append labels and pointers in byte order, including when joining blocks. */
typedef struct
{
	Buffer bytes;
	Label *labels;
	size_t labelCount;
	Pointer *pointers;
	size_t pointerCount;
	size_t alignment;
} Assembly;
bool identifier(const char *name);
void labelAt(Assembly *a, const char *name, size_t offset, bool global);
void pointerAt(Assembly *a, const char *name, size_t offset, int64_t addend);
void addPointer(Assembly *a, const char *name, int64_t addend);
void alignAssembly(Assembly *a, size_t alignment, uint8_t fill);
void appendAssembly(Assembly *a, Assembly *other);
void appendAssemblySource(Buffer *text, const Assembly *a);
void freeAssembly(Assembly *a);
#endif
