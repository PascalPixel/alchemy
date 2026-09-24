/* Draft, not exact (2026-09-24): 352 of 356 bytes. Everything matches but
   the reference's dead "movs r2, #1; negs r2, r2" after the final call; no
   source spelling tried so far keeps that dead -1. */

#include "TYPES.H"

struct AnimationMetadata {
    u8 width;
    u8 height;
};

struct SpriteObject {
    u32 words[7];
    u8 slot;
    u8 reserved_1d;
    u16 frame;
    u8 active;
    u8 reserved_21[5];
    u8 visible;
    u8 reserved_27[17];
};

extern struct SpriteObject *Data_03001e60[];
struct DisplayState { u8 unknown_000[374]; u16 value_176; };
extern struct DisplayState Data_03001b10;

struct AnimationMetadata *Func_08185000(s32 id);
s32 Func_08004080(void);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_0800b8ac(struct SpriteObject *object, s32 id);

struct SpriteObject *Func_0800bc70(s32 id)
{
    struct AnimationMetadata *metadata;
    struct SpriteObject *entry;
    struct SpriteObject *found;
    u32 *word;
    s32 slot;
    s32 tile;
    s32 i;
    u32 attributes;

    found = NULL;
    metadata = Func_08185000(id);
    slot = Func_08004080();
    entry = Data_03001e60[0];
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
    if (slot == 96)
        return NULL;
    tile = Func_08003fa4(slot, 0, 0);
    if (tile == 0)
        return NULL;
    found->slot = slot;
    found->frame = 0;
    found->visible = 1;
    switch ((u32)((metadata->width << 8) + metadata->height)) {
    case 0x0808: attributes = 0; break;
    case 0x0810: attributes = 0x8000; break;
    case 0x1008: attributes = 0x4000; break;
    case 0x1010: attributes = 0x40000000; break;
    case 0x1020: attributes = 0x80008000; break;
    case 0x2010: attributes = 0x80004000; break;
    case 0x2020: attributes = 0x80000000; break;
    case 0x2040: attributes = 0xc0008000; break;
    case 0x4020: attributes = 0xc0004000; break;
    case 0x4040: attributes = 0xc0000000; break;
    default: attributes = 0; break;
    }
    word = found->words;
    *word++ = 0;
    *word++ = attributes | 0x2000;
    *word++ = tile | 0x800;
    *word++ = 0;
    *word++ = 0x6000;
    *word = (Data_03001b10.value_176 >> 5) | 0x800;
    Func_0800b8ac(entry, id);
    return found;
}
