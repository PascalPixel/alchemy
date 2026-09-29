/* DRAFT: complete 320-byte RenderOutput_UpdateScaleAnimation.
 * Corrected from its own listing using Ui_ApplyTableScaleToObject's sprite
 * attributes and AffineMatrix_BuildForEffect's eight-byte effect record.
 * frame is +12, sprite is +16, and modes 11/12 use frame * 2 + 16.
 * 2026-09-26: 316/320 bytes, 137 differing halfwords / 45 aligned edits.
 * Entry and affine-effect construction match. Remaining: modes 11/12 keep
 * old-counter copies in the reference; case 10 shares only the shift, not
 * its table load, with case 12. Reference affine 0/1 tails share one byte
 * store; this draft retains separate stores. Counter scratch registers and
 * final x-load order differ. A u16 local adds redundant reads/shift masks
 * (340 bytes, 74 edits); retain u32. No further declaration permutations.
 * Owner was bundled with 080191cc; its complete pool ends at 080191cc.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 38,868
 * candidates; the best scored 685 against the old draft's 1080 (21
 * register-only, 11 operand, 1 reordered, 1 inserted, 2 deleted). Its one
 * needed rewrite, kept here, reads the scale table through a local pointer
 * in modes 11 and 12, which restores both reloads of the table's address.
 * Data_080366f8 still needs its own ROM label; the remaining difference is
 * register choice in the two mode blocks and the sprite flag byte.
 */
#include "TYPES.H"

struct ScaleEffect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};
struct OutputSprite {
    u32 link;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 other : 4;
    u16 x : 9;
    u16 affine_index : 5;
    u16 other_x : 2;
    u16 tile;
};
struct AnimatedOutput {
    u8 unknown_00[5];
    u8 mode;
    u16 x;
    u16 y;
    u16 unknown_0a;
    u16 frame;
    u8 unknown_0e[2];
    struct OutputSprite sprite;
};
extern u16 Data_080366f8[];
s32 AffineMatrix_BuildForEffect(struct ScaleEffect *);

void RenderOutput_UpdateScaleAnimation(struct AnimatedOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = &output->sprite;
    u32 frame;
    struct ScaleEffect effect;

    switch (output->mode) {
    case 9:
        frame = output->frame;
        output->frame++;
        scale = Data_080366f8[frame & 31];
        break;
    case 10:
        frame = output->frame;
        output->frame++;
        scale = Data_080366f8[frame & 31] >> 1;
        break;
    case 11:
        frame = output->frame;
        if (frame <= 7) {
            u16 *table;
            output->frame++;
            table = Data_080366f8;
            scale = table[frame * 2 + 16];
        }
        break;
    case 12:
        frame = output->frame;
        if (frame <= 7) {
            u16 *table;
            output->frame++;
            table = Data_080366f8;
            scale = table[frame * 2 + 16] >> 1;
        }
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
    } else {
        effect.x = scale;
        effect.y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            sprite->x = output->x + 0xfff8;
            sprite->y = *(u8 *)&output->y + 0xf8;
            return;
        }
        sprite->affine = 1;
    }
    sprite->x = output->x;
    sprite->y = output->y;
}
