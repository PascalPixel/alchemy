#include "KORIMAKI.H"

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    KorimaPalette_SaveFirst();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = PaletteScene_AdjustColor(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); ColorBuffer_ApplyTarget(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_Divide(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Math_Divide(green, adjustment));
    blue = (s16)(blue - Math_Divide(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
