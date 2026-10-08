#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "WINDOW.H"
#include "AFFINE.H"
#include "BTLPTY.H"
#include "RESOURCE.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"

enum {
    TILEMAP_ENTRY_PLAIN,
    TILEMAP_ENTRY_NONE,
    TILEMAP_ENTRY_PALETTE_14,
    TILEMAP_ENTRY_PALETTE_15,
    TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

void UiWindow_SetTilemapEntry(
    struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode)
{
    u16 *map = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
    s32 palette;
    s32 index;

    y += 1;
    x += 1;
    if (y > (u32)(window->height - 1))
        return;
    if (x > (u32)(window->width - 1))
        return;

    switch (mode) {
    case TILEMAP_ENTRY_PALETTE_15:
        palette = 0xf000;
        break;
    case TILEMAP_ENTRY_PALETTE_14:
        palette = 0xe000;
        break;
    case TILEMAP_ENTRY_PALETTE_1:
        palette = 0x1000;
        break;
    default:
        palette = 0;
        break;
    }

    /* FAKEMATCH: needed to match: byte-offset stores preserve the strh base and offset register order; map[index] reverses them in both stores. */
    switch (mode) {
    case TILEMAP_ENTRY_NONE:
        return;
    case TILEMAP_ENTRY_PALETTE_14:
    case TILEMAP_ENTRY_PALETTE_15:
    case TILEMAP_ENTRY_PALETTE_1:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = palette | value;
        return;
    case TILEMAP_ENTRY_PLAIN:
    default:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = value;
        return;
    }
}

struct OutputSprite {
    u32 link;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 mosaic : 1;
    u8 colour : 1;
    u8 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    union {
        u16 value;
        struct {
            u16 tile : 10;
            u16 priority : 2;
            u16 palette : 4;
        } f;
    } attr2;
};

void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = (struct OutputSprite *)output->unknown_10;
    struct AffineTransform effect;

    switch ((u8)output->active) {
    case 9:
        scale = Ui_BounceScales[output->anim_step++ & 31];
        break;
    case 10:
        scale = Ui_BounceScales[output->anim_step++ & 31] >> 1;
        break;
    case 11:
        if (output->anim_step <= 7)
            scale = Ui_BounceScales[output->anim_step++ * 2 + 16];
        break;
    case 12:
        if (output->anim_step <= 7)
            scale = Ui_BounceScales[output->anim_step++ * 2 + 16] >> 1;
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
        sprite->x = output->x;
        sprite->y = output->y;
    } else {
        effect.scale_x = scale;
        effect.scale_y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            /* FAKEMATCH: needed to match: the unsigned halfword read of the x cell; a cast of the signed member swaps the x load and the -8 pool load. */
            sprite->x = *(u16 *)&output->x - 8;
            sprite->y = (u16)output->y - 8;
        } else {
            sprite->affine = 1;
            sprite->x = output->x;
            sprite->y = output->y;
        }
    }
}

struct SpriteOutput {
    struct SpriteOutput *next;
    u8 unknown_04;
    u8 mode;
    u16 x;
    u16 y;
    u16 unknown_0a;
    u16 frame;
    u8 tile;
    u8 priority;
    struct OutputSprite sprite;
};

extern u32 gFrameTick;

u32 Random16(void);

void UiWork_AnimateSpriteSlots(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    struct UiWindow *window = work->windows;
    s32 no;
    struct AffineTransform effect;

    for (no = 0; no != WINDOW_COUNT; window++, no++) {
        struct SpriteOutput *output;
        u32 phase;

        if (!(window->flags & 1))
            continue;
        output = (struct SpriteOutput *)window->output.head;
        phase = (gFrameTick >> 2) & 7;
        while (output != NULL) {
            struct OutputSprite *sprite = &output->sprite;

            if (window->effect == 4) {
                output->frame = 2;
                output->mode = 8;
            }
            switch (output->mode) {
            case 2:
                if (work->glyph_resource != 0x60) {
                    const u8 *tiles = &UiSprite_CursorFrameTiles[phase * 128];

                    sprite->attr2.f.tile = VramBlock_LoadCached(
                        work->glyph_resource, 128, tiles);
                    output->tile = sprite->attr2.f.tile;
                    sprite->mode = 0;
                    sprite->mosaic = 0;
                    sprite->colour = 1;
                    sprite->size = 0;
                    sprite->shape = 2;
                    sprite->y = output->y + UiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
                    sprite->affine = 0;
                    sprite->affine_index = 0;
                }
                break;
            case 5:
                if (gFrameTick & 1) {
                    sprite->x = output->x + (s32)(((Random16() * 3 >> 16) +
                        (Random16() * 3 >> 16)) >> 1) - 1;
                    sprite->y = output->y + (s32)(((Random16() * 3 >> 16) +
                        (Random16() * 3 >> 16)) >> 1) - 1;
                }
                break;
            case 6:
                if (output->frame == 0)
                    goto reset;
                effect.scale_x = 512;
                effect.scale_y = 512;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                sprite->x = output->x - 5;
                sprite->y = output->y - 5;
                output->frame--;
                break;
            case 7:
                effect.scale_x = 256;
                effect.scale_y = 256;
                output->frame += 768;
                effect.angle = output->frame;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 1;
                sprite->x = output->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
                sprite->y = output->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
                break;
            case 4:
                if (gFrameTick & 1)
                    output->frame++;
                sprite->x = output->x + UiSprite_WobbleOffsets[output->frame % 20][0];
                sprite->y = output->y + UiSprite_WobbleOffsets[output->frame % 20][1] - 2;
                break;
            case 17:
                output->frame++;
                sprite->y = output->y - UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 14:
            case 15:
            case 16:
                output->frame++;
                sprite->y = output->y + UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 18:
                output->frame++;
                sprite->x = output->x - UiSprite_BounceOffsets[output->frame & 15];
                sprite->y = output->y + UiSprite_BounceOffsets[output->frame & 15];
                break;
            case 8:
                if (output->frame == 0)
                    goto reset;
                effect.scale_x = 320;
                effect.scale_y = 320;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                sprite->x = output->x - 8;
                sprite->y = output->y - 8;
                output->frame--;
                break;
            reset:
                sprite->affine_index = 0;
                sprite->affine = 0;
                sprite->x = output->x;
                sprite->y = output->y;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
                RenderOutput_UpdateScaleAnimation((struct RenderOutput *)output);
                break;
            }
            if (output->mode == 2) {
                if (work->glyph_resource != 0x60)
                    Runtime_PushSlotEntry(sprite, output->priority);
            } else if (output->mode != 13) {
                Runtime_PushSlotEntry(sprite, output->priority);
            }
            output = output->next;
            phase = (gFrameTick >> 2) & 7;
        }
    }
}
