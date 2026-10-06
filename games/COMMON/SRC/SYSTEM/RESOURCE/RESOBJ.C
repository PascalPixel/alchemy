#include "EDITION.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "IO_REG.H"
#include "ANIMSPR.H"

struct ResourceObjectWork {
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    u8 unknown_00[16];
    u8 resource;
    u8 flags;
    u8 unknown_12[22];
#else
    u8 unknown_00[28];
    u8 resource;
    u8 flags;
    u8 unknown_1e[10];
#endif
    void *children[4];
};

typedef char ResourceObjectWork_size[
    sizeof(struct ResourceObjectWork) == 56 ? 1 : -1
];

void ResourceMetadata_ClearRecord(void *destination);

#if !GAME_TLA
/* The Lost Age keeps its own version, 0x08022d40, as a listing for now. */
#include "METADATA_LOOKUP.H"
#include "VRAM_TAB.H"

extern struct AnimationObject *gSpriteObjects;

s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id);

struct ResourceObject *ResourceObject_Create(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationObject *entry;
    struct AnimationObject *found;
    u32 *word;
    s32 resource;
    s32 tile;
    s32 i;
    u32 shape;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    resource = Resource_FindFreeEntry();
    entry = gSpriteObjects;
    if (metadata->width == 0)
        return NULL;
    for (i = 0; i <= 63; i++, entry++) {
        if (entry->width == 0) {
            found = entry;
            break;
        }
    }
    if (found == NULL)
        return NULL;
    if (resource == 96)
        return NULL;
    tile = VramBlock_LoadCached(resource, 0, 0);
    if (tile == 0)
        return NULL;
    found->slot = resource;
    found->rotation = 0;
    found->flags = 1;
    switch ((u32)((metadata->width << 8) + metadata->height)) {
    case 0x0808: shape = 0; break;
    case 0x0810: shape = 0x8000; break;
    case 0x1008: shape = 0x4000; break;
    case 0x1010: shape = 0x40000000; break;
    case 0x1020: shape = 0x80008000; break;
    case 0x2010: shape = 0x80004000; break;
    case 0x2020: shape = 0x80000000; break;
    case 0x2040: shape = 0xc0008000; break;
    case 0x4020: shape = 0xc0004000; break;
    case 0x4040: shape = 0xc0000000; break;
    default: shape = 0; break;
    }
    word = (u32 *)found->part;
    *word++ = 0;
    *word++ = shape | 0x2000;
    *word++ = tile | 0x800;
    *word++ = 0;
    *word++ = 0x6000;
    *word = (gVramBlockCache[93].offset >> 5) | 0x800;
    if (ResourceMetadata_Register(entry, id) == -1) {
        /* FAKEMATCH: native dead -1 after Register (movs/negs); empty arm
           after a same-byte reload keeps the carrier (differ 32 -> 0). */
        found->dirty = found->dirty;
    }
    return (struct ResourceObject *)found;
}
#endif

void ResourceObject_Release(struct ResourceObjectWork *work)
{
    volatile u32 zero;
    s32 count;
    void **child;

    if (work) {
        if (!(work->flags & 1))
            Resource_ResetEntry(work->resource);
        child = work->children;
        count = 3;
        do {
            ResourceMetadata_ClearRecord(*child++);
        } while (--count >= 0);
        zero = 0;
        Dma_Set(&zero, work, 0x85000000 | (sizeof(*work) / 4), REG_DMA3);
    }
}

#include "METADATA_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"

s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt)
{
    struct AnimationMetadata *info;
    u8 *p;
    u8 op;
    u8 val;
    s32 sum = 0;

    info = Resource_GetMetadataRecordFar(id);
    if (no >= info->animation_count) {
        return 0;
    }
    p = ((u8 **)info->animation)[no];
    for (;;) {
        op = p[0];
        val = p[1];
        p += 2;
        if (op == 254 || op == 241 || op == 253 || op == 239) {
            break;
        }
        if (op == 245 || op == 255 || op <= 238) {
            sum += val;
            cnt--;
            if (cnt == 0) {
                break;
            }
        }
    }
    return sum;
}
