#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

/// @file
/// Resources and the sprites and VRAM cache entries made from them.

#include "types.h"
#include "edition.h"

/// A battle sprite id's low twelve bits; the two-part sprites' second part
/// is their id + 0x2001.
#define SPRITE_ID_MASK 0xfff
#define SPRITE_SECOND_PART 0x2001

struct ResourceObject;
struct Sprite;

struct ResourceObject *Sprite_Create(s32 resource);
void Sprite_Destroy(struct Sprite *sprite);
/// Creates an animated sprite object for animation id, using one of 64 free
/// objects and a VRAM cache entry.
struct ResourceObject *Sprite_CreateFar(s32 resource);
/// Releases an object.
void Sprite_DestroyFar(struct Sprite *sprite);

void *Resource_GetTableEntry(s32 resourceId);

s32 VramCache_FindFreeEntry(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 no, s32 src);
s32 VramCache_Load(u32 no, u32 size, const void *src);
s32 VramCache_FreeEntry(u32 no);
s32 VramCache_ReleaseVram(u32 no);
s32 VramCache_FreeBlocks(s32 id);
s32 VramCache_AllocateBlocks(u32 id, u32 size);

s32 Decompress_TaggedLz(const void *source, void *destination);

#endif
