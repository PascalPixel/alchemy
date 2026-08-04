#include "types.h"

struct SpriteAttributes_080c0f98 {
    u8 padding00[5];
    u8 attribute0_high;
    u8 padding06[0x0b];
    u8 attribute1_high;
};

struct RenderObject_080c0f98 {
    u8 padding00[0x50];
    void *sprite_storage;
    u8 kind;
};

struct BattleObjectSlot_080c0f98 {
    struct RenderObject_080c0f98 *object;
};

struct BattleObjectSlot_080c0f98 *Func_080b7dd0(s32);

static void SetPaletteBank_080c0f98(
    struct SpriteAttributes_080c0f98 *sprite,
    u8 palette_bits)
{
    sprite->attribute0_high =
        (u8)((sprite->attribute0_high & ~0x0c) | palette_bits);
    sprite->attribute1_high =
        (u8)((sprite->attribute1_high & ~0x0c) | palette_bits);
}

/*
 * Select one of four palette banks for a battle object's sprite data.  Kind
 * 1 owns one inline sprite record; kind 2 owns a null-terminated array of up
 * to four records.  Other object kinds do not carry this palette layout.
 */
void Func_080c0f98(s32 object_id, s32 palette_bank)
{
    struct BattleObjectSlot_080c0f98 *slot = Func_080b7dd0(object_id);
    struct RenderObject_080c0f98 *object;
    u8 palette_bits = (u8)((palette_bank & 3) << 2);

    if (slot == 0)
        return;
    object = slot->object;
    if (object == 0)
        return;

    switch (object->kind & 0x0f) {
    case 1:
        SetPaletteBank_080c0f98(
            (struct SpriteAttributes_080c0f98 *)object->sprite_storage,
            palette_bits);
        break;

    case 2:
    {
        struct SpriteAttributes_080c0f98 **sprites =
            (struct SpriteAttributes_080c0f98 **)object->sprite_storage;
        s32 index;

        for (index = 0; index < 4 && sprites[index] != 0; index++)
            SetPaletteBank_080c0f98(sprites[index], palette_bits);
        break;
    }
    }
}
