#include "DMA.H"
#include "TYPES.H"
#include "VRAM_BLOCK.H"

void ResourceMetadata_ClearRecord(void *destination)
{
    if (destination != 0) {
        volatile u32 clear_value;

        clear_value = 0;
        Dma_Set(&clear_value, destination, 0x85000006, (volatile u32 *)0x040000d4);
    }
}

/* system/resource/create_object.c */

struct ObjectMetadata {
    u8 width;
    u8 height;
};

struct ResourceObject {
    u32 oam[6];
    s32 scale;
    u8 resource;
    u8 flags;
    u16 frame;
    u8 active;
    u8 unknown_21[5];
    u8 visible;
    u8 unknown_27[17];
};

extern struct ResourceObject *gSpriteObjects[];

struct ObjectMetadata *Resource_GetMetadataRecordFar(s32 id);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
s32 ResourceMetadata_Register(struct ResourceObject *object, s32 id);

/* Take the first free object of the 64, give it a resource entry and its
   tiles, and fill its OAM words for the metadata's width and height. */
struct ResourceObject *ResourceObject_Create(s32 id)
{
    struct ObjectMetadata *metadata;
    struct ResourceObject *entry;
    struct ResourceObject *found;
    u32 *word;
    s32 resource;
    s32 tile;
    s32 i;
    u32 shape;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    resource = Resource_FindFreeEntry();
    entry = gSpriteObjects[0];
    if (metadata->width == 0)
        return NULL;
    for (i = 0; i <= 63; i++, entry++) {
        if (entry->active == 0) {
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
    found->resource = resource;
    found->frame = 0;
    found->visible = 1;
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
    word = found->oam;
    *word++ = 0;
    *word++ = shape | 0x2000;
    *word++ = tile | 0x800;
    *word++ = 0;
    *word++ = 0x6000;
    *word = (gVramBlockCache[93].offset >> 5) | 0x800;
    /* FAKEMATCH: the reference compares the registration result with -1
       and keeps the dead -1 after the branch is gone; a failure branch whose
       only statement reload deletes as a no-op reproduces that. */
    if (ResourceMetadata_Register(entry, id) == -1)
        found->active = found->active;
    return found;
}

struct Work { u8 unknown[28]; u8 resource, flags; u8 unknown_1e[10]; void *children[4]; };
void Resource_ResetEntry(s32);
void ResourceMetadata_ClearRecord(void *);
void ResourceObject_Release(struct Work *work)
{
    volatile u32 zero;
    s32 count;
    void **child;
    if (work) {
        if (!(work->flags & 1)) Resource_ResetEntry(work->resource);
        child = work->children;
        count = 3;
        do { ResourceMetadata_ClearRecord(*child++); } while (--count >= 0);
        zero = 0;
        Dma_Set(&zero, work, 0x8500000e, (volatile u32 *)0x040000d4);
    }
}
