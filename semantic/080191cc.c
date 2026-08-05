#include "layout_guard.h"
#include "types.h"

struct SpriteSlot_080191cc {
    u8 padding00[4];
    u8 oam_y;
    u8 oam_mode;
    u16 oam_attr1;
    u16 oam_attr2;
    u8 padding0a[2];
};

struct MapEffect_080191cc {
    struct MapEffect_080191cc *next;
    u8 padding04;
    u8 kind;
    u16 x;
    u8 y;
    u8 padding09[3];
    u16 timer;
    s8 tile_index;
    u8 priority;
    struct SpriteSlot_080191cc sprite;
};

struct EffectList_080191cc {
    struct MapEffect_080191cc *head;
    u8 padding04[0x0e];
    u16 mode;
    u8 padding14[2];
    u16 flags;
    u8 padding18[0x0c];
};

struct FieldState_080191cc {
    u8 padding0000[0x500];
    struct EffectList_080191cc effect_lists[8];
    u8 padding0620[0x0c96];
    u16 map_id;
};

LAYOUT_SIZE_GUARD(SpriteSlot080191cc_Size, struct SpriteSlot_080191cc, 0x0c);
LAYOUT_OFFSET_GUARD(
    SpriteSlot080191cc_Y,
    struct SpriteSlot_080191cc,
    oam_y,
    0x04);
LAYOUT_OFFSET_GUARD(
    SpriteSlot080191cc_Attr1,
    struct SpriteSlot_080191cc,
    oam_attr1,
    0x06);
LAYOUT_OFFSET_GUARD(
    SpriteSlot080191cc_Attr2,
    struct SpriteSlot_080191cc,
    oam_attr2,
    0x08);
LAYOUT_OFFSET_GUARD(
    MapEffect080191cc_Kind,
    struct MapEffect_080191cc,
    kind,
    0x05);
LAYOUT_OFFSET_GUARD(
    MapEffect080191cc_X,
    struct MapEffect_080191cc,
    x,
    0x06);
LAYOUT_OFFSET_GUARD(
    MapEffect080191cc_Timer,
    struct MapEffect_080191cc,
    timer,
    0x0c);
LAYOUT_OFFSET_GUARD(
    MapEffect080191cc_Sprite,
    struct MapEffect_080191cc,
    sprite,
    0x10);
LAYOUT_SIZE_GUARD(EffectList080191cc_Size, struct EffectList_080191cc, 0x24);
LAYOUT_OFFSET_GUARD(
    EffectList080191cc_Mode,
    struct EffectList_080191cc,
    mode,
    0x12);
LAYOUT_OFFSET_GUARD(
    EffectList080191cc_Flags,
    struct EffectList_080191cc,
    flags,
    0x16);
LAYOUT_OFFSET_GUARD(
    FieldState080191cc_EffectLists,
    struct FieldState_080191cc,
    effect_lists,
    0x500);
LAYOUT_OFFSET_GUARD(
    FieldState080191cc_MapId,
    struct FieldState_080191cc,
    map_id,
    0x12b6);

extern struct FieldState_080191cc *Data_03001e8c;
extern u32 Data_03001800;

s32 Func_08002304(s32 value, s32 modulus);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
s32 Func_08003d28(const u32 *affine);
void Func_08003dec(struct SpriteSlot_080191cc *sprite, s32 priority);
s32 Func_08003fa4(s32 index, s32 size, const void *source);
u32 Func_08004458(void);
void Func_0801908c(struct MapEffect_080191cc *effect);

static void SetSpriteX_080191cc(struct SpriteSlot_080191cc *sprite, s32 x) {
    sprite->oam_attr1 =
        (sprite->oam_attr1 & 0xfe00) | (x & 0x01ff);
}

static void SetAffineSlot_080191cc(
    struct SpriteSlot_080191cc *sprite,
    s32 affine_slot
) {
    sprite->oam_attr1 =
        (sprite->oam_attr1 & 0xc1ff) | ((affine_slot & 31) << 9);
}

static void PlaceNormalSprite_080191cc(struct MapEffect_080191cc *effect) {
    struct SpriteSlot_080191cc *sprite = &effect->sprite;

    sprite->oam_attr1 &= 0xc1ff;
    sprite->oam_mode &= (u8)~3;
    SetSpriteX_080191cc(sprite, effect->x);
    sprite->oam_y = effect->y;
}

static s32 SmallJitter_080191cc(void) {
    u32 first = (Func_08004458() * 3) >> 16;
    u32 second = (Func_08004458() * 3) >> 16;

    return (first + second) / 2 - 1;
}

static void AnimateMapTile_080191cc(
    struct FieldState_080191cc *field,
    struct MapEffect_080191cc *effect
) {
    struct SpriteSlot_080191cc *sprite = &effect->sprite;
    s32 frame = (Data_03001800 >> 2) & 7;
    s32 tile;

    if (field->map_id == 0x60) {
        return;
    }

    tile = Func_08003fa4(
        field->map_id,
        0x80,
        (const u8 *)0x080368D4 + frame * 0x80);
    sprite->oam_attr2 = (sprite->oam_attr2 & 0xfc00) | (tile & 0x03ff);
    effect->tile_index = (s8)sprite->oam_attr2;
    sprite->oam_mode =
        ((((sprite->oam_mode & (u8)~0x0c) & (u8)~0x10) | 0x20) & 0x3f) |
        0x80;
    sprite->oam_y = (u8)(effect->y +
        ((const u8 *)0x08033E60)[Func_08002304(Data_03001800, 0x50)] + 2);
    sprite->oam_mode &= (u8)~3;
    sprite->oam_attr1 &= 0x01ff;
}

