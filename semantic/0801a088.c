#include "layout_guard.h"
#include "types.h"

struct EffectDefinition_0801a088 {
    u8 padding00[3];
    u8 modifier_flags;
    u8 padding04[2];
    u16 transfer_index;
};

struct EffectTransfer_0801a088 {
    u8 padding000[0x600];
    u16 width;
    u16 height;
    const void *source;
};

LAYOUT_OFFSET_GUARD(
    EffectDefinition0801a088_ModifierFlags,
    struct EffectDefinition_0801a088,
    modifier_flags,
    3);
LAYOUT_OFFSET_GUARD(
    EffectDefinition0801a088_TransferIndex,
    struct EffectDefinition_0801a088,
    transfer_index,
    6);
LAYOUT_OFFSET_GUARD(
    EffectTransfer0801a088_Source,
    struct EffectTransfer_0801a088,
    source,
    0x604);

extern struct EffectTransfer_0801a088 *Data_03001e94;

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_0801a5a4(struct EffectTransfer_0801a088 *, s32);
struct EffectDefinition_0801a088 *Func_08077018(s32);

static void SubmitEffectTransfer_0801a088(
    struct EffectTransfer_0801a088 *transfer,
    const void *source,
    s32 alternate)
{
    transfer->source = source;
    transfer->width = 2;
    transfer->height = 2;
    Func_0801a5a4(transfer, alternate);
}

/*
 * Queue the base transfer and requested modifier records for an encoded
 * effect ID.  Bits 0-8 select the definition, bits 9-10 select conditional
 * modifiers, and bits 11-15 encode a one-based quantity whose decimal glyphs
 * may be appended by mode bits 1 and 2.
 */
s32 Func_0801a088(s32 encoded_effect, s32 mode)
{
    const void *const *prefix_transfers =
        (const void *const *)0x08029a10;
    const void *const *base_transfers =
        (const void *const *)0x08029ee4;
    const void *const *modifier_transfers =
        (const void *const *)0x08029acc;
    const void *const *digit_transfers =
        (const void *const *)0x08029b68;
    struct EffectDefinition_0801a088 *definition =
        Func_08077018(encoded_effect & 0x01ff);
    struct EffectTransfer_0801a088 *transfer = Data_03001e94;
    s32 base_alternate = 0;
    s32 quantity = 0;

    if (transfer == 0)
        return -1;

    if ((mode & 1) != 0) {
        SubmitEffectTransfer_0801a088(transfer, prefix_transfers[2], 0);
        base_alternate = 1;
    }

    SubmitEffectTransfer_0801a088(
        transfer,
        base_transfers[definition->transfer_index],
        base_alternate);

    if ((mode & 8) != 0 && (encoded_effect & 0x0400) != 0)
        SubmitEffectTransfer_0801a088(transfer, modifier_transfers[1], 1);
    if ((mode & 0x10) != 0 && (encoded_effect & 0x0200) != 0)
        SubmitEffectTransfer_0801a088(transfer, modifier_transfers[0], 1);
    if ((mode & 0x20) != 0 &&
        (encoded_effect & 0x0200) != 0 &&
        (definition->modifier_flags & 3) == 3) {
        SubmitEffectTransfer_0801a088(transfer, modifier_transfers[2], 1);
    }

    if ((mode & 2) != 0) {
        quantity = ((encoded_effect & 0xf800) >> 11) + 1;
        if (quantity <= 1)
            quantity = 0;
    }
    if ((mode & 4) != 0)
        quantity = ((encoded_effect & 0xf800) >> 11) + 1;

    if (quantity != 0 && quantity <= 30) {
        s32 ones = Func_080022fc(quantity, 10);
        s32 tens;

        SubmitEffectTransfer_0801a088(transfer, digit_transfers[ones], 1);
        tens = Func_080022ec(quantity, 10);
        if (tens != 0) {
            SubmitEffectTransfer_0801a088(
                transfer,
                digit_transfers[9 + tens],
                1);
        }
    }

    return 0x100;
}
