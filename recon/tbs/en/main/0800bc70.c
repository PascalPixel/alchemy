/* CANONICAL DRAFT: ResourceObject_Create; ordinary API/dead-result repair.
 * 2026-10-02: one approved ordinary form removes the no-op failure branch
 * used only to retain the native discarded -1 instruction. No new devices.
 * Native complete356 bytes; saved draft complete352 bytes and59 differing
 * bytes in all six TBS editions. Full calls and symbolic pools retain their
 * actual native owners. The old whole module468/native472 differs130 bytes;
 * its trailing release difference2 was solely the shifted BL displacement.
 * The true MetadataSlotState two-argument API is expressed by an opaque
 * declaration and a data-pointer cast; this does not model its private view.
 * This attempt is uncredited. The leading ClearRecord40 remains C, Creator
 * stays raw356, and the complete shared Release+metadata owner is separate.
 */
#include "TYPES.H"
#include "VRAM_BLOCK.H"
#include "RESOURCE.H"

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
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
struct MetadataSlotState;
s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id);

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
    ResourceMetadata_Register((struct MetadataSlotState *)entry, id);
    return found;
}
