#include "types.h"

struct AssetLookup_0800b6b8 {
    u16 source;
    s16 id;
};

void *Func_08002f40(s32);
s32 Func_08005340(const void *, void *);
const u8 *Func_08185000(s16);

/*
 * Load one indexed map asset, relocate its leading offset table in place, and
 * optionally remap its trailing character data through one of five language
 * tables. Return the asset's width-times-height metadata.
 */
s32 Func_0800b6b8(
    u32 layer,
    u8 *destination,
    s32 asset_id,
    s32 character_map
) {
    const struct AssetLookup_0800b6b8 *lookup;
    const u8 *dimensions;
    const u8 *mapping;
    u8 *layer_state;
    u8 *text;
    s32 decompressed_size;
    s32 relocation_count;
    s32 relative_offset;
    u32 lookup_index;
    u32 map_index;

    if (layer > 7) {
        return 0;
    }

    layer_state = *(u8 **)0x03001E68 + layer * 8;
    dimensions = Func_08185000((s16)asset_id);
    *(s32 *)(layer_state + 0x1C) = (layer << 12) | asset_id;
    *(u8 **)(layer_state + 0x20) = destination;

    lookup = (const struct AssetLookup_0800b6b8 *)0x08012FA0;
    lookup_index = 0;
    while (lookup[lookup_index].id != 0 &&
           lookup[lookup_index].id != asset_id &&
           lookup_index < 0xFF) {
        lookup_index++;
    }

    decompressed_size =
        Func_08005340(Func_08002f40(lookup[lookup_index].source), destination);

    relocation_count = 0;
    relative_offset = *(s32 *)destination;
    while (relative_offset != 0) {
        ((s32 *)destination)[relocation_count] =
            (s32)(destination + relative_offset);
        relocation_count++;
        if (relocation_count > 0xFF) {
            break;
        }
        relative_offset = ((s32 *)destination)[relocation_count];
    }

    if (character_map != 0) {
        map_index = character_map - 1;
        if (map_index > 4) {
            map_index = 0;
        }
        mapping = (const u8 *)0x080092B8 + map_index * 0x100;
        text = (u8 *)&((s32 *)destination)[relocation_count + 1];
        while (text < destination + decompressed_size) {
            if (*text <= 0xDF) {
                *text = mapping[*text];
            }
            text++;
        }
    }

    return dimensions[0] * dimensions[1];
}
