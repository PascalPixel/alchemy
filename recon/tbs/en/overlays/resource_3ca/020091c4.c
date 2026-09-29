/* Draft of BabiFune_StepFade, resource_3ca at 0x020091c4, for
 * FIELD/BABI_FUNE. Linking it needs the 288-byte buffer that opens the
 * overlay's .bss labelled BabiFune_FadeSprites and the import veneer to
 * Runtime_PushSlotEntryFar labelled Runtime_PushSlotEntry.
 * Remaining difference: 13 register-only, 6 operand and 4 reordered
 * instructions (score 940). The game keeps the sprite pointer in r0 and the
 * step in r1, materialises 0 and 255 inside the first loop, and walks the
 * second and third columns with a copy of the pointer (adds r1, r0, #0)
 * while the second loop still advances r0; here the pointer itself walks.
 * Tried: a struct pointer throughout, a word index, and permute (535 with
 * temporaries that no programmer would write). */
#include "TYPES.H"
#include "VRAM_BLOCK.H"

/* A sprite as the slot entries take it: its attributes packed in the
   second word and its tile in the third. */
struct FadeSprite {
    u32 unused;
    u32 attributes;
    u32 tile;
};

extern struct FadeSprite BabiFune_FadeSprites[24];
extern s16 BabiFune_FadeSlot;
extern s16 BabiFune_FadeStep;

void Runtime_PushSlotEntry(struct FadeSprite *entry, s32 priority);

/* Each frame of the Babi Fune fade: count the step down and push three
   columns of eight wide sprites, the first sliding in from the left edge
   and the other two from the right as the step shrinks. */
void BabiFune_StepFade(void)
{
    u32 *entry;
    u32 tile;
    s32 step;
    s32 x;
    u32 i;

    tile = gVramBlockCache[BabiFune_FadeSlot].offset >> 5;
    entry = (u32 *)BabiFune_FadeSprites;
    if (BabiFune_FadeStep != 0)
        BabiFune_FadeStep--;
    for (i = 0; i < 8; i++) {
        step = BabiFune_FadeStep;
        *entry++ = 0;
        *entry++ = ((-step / 2) & 0xff) | (i << 21) | 0x80004000;
        *entry++ = tile;
    }
    x = (step / 2 + 136) & 0xff;
    for (i = 0; i < 8; i++) {
        entry[1] = (i << 21) | x | 0x80004000;
        entry[0] = 0;
        entry[2] = tile;
        entry += 3;
    }
    x = (BabiFune_FadeStep / 2 + 152) & 0xff;
    for (i = 0; i < 8; i++) {
        entry[1] = (i << 21) | x | 0x80004000;
        entry[0] = 0;
        entry[2] = tile;
        entry += 3;
    }
    for (i = 0; i < 24; i++)
        Runtime_PushSlotEntry(&BabiFune_FadeSprites[i], 255);
}