static void AnimateShrink_080191cc(struct MapEffect_080191cc *effect) {
    u32 affine[2] = {0x02000200, 0};

    if (effect->timer == 0) {
        PlaceNormalSprite_080191cc(effect);
        return;
    }

    SetAffineSlot_080191cc(&effect->sprite, Func_08003d28(affine));
    effect->sprite.oam_mode |= 3;
    SetSpriteX_080191cc(&effect->sprite, effect->x - 5);
    effect->sprite.oam_y = effect->y - 5;
    effect->timer--;
}

static void AnimateOrbit_080191cc(struct MapEffect_080191cc *effect) {
    u32 affine[2];
    u16 angle;

    effect->timer += 0x300;
    angle = effect->timer;
    affine[0] = 0x01000100;
    affine[1] = angle;
    SetAffineSlot_080191cc(&effect->sprite, Func_08003d28(affine));
    effect->sprite.oam_mode =
        (effect->sprite.oam_mode & (u8)~3) | 1;
    SetSpriteX_080191cc(
        &effect->sprite,
        effect->x - (Func_08002322((u16)(angle + 0xe800)) >> 14) - 2);
    effect->sprite.oam_y =
        effect->y - (Func_0800231c((u16)(angle + 0x6800)) >> 14) - 2;
}

static void AnimateBob_080191cc(struct MapEffect_080191cc *effect) {
    const s8 *offsets = (const s8 *)0x08033EB0;
    s32 x_offset_index;
    s32 y_offset_index;

    if (Data_03001800 & 1) {
        effect->timer++;
    }
    x_offset_index = Func_08002304(effect->timer, 0x14) * 2;
    SetSpriteX_080191cc(&effect->sprite, effect->x + offsets[x_offset_index]);
    y_offset_index = Func_08002304(effect->timer, 0x14) * 2 + 1;
    effect->sprite.oam_y = effect->y + (u8)offsets[y_offset_index] - 2;
}

static void AnimateLargeShrink_080191cc(struct MapEffect_080191cc *effect) {
    u32 affine[2] = {0x01400140, 0};

    if (effect->timer == 0) {
        PlaceNormalSprite_080191cc(effect);
        return;
    }

    SetAffineSlot_080191cc(&effect->sprite, Func_08003d28(affine));
    effect->sprite.oam_mode |= 3;
    SetSpriteX_080191cc(&effect->sprite, effect->x - 8);
    effect->sprite.oam_y = effect->y - 8;
    effect->timer--;
}

static void AnimateEffect_080191cc(
    struct FieldState_080191cc *field,
    struct MapEffect_080191cc *effect
) {
    const s8 *step = (const s8 *)0x08033EE8;

    switch (effect->kind) {
    case 2:
        AnimateMapTile_080191cc(field, effect);
        break;
    case 4:
        AnimateBob_080191cc(effect);
        break;
    case 5:
        if (Data_03001800 & 1) {
            SetSpriteX_080191cc(&effect->sprite, effect->x + SmallJitter_080191cc());
            effect->sprite.oam_y = effect->y + SmallJitter_080191cc();
        }
        break;
    case 6:
        AnimateShrink_080191cc(effect);
        break;
    case 7:
        AnimateOrbit_080191cc(effect);
        break;
    case 8:
        AnimateLargeShrink_080191cc(effect);
        break;
    case 9:
    case 10:
    case 11:
    case 12:
        Func_0801908c(effect);
        break;
    case 14:
    case 15:
    case 16:
        effect->timer++;
        effect->sprite.oam_y = effect->y + (u8)step[effect->timer & 15];
        break;
    case 17:
        effect->timer++;
        effect->sprite.oam_y = effect->y - (u8)step[effect->timer & 15];
        break;
    case 18:
        effect->timer++;
        SetSpriteX_080191cc(
            &effect->sprite,
            effect->x - step[effect->timer & 15]);
        effect->sprite.oam_y = effect->y + (u8)step[effect->timer & 15];
        break;
    }
}

/* Updates the eight active map-object lists and their attached sprite state. */
void Func_080191cc(void) {
    struct FieldState_080191cc *field = Data_03001e8c;
    u32 list_index;

    for (list_index = 0; list_index < 8; list_index++) {
        struct EffectList_080191cc *list = &field->effect_lists[list_index];
        struct MapEffect_080191cc *effect;

        if (!(list->flags & 1)) {
            continue;
        }

        for (effect = list->head; effect != 0; effect = effect->next) {
            if (list->mode == 4) {
                effect->timer = 2;
                effect->kind = 8;
            }

            AnimateEffect_080191cc(field, effect);
            if ((effect->kind != 2 || field->map_id != 0x60) &&
                effect->kind != 13) {
                Func_08003dec(&effect->sprite, effect->priority);
            }
        }
    }
}
