#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct DmaTransfer_08021cb8 {
    const void *source;
    void *destination;
    u32 control;
};

struct Mapping_08021cb8 {
    u8 entries[256];
    u32 nextPalette;
};

struct Decoded_08021cb8 {
    u8 values[0x604];
    const void *source;
};

typedef char Assert_08021cb8_mapping_counter[
    OFFSET_OF(struct Mapping_08021cb8, nextPalette) == 0x100 ? 1 : -1];
typedef char Assert_08021cb8_decoded_source[
    OFFSET_OF(struct Decoded_08021cb8, source) == 0x604 ? 1 : -1];
typedef char Assert_08021cb8_decoded_size[
    sizeof(struct Decoded_08021cb8) == 0x608 ? 1 : -1];

void *Func_080048b0(s32 kind, s32 size);
const u8 *Func_08002f40(s32 resource);
u32 Func_080053e8(const void *source, void *destination);
void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);
void Func_08002dd8(s32 kind);

void Func_08021cb8(
    struct Mapping_08021cb8 *mapping,
    s32 resourceIndex,
    s32 destinationIndex)
{
    volatile struct DmaTransfer_08021cb8 *dma =
        (volatile struct DmaTransfer_08021cb8 *)0x040000d4;
    struct Decoded_08021cb8 *decoded;
    const u8 *resource;
    const u8 *source;
    u8 *output;
    s32 index;

    decoded = Func_080048b0(17, 0x608);
    resource = Func_08002f40(0xf1);
    source = resource + ((const u16 *)resource)[resourceIndex];
    decoded->source = source;
    Func_080053e8(source, decoded);

    output = Func_08004938(0x400);
    for (index = 0; index < 0x400; index++) {
        u8 code = decoded->values[index];
        u8 value = mapping->entries[code];

        if (value == 0xff) {
            u32 next = mapping->nextPalette;

            mapping->entries[code] = next;
            if (next <= 63) {
                *(volatile u16 *)(0x05000000 + next * 2) =
                    *(volatile u16 *)(0x05000200 + code * 2);
                mapping->nextPalette = next + 1;
                value = mapping->entries[code];
            } else {
                value = next;
            }
        }

        output[index] = value;
    }

    dma->source = output;
    dma->destination =
        (void *)(0x06004000 + destinationIndex * 0x40);
    dma->control = 0x84000100;

    Func_08002df0(output);
    Func_08002dd8(17);
}
